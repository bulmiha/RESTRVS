#pragma once

#include <cpprest/json.h>
#include <cpprest/http_listener.h>
#include <cpprest/uri.h>
#include <cpprest/asyncrt_utils.h>
#include <db_cxx.h>
#include <dbstl_map.h>
#include <boost/format.hpp>


class MyAPI{
    public:
        MyAPI(utility::string_t uri);
        pplx::task<void> open() { return listener.open(); }
        pplx::task<void> close() { return listener.close(); }
        ~MyAPI();

    private:
        web::http::experimental::listener::http_listener listener;

        void handle_get(web::http::http_request msg);

        void handle_put(web::http::http_request msg);

        void handle_post(web::http::http_request msg);

        void handle_delete(web::http::http_request msg);

        web::json::value val;

        struct val
        {
            int val;
        };

        Db * pdb;
        
};