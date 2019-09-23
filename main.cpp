#include <cpprest/uri.h>
#include <cpprest/asyncrt_utils.h>
#include "api.h"

std::unique_ptr<MyAPI> api;

void OnStart(const utility::string_t addr){
    web::uri_builder api_uri(addr);
    api_uri.append_path(U("/v1/increment"));
    api=std::unique_ptr<MyAPI>(new MyAPI(api_uri.to_uri().to_string()));
    api->open().wait();
}

void OnEnd(){
    api->close().wait();
}

int main(){

    OnStart("http://0.0.0.0:8080");

    std::cout << "Write exit and press Enter to exit" << std::endl;

    std::string line;
    do {
        std::getline(std::cin, line);
    }
    while (line!="exit");

    OnEnd();

    return 0;
}