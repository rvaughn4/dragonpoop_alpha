
#ifndef dragonpoop_dptaskpool_h
#define dragonpoop_dptaskpool_h

#include "../shared_obj/shared_obj.h"

namespace dragonpoop
{

    class dptask_ref;
    class dpthread;
    class dptask;

    class dptaskpool : public shared_obj
    {

    private:

        //threads
        struct
        {
            unsigned int cnt, max, acnt;
            dpthread **buffer;
        } threads;
        //tasks
        struct
        {
            unsigned int cnt, max;
            dptask_ref **buffer;
        } tasks;
        dpmutex_master *mm;


    protected:

        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //add task, will create ref so caller retains original
        void addTask( dptask *t );
        //add task, will create ref so caller retains original
        void addTask( dptask_ref *t );
        //adjust thread count
        void setThreadCount( unsigned int c );
        //return thread count
        unsigned int getThreadCount( void );
        //push task into pool
        void pushTask( dptask_ref *t );
        //pop task from pool
        dptask_ref *popTask( void );
        //push thread into pool
        void pushThread( dpthread *t );
        //pop thread from pool
        dpthread *popThread( void );

    public:

        //ctor
        dptaskpool( dpmutex_master *mm, unsigned int thread_cnt );
        //dtor
        virtual ~dptaskpool( void );


        friend class dptaskpool_readlock;
        friend class dptaskpool_writelock;
        friend class dpthread;
    };
    
};

#endif