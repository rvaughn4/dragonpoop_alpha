
#ifndef dragonpoop_dpsky_man_readlock_h
#define dragonpoop_dpsky_man_readlock_h

#include "../../core/shared_obj/shared_obj_readlock.h"

#include <list>

namespace dragonpoop
{
    
    class dpsky_man;
    class core;
    
    class dpsky_man_readlock : public shared_obj_readlock
    {
        
    private:
        
        dpsky_man *t;
        
    protected:
        
        //ctor
        dpsky_man_readlock( dpsky_man *t, dpmutex_readlock *l );
        //dtor
        virtual ~dpsky_man_readlock( void );
        
    public:
        
        //return core
        core *getCore( void );

        friend class dpsky_man;
    };
    
};

#endif