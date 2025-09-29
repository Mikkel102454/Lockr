#include "crypt_blowfish/ow-crypt.h"

/* These are declared in the other C files you already build */
extern char *_crypt_gensalt_blowfish_rn(const char *prefix, unsigned long count,
                                        const char *input, int size,
                                        char *output, int output_size);

extern char *_crypt_blowfish_rn(const char *key, const char *setting,
                                char *output, int size);

char *crypt_gensalt_rn(const char *prefix, unsigned long count,
                       const char *input, int size,
                       char *output, int output_size) {
    return _crypt_gensalt_blowfish_rn(prefix, count, input, size, output, output_size);
}

char *crypt_rn(const char *key, const char *setting,
               char *output, int size) {
    return _crypt_blowfish_rn(key, setting, output, size);
}
