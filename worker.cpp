#include "worker.h"


    /* each thread will be assigned a portion of the
     * individual array to simulate */
Worker::Worker(int id, boost::mutex* lock, int begin, int end, Population *pop) :
    _id(id), _lock(lock), _begin(begin), _end(end), _pop(pop) {}

Worker::~Worker()  {
}

void Worker::operator()() {
    for (int i = _begin; i < _end; i++) {
            /* Current thread will simulate child strings from begin
             * to end */
        Individual *ind = _pop->ind[i];
        ind->simulate(_id);
        _lock->lock(); //acquire lock

        cout << "Thread " << _id << " is simulating child at position " << i << "\n";
        _lock->unlock();  //release lock
    }
    
}
