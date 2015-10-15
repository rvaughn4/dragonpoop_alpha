
#ifndef dragonpoop_dpland_man_readlock_h
#define dragonpoop_dpland_man_readlock_h

#include "../../core/shared_obj/shared_obj_readlock.h"

namespace dragonpoop
{

    class dpland_man;
    class core;
    
    class dpland_man_readlock : public shared_obj_readlock
    {
        
    private:
        
        dpland_man *t;
        
    protected:
        
        //ctor
        dpland_man_readlock( dpland_man *t, dpmutex_readlock *l );
        //dtor
        virtual ~dpland_man_readlock( void );
        
    public:
        
        //return core
        core *getCore( void );
        
        friend class dpland_man;
    };
    
};

#endif