//
//  GTimerThread.cpp
//  ABL
//
//  Created by Caleb Jordan on 5/17/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#include "GTimerThread.h"
#include <sys/time.h>

void *serveRequests(void *);
bool timeValComp(timeval left, timeval right);

GTimerThread::GTimerThread()
{
    serveFlag = true;
    
    pthread_create(&thread, NULL, serveRequests, (void*)this);
    pthread_cond_init(&wakeFlag, NULL);
    pthread_mutex_init(&queueLock, NULL);
}

GTimerThread::~GTimerThread()
{
    pthread_mutex_lock(&queueLock);
    serveFlag = false;
    pthread_cond_signal(&wakeFlag);
    pthread_mutex_unlock(&queueLock);
    
    void *exitData;
    pthread_join(thread, &exitData);
    
    pthread_cond_destroy(&wakeFlag);
    pthread_mutex_destroy(&queueLock);
}

void GTimerThread::tick(void (*afn)(void*), void *data, double delay)
{
    Task task;
    task.fn = afn;
    task.data = data;
    
    task.time = getSysTime();
    task.time.tv_sec += floor(delay);
    task.time.tv_usec += fmod(delay, 1.f);
    
    pthread_mutex_lock(&queueLock);
    
    queue.push(task);
    pthread_cond_signal(&wakeFlag);
    pthread_mutex_unlock(&queueLock);
    
}

void GTimerThread::tickms(void (*afn)(void*), void *data, double delay)
{
    tick(afn, data, delay * .001);
}

bool GTimerThread::getNextTask(Task *task)
{
    pthread_mutex_lock(&queueLock);

    if (!serveFlag) {
        pthread_mutex_unlock(&queueLock);
        return false;
    }
    
    else {
        
        if (queue.size() < 1) {
            pthread_cond_wait(&wakeFlag, &queueLock);
            
            if (!serveFlag) {
                pthread_mutex_unlock(&queueLock);
                return false;
            }
        }
        
        while (timeValComp(getSysTime(), queue.top().time)) {
            
            timespec wakeTime;
            wakeTime.tv_sec = queue.top().time.tv_sec;
            wakeTime.tv_nsec = queue.top().time.tv_usec * 1000;

            pthread_cond_timedwait(&wakeFlag, &queueLock, &wakeTime);
            
            if (!serveFlag) {
                pthread_mutex_unlock(&queueLock);
                return false;
            }
        }
        
        *task = queue.top();
        queue.pop();
        
        pthread_mutex_unlock(&queueLock);
        return true;
    }
}

void *serveRequests(void *vtimer)
{
    GTimerThread *timer = (GTimerThread*)vtimer;
    
    GTimerThread::Task nextTask;
    while (timer->getNextTask(&nextTask)) {
        printf("Executing task for time @ time %f\n", timer->getTime());
        nextTask.fn(nextTask.data);
    }
    
    return NULL;
}

bool timeValComp(timeval left, timeval right)
{
    return (left.tv_sec < right.tv_sec ||
            (left.tv_sec == right.tv_sec &&
             left.tv_usec < right.tv_usec));
}