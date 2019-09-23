#include "api.h"

MyAPI::MyAPI(utility::string_t uri):listener(uri){
    pdb = new Db(NULL,0);
    pdb->open(NULL,"db.db",NULL,DB_BTREE,DB_CREATE|DB_TRUNCATE,0);
    listener.support(web::http::methods::POST,std::bind(&MyAPI::handle_post, this, std::placeholders::_1));
    // listener.support(web::http::methods::GET,std::bind(&MyAPI::handle_get, this, std::placeholders::_1));
    // listener.support(web::http::methods::GET,std::bind(&MyAPI::handle_put, this, std::placeholders::_1));
    // listener.support(web::http::methods::GET,std::bind(&MyAPI::handle_delete, this, std::placeholders::_1));
}
void MyAPI::handle_post(web::http::http_request msg){
    
    msg.extract_string().then([=](const utility::string_t str){
        int val,lastval=-1;
        std::string keys="value";
        Dbt key(const_cast<char*>(keys.data()),keys.size());
        char buffer[sizeof(int)];
        Dbt data;
        data.set_data(buffer);
        data.set_ulen(sizeof(int));
        data.set_flags(DB_DBT_USERMEM);
        if (pdb->get(NULL, &key, &data, 0) != DB_NOTFOUND) {
                lastval=*reinterpret_cast<int*>(buffer);
        }
        val=std::atoi(str.c_str());
        if(lastval>=val){
            msg.reply(web::http::status_codes::BadRequest);
            std::cout << "New value is equal or less" << std::endl;
            return;
        }
        Dbt value(&val,sizeof(val));
        pdb->put(NULL,&key,&value,0);
        msg.reply(web::http::status_codes::OK,std::to_string(val+1));
    });
}

// void MyAPI::handle_get(web::http::http_request msg){
//     msg.reply(web::http::status_codes::BadRequest);
// }

// void MyAPI::handle_put(web::http::http_request msg){
//     // std::cout<<"HIGET"<<std::endl;
//     msg.reply(web::http::status_codes::BadRequest);
// }

// void MyAPI::handle_delete(web::http::http_request msg){
//     // std::cout<<"HIGET"<<std::endl;
//     msg.reply(web::http::status_codes::BadRequest);
// }