
#ifndef dragonpoop_dptaskpool_logger_h
#define dragonpoop_dptaskpool_logger_h

#include "../../core/shared_obj/shared_obj.h"
#include <atomic>
#include <memory>

namespace dragonpoop
{

    class dptaskpool_logger_ref;
    class dptask;
    class dptask_ref;
    class dptask_readlock;

#define dptaskpool_logger_value_name_max 100
#define dptaskpool_logger_value_max 200

    struct dptaskpool_logger_value
    {
        unsigned int tid;
        bool isStatic;
        char name[ dptaskpool_logger_value_name_max ];
    };

    class dptaskpool_logger : public shared_obj
    {

    private:

        dptaskpool_logger_value values[ dptaskpool_logger_value_max ];
        unsigned int lastTask;
        std::shared_ptr<std::atomic<uint64_t>> tm;

    protected:

        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //clear
        void clear( void );
        //sync
        void sync( dptaskpool_logger_ref *r );
        //add task
        void addTask( unsigned int tid, dptask *t );
        //add task
        void addTask( unsigned int tid, dptask_ref *t );
        //add task
        void addTask( unsigned int tid, dptask_readlock *t );
        //get task list
        unsigned int getTaskList( dptaskpool_logger_value **p );
        //get time
        uint64_t getTime( void );
        //set time
        void setTime( uint64_t t );

    public:

        //ctor
        dptaskpool_logger( dpmutex_master *mm );
        //dtor
        virtual ~dptaskpool_logger( void );


        friend class dptaskpool_logger_writelock;
        friend class dptaskpool_logger_readlock;
        friend class dptaskpool_logger_ref;

    };

};

#endif
