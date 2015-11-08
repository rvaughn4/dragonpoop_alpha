
#ifndef dragonpoop_dpland_man_writelock_h
#define dragonpoop_dpland_man_writelock_h

#include "../../core/shared_obj/shared_obj_writelock.h"

#include <list>

namespace dragonpoop
{
    
    class dpland_man;
    class core;
    class dpthread_lock;
    class dpland;
    struct dpland_skydome;
    
    class dpland_man_writelock : public shared_obj_writelock
    {
        
    private:
        
        dpland_man *t;
        
    protected:
        
        //ctor
        dpland_man_writelock( dpland_man *t, dpmutex_writelock *l );
        //dtor
        virtual ~dpland_man_writelock( void );
        
    public:
        
        //return core
        core *getCore( void );
        //run
        void run( dpthread_lock *thd );
        //get tiles
        void getTiles( std::list<dpland *> *l );
        //get sky
        dpland_skydome *getSky( void );
        
        friend class dpland_man;
    };
    
};

#endif