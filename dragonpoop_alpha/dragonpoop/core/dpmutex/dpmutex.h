
#ifndef dragonpoop_dpmutex_h
#define dragonpoop_dpmutex_h

#include <stdint.h>
#include <atomic>
#include <thread>
#include <vector>

namespace dragonpoop
{

    class dpmutex_master;
    class dpmutex_readlock;
    class dpmutex_writelock;
    class dpspinlock;

    struct dpmutex_thread
    {
        std::thread::id tid;
        int cnt;
    };

    class dpmutex
    {

    private:

        dpmutex_master *m;
        std::vector<dpmutex_thread> rids, wids;

    protected:

        //ctor
        dpmutex( dpmutex_master *m );
        //dtor
        virtual ~dpmutex( void );
        //add thread id to readlock list
        void addReadThread( std::thread::id tid );
        //add thread id to writelock list
        void addWriteThread( std::thread::id tid );
        //remove thread id from readlock list
        void removeReadThread( std::thread::id tid );
        //remove thread id from writelock list
        void removeWriteThread( std::thread::id tid );
        //return count of threads holding readlock that are not this thread
        int countReadThreads( std::thread::id tid );
        //return count of threads holding writelock that are not this thread
        int countWriteThreads( std::thread::id tid );
        //find thread with id
        dpmutex_thread *findReadThread( std::thread::id tid );
        //find thread with id
        dpmutex_thread *findWriteThread( std::thread::id tid );

    public:

        //try read lock
        dpmutex_readlock *tryReadLock( void );
        //try write lock
        dpmutex_writelock *tryWriteLock( void );
        //try read lock until timeout in ms
        dpmutex_readlock *tryReadLock( uint64_t w );
        //try write lock until timeout in ms
        dpmutex_writelock *tryWriteLock( uint64_t w );
        //do not return until read locked
        dpmutex_readlock *readLock( void );
        //do not return until write locked
        dpmutex_writelock *writeLock( void );

        friend class dpmutex_master;
        friend class dpmutex_lock;
    };
    
};

#endif