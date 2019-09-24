#include "api.h"

MyAPI::MyAPI(utility::string_t uri):listener(uri){
    pdb = new Db(NULL,0);
    pdb->open(NULL,"db.db",NULL,DB_BTREE,DB_CREATE,0);
    
    listener.support(web::http::methods::POST,std::bind(&MyAPI::handle_post, this, std::placeholders::_1));
}

MyAPI::~MyAPI(){
    pdb->close(0);
}

void MyAPI::handle_post(web::http::http_request msg){
    
    msg.extract_string().then([&](const utility::string_t str){
        int val,lastval;
        std::string keys="value";
        Dbt key(const_cast<char*>(keys.data()),keys.size());
        Dbt data;
        data.set_data(&lastval);
        data.set_ulen(sizeof(lastval));
        data.set_flags(DB_DBT_USERMEM);
        if (pdb->get(NULL, &key, &data, 0) == DB_NOTFOUND) {
               lastval=INT_MIN;
        }
        val=std::atoi(str.c_str());
        if(lastval>=val){
            msg.reply(web::http::status_codes::BadRequest);
            printf("New value %d is equal or less than save one(%d)\n",val,lastval);
            return;
        }
        Dbt value(&val,sizeof(val));
        pdb->put(NULL,&key,&value,0);
        printf("Received value of %d\n",val);
        msg.reply(web::http::status_codes::OK,std::to_string(val+1));
    });
}