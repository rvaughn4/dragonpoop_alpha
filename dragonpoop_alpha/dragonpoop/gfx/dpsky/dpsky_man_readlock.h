
#ifndef dragonpoop_dpsky_man_readlock_h
#define dragonpoop_dpsky_man_readlock_h

#include "../../core/shared_obj/shared_obj_readlock.h"

#include <list>

namespace dragonpoop
{
    
    class dpsky_man;
    class core;
    struct dpsky_stuff;
    
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
        //return sky stuff
        dpsky_stuff *getSky( void );

        friend class dpsky_man;
    };
    
};

#endif