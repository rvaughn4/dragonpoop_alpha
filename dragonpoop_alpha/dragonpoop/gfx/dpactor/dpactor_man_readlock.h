
#ifndef dragonpoop_dpactor_man_readlock_h
#define dragonpoop_dpactor_man_readlock_h

#include "../../core/shared_obj/shared_obj_readlock.h"

namespace dragonpoop
{
    
    class dpactor_man;
    class core;
    
    class dpactor_man_readlock : public shared_obj_readlock
    {
        
    private:
        
        dpactor_man *t;
        
    protected:
        
        //ctor
        dpactor_man_readlock( dpactor_man *t, dpmutex_readlock *l );
        //dtor
        virtual ~dpactor_man_readlock( void );
        
    public:
        
        //return core
        core *getCore( void );
        
        friend class dpactor_man;
    };
    
};

#endif