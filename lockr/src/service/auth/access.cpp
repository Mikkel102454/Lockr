#include "service/auth/access.h"

#include "utils/env.h"

#include "jwt-cpp/jwt.h"
#include "httplib.h"

namespace lockr {
    bool ValidateAccessToken(const std::string &token) {
        try{
            std::string key = GetEnv("ACCESS_TOKEN_KEY");
            auto verifier = jwt::verify()
                    .allow_algorithm(jwt::algorithm::hs256(key))
                    .with_issuer("Lockr_Auth");

            verifier.verify(jwt::decode(token));
            return true;
        } catch (const std::exception& e) {
            std::cout << e.what() << "\n";
            return false;
        }
    }
    bool ValidateAccessToken(const std::string &token, std::string& outUserId) {
        try{
            std::string key = GetEnv("ACCESS_TOKEN_KEY");
            auto verifier = jwt::verify()
                    .allow_algorithm(jwt::algorithm::hs256(key))
                    .with_issuer("Lockr_Auth");

            auto decodedJwt = jwt::decode(token);
            verifier.verify(decodedJwt);

            outUserId = decodedJwt.get_subject();

            return true;
        } catch (const std::exception& e) {
            std::cout << e.what() << "\n";
            return false;
        }
    }

    int CreateNewAccessToken(std::string &outToken, const std::string &userId) {

        std::string key = GetEnv("ACCESS_TOKEN_KEY");
        outToken = jwt::create()
                .set_issuer("Lockr_Auth")
                .set_type("JWT")
                .set_subject(userId)
                .set_issued_now()
                .set_expires_at(std::chrono::system_clock::now() + std::chrono::seconds(600))
                .sign(jwt::algorithm::hs256(jwt::algorithm::hs256(key)));
        return httplib::OK_200;
    }
}