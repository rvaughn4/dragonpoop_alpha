
#ifndef dragonpoop_dpactor_readlock_h
#define dragonpoop_dpactor_readlock_h

#include "../../core/shared_obj/shared_obj_readlock.h"

namespace dragonpoop
{

    class core;
    class dpactor;
    class dpposition;
    
    class dpactor_readlock : public shared_obj_readlock
    {
        
    private:
        
        dpactor *t;
        
    protected:
        
        //ctor
        dpactor_readlock( dpactor *t, dpmutex_readlock *l );
        //dtor
        virtual ~dpactor_readlock( void );
        
    public:
        
        //return core
        core *getCore( void );
        //get position
        void getPosition( dpposition *p );
        
        friend class dpactor;
    };
    
};

#endif