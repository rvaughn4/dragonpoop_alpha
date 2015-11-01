
#include "render_api_commandlist_generic_batch.h"
#include "render_api_texture_ref.h"
#include "render_api_shader_ref.h"
#include "render_api_vertexbuffer_ref.h"
#include "render_api_indexbuffer_ref.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include "../../../core/shared_obj/shared_obj_writelock.h"

namespace dragonpoop
{
    
    //ctor
    render_api_commandlist_generic_batch::render_api_commandlist_generic_batch( render_api_commandlist_generic *l, render_api_shader_ref *sdr, render_api_texture_ref *t0, render_api_texture_ref *t1, render_api_vertexbuffer_ref *vb, render_api_indexbuffer_ref *ib, dpmatrix *m, float alpha )
    {
        shared_obj_guard o;
        shared_obj_writelock *wl;
        
        wl = o.tryWriteLock( sdr, 5000, "render_api_commandlist_generic_batch::render_api_commandlist_generic_batch" );
        if( wl )
            this->sdr = (render_api_shader_ref *)wl->getRef();
        else
            this->sdr = 0;
        
        wl = o.tryWriteLock( t0, 5000, "render_api_commandlist_generic_batch::render_api_commandlist_generic_batch" );
        if( wl )
            this->t0 = (render_api_texture_ref *)wl->getRef();
        else
            this->t0 = 0;
        
        wl = o.tryWriteLock( t1, 5000, "render_api_commandlist_generic_batch::render_api_commandlist_generic_batch" );
        if( wl )
            this->t1 = (render_api_texture_ref *)wl->getRef();
        else
            this->t1 = 0;
        
        wl = o.tryWriteLock( vb, 5000, "render_api_commandlist_generic_batch::render_api_commandlist_generic_batch" );
        if( wl )
            this->vb = (render_api_vertexbuffer_ref *)wl->getRef();
        else
            this->vb = 0;
        
        wl = o.tryWriteLock( ib, 5000, "render_api_commandlist_generic_batch::render_api_commandlist_generic_batch" );
        if( wl )
            this->ib = (render_api_indexbuffer_ref *)wl->getRef();
        else
            this->ib = 0;
        
        this->m.copy( m );
        this->alpha = alpha;
        this->l = l;
    }
    
    //dtor
    render_api_commandlist_generic_batch::~render_api_commandlist_generic_batch( void )
    {
        delete this->sdr;
        delete this->t0;
        delete this->t1;
        delete this->vb;
        delete this->ib;
    }
    
    //execute batch
    void render_api_commandlist_generic_batch::execute( render_api_context_writelock *ctx )
    {
        this->l->executeBatch( ctx, this->sdr, this->t0, this->t1, this->vb, this->ib, &this->m, this->alpha );
    }
    
    
};
