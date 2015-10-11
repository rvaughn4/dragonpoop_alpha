
#ifndef dragonpoop_dpactor_writelock_h
#define dragonpoop_dpactor_writelock_h

#include "../../core/shared_obj/shared_obj_writelock.h"

namespace dragonpoop
{
    
    class core;
    class dpactor;
    class dpposition;
    
    class dpactor_writelock : public shared_obj_writelock
    {
        
    private:
        
        dpactor *t;
        
    protected:
        
        //ctor
        dpactor_writelock( dpactor *t, dpmutex_writelock *l );
        //dtor
        virtual ~dpactor_writelock( void );
        
    public:
        
        //return core
        core *getCore( void );
        //get position
        void getPosition( dpposition *p );
        //set position
        void setPosition( dpposition *p );
        
        friend class dpactor;
    };
    
};

#endif