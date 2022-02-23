#ifndef LOCKER_H
#define LOCKER_H
//线程同步机制封装类

#include <pthread.h>
#include <exception>
#include <semaphore.h>

//互斥锁类
class locker{
    public:
        locker(){
            if(pthread_mutex_init(&m_mutex, NULL) != 0){
                //函数成功完成之后会返回零，其他任何返回值都表示出现了错误。
                throw std::exception();//抛出异常对象
            }
        }

        ~locker(){
            pthread_mutex_destroy(&m_mutex);
        }

        bool lock(){
            //上锁
            return pthread_mutex_lock(&m_mutex) == 0;
        }

        bool unlock(){
            //解锁
            return pthread_mutex_unlock(&m_mutex) == 0;
        }

        pthread_mutex_t * get(){
            //获得互斥量
            return &m_mutex;
        }
    private:
        pthread_mutex_t m_mutex;
};

//条件变量类 condition
class cond{
    public:
        cond(){
            if(pthread_cond_init(&m_cond, NULL) != 0){
                throw std:: exception();
            }
        }
        ~cond(){
            pthread_cond_destroy(&m_cond);
        }
        bool wait(pthread_mutex_t * mutex){
            return pthread_cond_wait(&m_cond, mutex) == 0;
        }
        bool timedwait(pthread_mutex_t * mutex, struct timespec t){
            return pthread_cond_timedwait(&m_cond, mutex, &t) == 0;
        }
        bool signal(){
            //让一个或多个线程唤醒
            return pthread_cond_signal(&m_cond) == 0;
        }
        bool broadcast(){
            return pthread_cond_broadcast(&m_cond) == 0;
        }

    private:
        pthread_cond_t m_cond;
};

//信号量类
class sem{
    public:
        sem(){
            if(sem_init(&m_sem, 0, 0) != 0){
                throw std::exception();
            }
        }

        sem(int num){
            if(sem_init(&m_sem, 0, num) != 0){
                throw std::exception();
            }
        }

        ~sem(){
            sem_destroy(&m_sem);
        }

        //等待信号量
        bool wait(){
            return sem_wait(&m_sem) == 0;
        }

        //增加信号量
        bool post(){
            return sem_post(&m_sem) == 0;
        }

    private:
        sem_t m_sem;
};
#endif