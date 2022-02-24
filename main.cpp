#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include "locker.h"
#include "threadpool.h"
#include <signal.h>
#include "http_conn.h"

#define MAX_FD 65535 //最大的文件描述符个数
#define MAX_EVENT_NUMBER 10000 //监听的最大的事件数量
//添加信号捕捉
void addsig(int sig, void(handler)(int)){
    //信号处理函数,sig:要处理的信号，回调函数一个函数指针
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = handler;//回调函数
    sigfillset(&sa.sa_mask);//临时阻塞信号集，都是阻塞的。将信号集中所有标志位都置为1
    sigaction(sig, &sa, NULL);
    /*sigaction()功能：捕捉某个信号，改变它的默认处理
    参数：  
            signum：需要捕捉的信号的编号或者宏值（除了SIGKILL和SIGSTOP）
            act:处理信号的回调函数
            oldact：上一次对信号捕捉的相关设置，一般不使用，传递NULL
    返回值：成功返回0，失败返回-1。
    */
}

int main(int argc, char* argv[]){
    //命令行传递参数
    if(argc <= 1){
        //参数个数，至少要传递端口号
        printf("按照如下格式运行： %s port_number\n", basename(argv[0]));//argv[0]是程序名
        exit(-1);
    }
    //获取端口号
    int port = atoi(argv[1]);//argv[0]是程序名

    //对SIGPIE信号进行处理
    addsig(SIGPIPE, SIG_IGN);//SIGPIPE:Broken pipe向一个没有读端的管道写数据
    

    //创建线程池，初始化线程池。任务和信息都在http_conn中
    threadpool<http_conn> * pool = NULL;
    try{
        pool = new threadpool<http_conn>;
    }catch(...){
        exit(-1);
    }

    //创建一个数组用于保存所有的客户端信息
    http_conn * users = new http_conn[MAX_FD];
    
    //网络部分
    //创建监听的套接字
    int listenfd = socket(PF_INET, SOCK_STREAM, 0);

    //补充判断有没有问题

    //设置端口复用，必须在绑定之前设置
    int reuse = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    //绑定
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    bind(listenfd, (struct sockaddr*)&address, sizeof(address));
    
    //监听
    listen(listenfd, 5);

    //创建epoll对象，事件数组，添加
    epoll_event events[MAX_EVENT_NUMBER];
    int epollfd = epoll_create(5);

    //将监听的文件描述符添加到epoll对象中

    return 0;
}