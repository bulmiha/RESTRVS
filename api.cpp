#include "api.h"

MyAPI::MyAPI(utility::string_t uri):listener(uri){
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::resolver resolver(io_service);
    std::string dbhost,dbport;
    if(const char *env_p = std::getenv("DB_HOST")){
        dbhost=env_p;
    }
    else{
        dbhost="redis";
    }
    if(const char *env_p = std::getenv("DB_PORT")){
        dbport=env_p;
    }
    else{
        dbport="6379";
    }
    boost::asio::ip::tcp::resolver::query query(dbhost, dbport);
    boost::asio::ip::tcp::resolver::iterator iter = resolver.resolve(query);

    boost::asio::ip::tcp::endpoint endpoint = iter->endpoint();

    boost::asio::io_service ioService;
    redis = new redisclient::RedisSyncClient(ioService);
    boost::system::error_code ec;

    redis->connect(endpoint,ec);


    if(ec)
    {
        std::cerr << "Can't connect to redis: " << ec.message() << std::endl;
        exit(EXIT_FAILURE);
    }

    listener.support(web::http::methods::POST,std::bind(&MyAPI::handle_post, this, std::placeholders::_1));
}

MyAPI::~MyAPI(){
    redis->disconnect();
}
void MyAPI::badData(web::http::http_request &msg)
{
    printf("Bad data\n");
    web::json::value err;
    err["error"] = web::json::value::string(U("Wrong data"));
    err["type"] = web::json::value::number(1);
    msg.reply(web::http::status_codes::BadRequest,err);
}

uint MyAPI::checkVal(int v){
    redisclient::RedisValue result;
    result=redis->command("GET",{std::to_string(v)});
    if(result.isError()){
            return 1;
    }
    result=redis->command("GET",{std::to_string(v+1)});
    if(result.isError()){
            return 2;
    }
    redis->command("SET",{std::to_string(v),"True"});
    return 0;
}

void MyAPI::handle_post(web::http::http_request msg){

    msg.extract_json().then([&](web::json::value jsonMsg){
        int val;
        try{
            val=jsonMsg["number"].as_integer();
        }
        catch(web::json::json_exception e){
            badData(msg);
            return;
        }
        if(val<0){
            badData(msg);
            return;
        }
        web::json::value result;
        switch(checkVal(val)){
            case 0:
            {
                result["result"] = web::json::value::number(val+1);
                msg.reply(web::http::status_codes::OK,result);
                return;
                break;
            }
            case 1:
            {
                result["error"] = web::json::value::string(U("Value exists"));
                result["type"] = web::json::value::number(1);
                msg.reply(web::http::status_codes::BadRequest,result);
                return;
                break;
            }
            case 2:
            {
                result["error"] = web::json::value::string(U("Value is 1 less than existing"));
                result["type"] = web::json::value::number(2);
                msg.reply(web::http::status_codes::BadRequest,result);
                return;
                break;
            }
        }
    });
}