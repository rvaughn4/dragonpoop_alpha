
#ifndef dragonpoop_dptaskpool_logger_writelock_h
#define dragonpoop_dptaskpool_logger_writelock_h

#include "../../core/shared_obj/shared_obj_writelock.h"

namespace dragonpoop
{
    class dptaskpool_logger;
    class dpposition;
    struct dptaskpool_logger_value;
    class dptask;
    class dptask_ref;
    class dptask_readlock;
    class dptaskpool_logger_ref;

    class dptaskpool_logger_writelock : public shared_obj_writelock
    {

    private:

        dptaskpool_logger *t;

    protected:

        //ctor
        dptaskpool_logger_writelock( dptaskpool_logger *t, dpmutex_writelock *l );
        //dtor
        virtual ~dptaskpool_logger_writelock( void );

    public:

        //get position time
        uint64_t getTime( void );
        //get task list
        unsigned int getTaskList( dptaskpool_logger_value **p );
        //add task
        void addTask( unsigned int tid, dptask *t );
        //add task
        void addTask( unsigned int tid, dptask_ref *t );
        //add task
        void addTask( unsigned int tid, dptask_readlock *t );
        //clear
        void clear( void );
        //sync
        void sync( dptaskpool_logger_ref *r );
        //set time
        void setTime( uint64_t t );

        friend class dptaskpool_logger;
    };

};

#endif
