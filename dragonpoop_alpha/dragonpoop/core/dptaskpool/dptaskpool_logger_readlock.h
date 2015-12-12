
#ifndef dragonpoop_dptaskpool_logger_readlock_h
#define dragonpoop_dptaskpool_logger_readlock_h

#include "../../core/shared_obj/shared_obj_readlock.h"

namespace dragonpoop
{
    class dptaskpool_logger;
    class dpposition;
    struct dptaskpool_logger_value;

    class dptaskpool_logger_readlock : public shared_obj_readlock
    {

    private:

        dptaskpool_logger *t;

    protected:

        //ctor
        dptaskpool_logger_readlock( dptaskpool_logger *t, dpmutex_readlock *l );
        //dtor
        virtual ~dptaskpool_logger_readlock( void );

    public:

        //get position time
        uint64_t getTime( void );
        //get task list
        unsigned int getTaskList( dptaskpool_logger_value **p );

        friend class dptaskpool_logger;
    };

};

#endif
