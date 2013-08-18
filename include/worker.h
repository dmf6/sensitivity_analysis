#ifndef __WORKER_H
#define __WORKER_H

#include "boost/thread.hpp"
#include "population.h"

/* even though Worker is treated as an inner nested class it does not
 * belong ot any class therefore we pass a reference to the instance
 * of the outer population
 */
class Population;

/*Runnable interface should be used if you are only planning to
 * override the run() method and no other Thread methods */
class Worker {
  private:
    int _id;
    boost::mutex* _lock;
    int _begin;
    int _end;
    Population *_pop;
    
  public:
    Worker(int id, boost::mutex* mutex, int begin, int end, Population *pop);
    ~Worker();
    
        /* When an object implementing interface Runnable is used
         * to create a thread, starting the thread causes the
         * object's run method to be called in that separately
         * executing thread. */
    void operator()();
};

#endif
