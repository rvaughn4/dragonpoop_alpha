
#ifndef dragonpoop_dptask_readlock_h
#define dragonpoop_dptask_readlock_h

#include "../shared_obj/shared_obj_readlock.h"
#include <string>

namespace dragonpoop
{
    class dptask;

    class dptask_readlock : public shared_obj_readlock
    {

    private:

        dptask *t;

    protected:

        //ctor
        dptask_readlock( dptask *t, dpmutex_readlock *l );
        //dtor
        virtual ~dptask_readlock( void );

    public:

        //returns true if task is alive
        bool isAlive( void );
        //get task sleep delay
        uint64_t getDelay( void );
        //returns true if task should run on one and the same thread only
        bool isSingleThread( void );
        //returns last ran time
        uint64_t getLastTime( void );
        //get name
        void getName( std::string *s );
        //returns percent usage
        unsigned int getUsage( void );
        //returns true if task can share thread with static task
        bool canShare( void );

        friend class dptask;
    };

};

#endif
