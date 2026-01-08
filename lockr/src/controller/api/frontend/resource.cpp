#include "controller/api/frontend/resource.h"

#include <fstream>

namespace lockr {
    void GetStyle(const httplib::Request& req, httplib::Response& res) {
        std::string fileName = req.matches[1].str();
        std::string extension = ".css";
        for (int i = 0; i < extension.length(); i++){
            if(fileName[fileName.length() - i] != extension[extension.length() - i]){
                fileName.append(extension);
                break;
            }
        }

        std::string out;

        std::ifstream file(RESOURCE_PATH "frontend/style/" + fileName);
        if(file.fail()){
            res.status = httplib::NotFound_404;
            return;
        }

        std::string line;
        while (getline (file, line)) {
            out += line;
        }
        file.close();

        res.set_content(out, "text/css");
        res.status = httplib::OK_200;
        return;
    }

    void GetCode(const httplib::Request& req, httplib::Response& res) {
        std::string fileName = req.matches[1].str();
        std::string extension = ".js";
        for (int i = 0; i < extension.length(); i++){
            if(fileName[fileName.length() - i] != extension[extension.length() - i]){
                fileName.append(extension);
                break;
            }
        }

        std::string out;

        std::ifstream file(RESOURCE_PATH "frontend/code/" + fileName);
        if(file.fail()){
            res.status = httplib::NotFound_404;
            return;
        }

        std::string line;
        while (getline (file, line)) {
            out += line;
        }
        file.close();

        res.set_content(out, "text/javascript");
        res.status = httplib::OK_200;
        return;
    }

    void GetPng(const httplib::Request& req, httplib::Response& res) {
        std::string fileName = req.matches[1].str();

        std::string extension = ".png";
        for (int i = 0; i < extension.length(); i++){
            if(fileName[fileName.length() - i] != extension[extension.length() - i]){
                fileName.append(extension);
                break;
            }
        }

        std::ifstream file(RESOURCE_PATH "frontend/resources/" + fileName, std::ios::binary);
        if(file.fail()){
            res.status = httplib::NotFound_404;
            return;
        }

        std::string out(
            (std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>()
        );

        res.set_content(out, "image/png");
        res.status = httplib::OK_200;
        return;
    }
}