/*
 * bcrypt wrapper library
 *
 * Written in 2011, 2013, 2014, 2015 by Ricardo Garcia <r@rg3.name>
 * Windows RNG support added for MSVC/CNG.
 *
 * CC0 1.0 Universal
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

/* ---------- Platform I/O / RNG headers ---------- */
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>   /* must precede <bcrypt.h> */
#include <bcrypt.h>    /* BCryptGenRandom(), NTSTATUS, BCRYPT_*  */

/* MSVC convenience: auto-link bcrypt.lib (no effect on other toolchains) */
#ifdef _MSC_VER
#pragma comment(lib, "bcrypt.lib")
#endif

#include <BaseTsd.h>   /* for SSIZE_T */
typedef SSIZE_T ssize_t;
#include <io.h>
#else
#include <sys/types.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  #include <unistd.h>
#endif

/* ---------- Local bcrypt wrapper & Openwall headers ---------- */
#include "bcrypt.h"
#include "libbcrypt.h"                 /* defines BCRYPT_HASHSIZE and prototypes */
#include "crypt_blowfish/ow-crypt.h"/* crypt_gensalt_rn(), crypt_rn()        */

/* Amount of random bytes to seed salt generation */
#define RANDBYTES (16)

/* ---------- Helpers ---------- */
#if defined(_WIN32)
/* Use Windows CNG system RNG */
static int get_random_bytes(void *dst, size_t len) {
    NTSTATUS st = BCryptGenRandom(
            NULL,                    /* use system-preferred provider */
            (PUCHAR)dst,
            (ULONG)len,
            BCRYPT_USE_SYSTEM_PREFERRED_RNG
    );
    return (st == 0) ? 0 : -1;
}
#else
/* Retry close that may be interrupted by signals */
static int try_close(int fd) {
    for (;;) {
        errno = 0;
        if (close(fd) == -1 && errno == EINTR) continue;
        break;
    }
    return 0;
}

/* Read exactly count bytes or fail */
static int try_read(int fd, char *out, size_t count) {
    size_t total = 0;
    while (total < count) {
        ssize_t n;
        for (;;) {
            errno = 0;
            n = read(fd, out + total, count - total);
            if (n == -1 && errno == EINTR) continue;
            break;
        }
        if (n < 1) return -1;
        total += (size_t)n;
    }
    return 0;
}
#endif

/* Constant-time strcmp. Returns 0 when equal, non-zero otherwise. */
static int timing_safe_strcmp(const char *s1, const char *s2) {
    const unsigned char *u1 = (const unsigned char *)s1;
    const unsigned char *u2 = (const unsigned char *)s2;

    int len1 = (int)strlen(s1);
    int len2 = (int)strlen(s2);
    if (len1 != len2) return 1;

    int acc = 0;
    for (int i = 0; i < len1; ++i) acc |= (u1[i] ^ u2[i]);
    return acc;
}

/* ---------- Public API ---------- */

int bcrypt_gensalt(int factor, char salt[BCRYPT_HASHSIZE]) {
    char input[RANDBYTES];

#if defined(_WIN32)
    /* Windows: CNG RNG */
    if (get_random_bytes(input, RANDBYTES) != 0) {
        return 2; /* random failure */
    }
#else
    /* POSIX: /dev/urandom */
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd == -1) return 1;

    if (try_read(fd, input, RANDBYTES) != 0) {
        try_close(fd);
        return 2;
    }
    try_close(fd);
#endif

    /* Cost clamped to sane default if out of range */
    int workf = (factor < 4 || factor > 31) ? 12 : factor;

    /* Generate salt ($2a$ or $2b$ are both understood by ow-crypt) */
    char *aux = crypt_gensalt_rn("$2a$", workf, input, RANDBYTES,
                                 salt, BCRYPT_HASHSIZE);
    return (aux == NULL) ? 5 : 0;
}

int bcrypt_hashpw(const char *passwd,
                  const char salt[BCRYPT_HASHSIZE],
                  char hash[BCRYPT_HASHSIZE]) {
    char *aux = crypt_rn(passwd, salt, hash, BCRYPT_HASHSIZE);
    return (aux == NULL) ? 1 : 0;
}

int bcrypt_checkpw(const char *passwd, const char hash[BCRYPT_HASHSIZE]) {
    char buf[BCRYPT_HASHSIZE];
    if (bcrypt_hashpw(passwd, hash, buf) != 0) return -1;
    return timing_safe_strcmp(hash, buf);
}
