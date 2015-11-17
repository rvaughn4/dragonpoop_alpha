
#ifndef dragonpoop_dptaskpool_task_h
#define dragonpoop_dptaskpool_task_h

#include "../dptask/dptask_owner.h"
#include "dptaskpool.h"
#include <atomic>

namespace dragonpoop
{

    class dptaskpool_ref;

    class dptaskpool_task : public dptask_owner
    {

    private:

        std::atomic<dptaskpool_ref *> g;

    protected:

    public:

        //ctor
        dptaskpool_task( dptaskpool *g );
        //dtor
        virtual ~dptaskpool_task( void );
        //run by task
        virtual void run( dptask_writelock *tl, dpthread_lock *th );

    };

};

#endif
