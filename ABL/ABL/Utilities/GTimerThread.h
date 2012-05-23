//
//  GTimerThread.h
//  ABL
//
//  Created by Caleb Jordan on 5/17/12.
//  Copyright (c) 2012 Matt Rubin. All rights reserved.
//

#ifndef ABL_GTimerThread_h
#define ABL_GTimerThread_h

#include "GTimer.h"
#include <pthread.h>
#include <queue>
#include <math.h>
#include <sys/time.h>
#include <assert.h>

using namespace std;

class GTimerThread : public GTimer {
private:
    struct Task {
        void (*fn)(void *);
        void *data;
        timeval time;
        Task() :fn(NULL), data(NULL), time() {}
    };
    
    class TaskCompare {
    public:
        bool operator()(const Task& left, const Task& right) const
        {
            return (left.time.tv_sec > right.time.tv_sec ||
                    (left.time.tv_sec == right.time.tv_sec &&
                     left.time.tv_usec > right.time.tv_usec));
        }
    };
    
    pthread_t thread;
    pthread_mutex_t queueLock;
    pthread_cond_t wakeFlag;
    priority_queue< Task, vector<Task>, TaskCompare > queue;
    
    bool serveFlag;
    
public:
    GTimerThread();
    ~GTimerThread();
    
    void tick(void (*afn)(void*), void *data, double delay);
    void tickms(void (*afn)(void*), void *data, double delay);
    
    friend void *serveRequests(void *timer);

private:
    bool getNextTask(Task *task);
    
};

#endif
