
#ifndef dragonpoop_renderer_model_man_writelock_h
#define dragonpoop_renderer_model_man_writelock_h

#include "../../core/shared_obj/shared_obj_writelock.h"
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
    class dpposition;
    class render_api_context_writelock;
    class render_api_commandlist_writelock;
    
    class renderer_model_man_writelock : public shared_obj_writelock
    {
        
    private:
        
        renderer_model_man *t;
        
    protected:
        
        //ctor
        renderer_model_man_writelock( renderer_model_man *t, dpmutex_writelock *l );
        //dtor
        virtual ~renderer_model_man_writelock( void );
        
    public:
        
        //return core
        core *getCore( void );
        //delete models
        void deleteModels( void );
        //render models
        void renderModels( dpthread_lock *thd, dpposition *campos, dpmatrix *m_world, render_api_context_writelock *ctx, render_api_commandlist_writelock *clist );

        friend class renderer_model_man;
    };
    
};

#endif