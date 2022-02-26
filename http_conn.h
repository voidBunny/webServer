#ifndef HTTPCONNECTION_H
#define HTTPCONNECTION_H

#include <sys/epoll.h>
class http_conn{
    public:
        http_conn(){};
        ~http_conn(){};
        void process();//处理客户端的请求、响应
    private:
};
#endif