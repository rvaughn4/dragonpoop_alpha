
#ifndef dragonpoop_renderer_model_man_readlock_h
#define dragonpoop_renderer_model_man_readlock_h

#include "../../core/shared_obj/shared_obj_readlock.h"
#include "../../core/dpid/dpid.h"
#include <list>

namespace dragonpoop
{
    
    class renderer_model_man;
    class core;
    class dpthread_lock;
    class renderer_model_ref;
    class renderer_model;
    class renderer_writelock;
    class dpmatrix;
    
    class renderer_model_man_readlock : public shared_obj_readlock
    {
        
    private:
        
        renderer_model_man *t;
        
    protected:
        
        //ctor
        renderer_model_man_readlock( renderer_model_man *t, dpmutex_readlock *l );
        //dtor
        virtual ~renderer_model_man_readlock( void );
        
    public:
        
        //return core
        core *getCore( void );

        friend class renderer_model_man;
    };
    
};

#endif