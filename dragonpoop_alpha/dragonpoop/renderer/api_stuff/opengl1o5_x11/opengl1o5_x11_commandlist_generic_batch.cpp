
#include "opengl1o5_x11_commandlist_generic_batch.h"
#include "../render_api/render_api_texture_ref.h"
#include "../render_api/render_api_shader_ref.h"
#include "../render_api/render_api_vertexbuffer_ref.h"
#include "../render_api/render_api_indexbuffer_ref.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include "../../../core/shared_obj/shared_obj_writelock.h"
#include "opengl1o5_x11_shader_readlock.h"
#include "opengl1o5_x11_texture_readlock.h"
#include "opengl1o5_x11_vertexbuffer_readlock.h"
#include "opengl1o5_x11_indexbuffer_readlock.h"

namespace dragonpoop
{
    
    //ctor
    opengl1o5_x11_commandlist_generic_batch::opengl1o5_x11_commandlist_generic_batch( opengl1o5_x11_commandlist_generic *l, render_api_shader_ref *sdr, render_api_texture_ref *t0, render_api_texture_ref *t1, render_api_vertexbuffer_ref *vb, render_api_indexbuffer_ref *ib, dpmatrix *m, float alpha ) : render_api_commandlist_generic_batch( l, sdr, t0, t1, vb, ib, m, alpha )
    {
        shared_obj_guard o;
        shared_obj_writelock *wl;
        shared_obj_readlock *rl;
        
        wl = o.tryWriteLock( sdr, 5000, "opengl1o5_x11_commandlist_generic_batch::opengl1o5_x11_commandlist_generic_batch" );
        if( wl )
            this->sdr = (render_api_shader_ref *)wl->getRef();
        else
            this->sdr = 0;
        
        rl = o.tryReadLock( t0, 5000, "opengl1o5_x11_commandlist_generic_batch::opengl1o5_x11_commandlist_generic_batch" );
        if( rl )
            this->t0 = ( (opengl1o5_x11_texture_readlock *)rl )->getTex();
        else
            this->t0 = 0;
        
        rl = o.tryReadLock( t1, 5000, "opengl1o5_x11_commandlist_generic_batch::opengl1o5_x11_commandlist_generic_batch" );
        if( rl )
            this->t1 = ( (opengl1o5_x11_texture_readlock *)rl )->getTex();
        else
            this->t1 = 0;
        
        rl = o.tryReadLock( vb, 5000, "opengl1o5_x11_commandlist_generic_batch::opengl1o5_x11_commandlist_generic_batch" );
        if( rl )
            this->vb.copy( ( (opengl1o5_x11_vertexbuffer_readlock *)rl )->getBuffer() );
        
        
        rl = o.tryReadLock( ib, 5000, "opengl1o5_x11_commandlist_generic_batch::opengl1o5_x11_commandlist_generic_batch" );
        if( rl )
            this->ib.copy( ( (opengl1o5_x11_indexbuffer_readlock *)rl )->getBuffer() );

        this->m.copy( m );
        this->alpha = alpha;
        this->l = l;
    }
    
    //dtor
    opengl1o5_x11_commandlist_generic_batch::~opengl1o5_x11_commandlist_generic_batch( void )
    {
        delete this->sdr;
    }
    
    //execute batch
    bool opengl1o5_x11_commandlist_generic_batch::execute( render_api_context_writelock *ctx, dpmatrix *m_world )
    {
        shared_obj_guard o;
        opengl1o5_x11_shader_readlock *l;
        
        l = (opengl1o5_x11_shader_readlock *)o.tryReadLock( sdr, 10, "opengl1o5_x11_commandlist_generic_batch::execute" );
        if( !l )
            return 0;
        
        return l->_render( ctx, t0, t1, &ib, &vb, &m, alpha, m_world );
    }
    
    
};
