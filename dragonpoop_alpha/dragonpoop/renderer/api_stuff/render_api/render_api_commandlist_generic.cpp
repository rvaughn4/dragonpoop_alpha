
#include "render_api_commandlist_generic.h"
#include "render_api_shader_readlock.h"
#include "render_api_shader_ref.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include "render_api_commandlist_generic_batch.h"

namespace dragonpoop
{
    
    //ctor
    render_api_commandlist_generic::render_api_commandlist_generic( dpmutex_master *mm ) : render_api_commandlist( mm )
    {
        
    }
    
    //dtor
    render_api_commandlist_generic::~render_api_commandlist_generic( void )
    {
        this->deleteBatches();
    }
    
    //called at begin of compile
    bool render_api_commandlist_generic::beginCompile( render_api_context_writelock *ctx )
    {
        this->deleteBatches();
        return 1;
    }
    
    //called at end of compile
    void render_api_commandlist_generic::endCompile( render_api_context_writelock *ctx )
    {
        
    }
    
    //called during compile for each draw call
    bool render_api_commandlist_generic::drawCompile( render_api_context_writelock *ctx, render_api_shader_ref *sdr, render_api_texture_ref *t0, render_api_texture_ref *t1, render_api_vertexbuffer_ref *vb, render_api_indexbuffer_ref *ib, dpmatrix *m, float alpha )
    {
        render_api_commandlist_generic_batch *b;
        b = this->genBatch( ctx, sdr, t0, t1, vb, ib, m, alpha );
        this->cmds.push_back( b );
        return 1;
    }
    
    //execute command list
    bool render_api_commandlist_generic::execute( render_api_context_writelock *r )
    {
        std::list<render_api_commandlist_generic_batch *> *l;
        std::list<render_api_commandlist_generic_batch *>::iterator i;
        render_api_commandlist_generic_batch *p;
        
        l = &this->cmds;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            if( !p->execute( r ) )
                return 0;
        }
        
        return 1;
    }
    
    //execute batch
    bool render_api_commandlist_generic::executeBatch( render_api_context_writelock *ctx, render_api_shader_ref *sdr, render_api_texture_ref *t0, render_api_texture_ref *t1, render_api_vertexbuffer_ref *vb, render_api_indexbuffer_ref *ib, dpmatrix *m, float alpha )
    {
        shared_obj_guard o;
        render_api_shader_readlock *l;
        
        l = (render_api_shader_readlock *)o.tryReadLock( sdr, 100, "render_api_commandlist_generic::executeBatch" );
        if( !l )
            return 0;
        
        l->render( ctx, t0, t1, ib, vb, m, alpha );
        return 1;
    }
    
    //delete batches
    void render_api_commandlist_generic::deleteBatches( void )
    {
        std::list<render_api_commandlist_generic_batch *> *l;
        std::list<render_api_commandlist_generic_batch *>::iterator i;
        render_api_commandlist_generic_batch *p;
        
        l = &this->cmds;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            delete p;
        }
        l->clear();
    }
    
    //generate batch
    render_api_commandlist_generic_batch *render_api_commandlist_generic::genBatch( render_api_context_writelock *ctx, render_api_shader_ref *sdr, render_api_texture_ref *t0, render_api_texture_ref *t1, render_api_vertexbuffer_ref *vb, render_api_indexbuffer_ref *ib, dpmatrix *m, float alpha )
    {
        return new render_api_commandlist_generic_batch( this, sdr, t0, t1, vb, ib, m, alpha );
    }
    
};
