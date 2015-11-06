
#ifndef dragonpoop_renderer_land_h
#define dragonpoop_renderer_land_h

#include "../../gfx/dpposition/dpposition.h"
#include "../../core/dpid/dpid.h"

namespace dragonpoop
{
    
    class dpland;
    class render_api_indexbuffer_ref;
    class render_api_vertexbuffer_ref;
    class render_api_context_writelock;
    class render_api_commandlist_writelock;
    class dpmatrix;
    class dpthread_lock;
    
    class renderer_land
    {
        
    private:
        
        dpposition pos;
        dpid id;
        render_api_indexbuffer_ref *ib;
        render_api_vertexbuffer_ref *vb;
        
    protected:
        
    public:
        
        //ctor
        renderer_land( dpland *l, render_api_context_writelock *ctx );
        //dtor
        virtual ~renderer_land( void );
        //get position
        void getPosition( dpposition *p );
        //returns id
        dpid getId( void );
        //render model
        void render( dpthread_lock *thd, dpposition *campos, dpmatrix *m_world, render_api_context_writelock *ctx, render_api_commandlist_writelock *clist );

    };
    
};

#endif