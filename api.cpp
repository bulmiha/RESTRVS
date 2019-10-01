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

void MyAPI::handle_post(web::http::http_request msg){
    
    msg.extract_string().then([&](const utility::string_t str){
        int val,lastval;
        std::string keys="value";
        val=std::atoi(str.c_str());
        redisclient::RedisValue result;
        result=redis->command("GET",{keys});
        if(result.isError()){
            lastval=INT_MIN;
        }
        else {
            lastval=std::atoi(result.toString().c_str());
        }
        if(lastval>=val){
            msg.reply(web::http::status_codes::BadRequest);
            printf("New value %d is equal or less than save one(%d)\n",val,lastval);
            return;
        }
        result=redis->command("SET",{keys,std::to_string(val)});
        if(result.isError()){
            printf("Can't save given value\n");
        }
        printf("Received value of %d\n",val);
        msg.reply(web::http::status_codes::OK,std::to_string(val+1));
    });
}