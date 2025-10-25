#include "controller/api/frontend/page.h"

#include <fstream>

namespace lockr {
    void GetPage(const httplib::Request& req, httplib::Response& res){
        std::string fileName = req.matches[1].str();
        std::string extension = ".html";
        for (int i = 0; i < extension.length(); i++){
            if(fileName[fileName.length() - i] != extension[extension.length() - i]){
                fileName.append(extension);
                break;
            }
        }

        std::string out;

        std::ifstream file(RESOURCE_PATH "frontend/pages/" + fileName);
        if(file.fail()){
            res.status = httplib::NotFound_404;
            return;
        }

        std::string line;
        while (getline (file, line)) {
            out += line;
        }
        file.close();

        res.set_content(out, "text/html");
        res.status = httplib::OK_200;
        return;
    }

    void GetRootPage(const httplib::Request& req, httplib::Response& res){
        std::string out;

        std::ifstream file(RESOURCE_PATH "frontend/pages/index.html");
        if(file.fail()){
            res.status = httplib::NotFound_404;
            return;
        }

        std::string line;
        while (getline (file, line)) {
            out += line;
        }
        file.close();

        res.set_content(out, "text/html");
        res.status = httplib::OK_200;
        return;
    }
}