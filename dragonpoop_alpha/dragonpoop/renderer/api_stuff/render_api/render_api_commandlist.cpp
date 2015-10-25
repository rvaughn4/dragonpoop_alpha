
#include "render_api_commandlist.h"
#include "render_api_commandlist_readlock.h"
#include "render_api_commandlist_writelock.h"
#include "render_api_commandlist_ref.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include "render_api_context_ref.h"
#include "render_api_context_writelock.h"
#include "render_api_command.h"

namespace dragonpoop
{
    
    //ctor
    render_api_commandlist::render_api_commandlist( dpmutex_master *mm ) : shared_obj( mm )
    {
        this->bIsCompiled = 0;
    }
    
    //dtor
    render_api_commandlist::~render_api_commandlist( void )
    {
        shared_obj_guard o;
        
        o.tryWriteLock( this, 5000, "render_api_commandlist::~render_api_commandlist" );
        o.unlock();
        this->unlink();
        
        this->deleteCommands();
    }
    
    //generate read lock
    shared_obj_readlock *render_api_commandlist::render_api_commandlist::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new render_api_commandlist_readlock( (render_api_commandlist *)p, l );
    }
    
    //generate write lock
    shared_obj_writelock *render_api_commandlist::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new render_api_commandlist_writelock( (render_api_commandlist *)p, l );
    }
    
    //generate ref
    shared_obj_ref *render_api_commandlist::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new render_api_commandlist_ref( (render_api_commandlist *)p, k );
    }
    
    //returns true if ready to be executed
    bool render_api_commandlist::isReadyForExecute( void )
    {
        return this->bIsCompiled;
    }
    
    //compile command list
    void render_api_commandlist::compile( render_api_context_ref *ctx )
    {
        std::list<render_api_command *> *l;
        std::list<render_api_command *>::iterator i;
        render_api_command *p;
        render_api_context_writelock *ctxl;
        shared_obj_guard o;
        
        ctxl = (render_api_context_writelock *)o.tryWriteLock( ctx, 100, "render_api_commandlist::compile" );
        if( !ctxl )
            return;
        
        if( !this->beginCompile( ctxl ) )
            return;
        
        l = &this->cmds;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            p->compile( ctxl );
        }
        
        this->endCompile( ctxl );
        this->bIsCompiled = 1;
    }
    
    //called at begin of compile
    bool render_api_commandlist::beginCompile( render_api_context_writelock *ctx )
    {
        return 1;
    }
    
    //called at end of compile
    void render_api_commandlist::endCompile( render_api_context_writelock *ctx )
    {
        
    }
    
    //called during compile for each draw call
    void render_api_commandlist::drawCompile( render_api_context_writelock *ctx, render_api_shader_ref *sdr, render_api_texture_ref *t0, render_api_texture_ref *t1, render_api_vertexbuffer_ref *vb, render_api_indexbuffer_ref *ib, dpmatrix *m )
    {
        
    }
    
    //execute command list
    void render_api_commandlist::execute( render_api_context_writelock *r )
    {
    }
    
    //set current shader
    void render_api_commandlist::cmd_setShader( render_api_shader_ref *r )
    {
        this->sdr = r;
    }
    
    //set current texture 0
    void render_api_commandlist::cmd_setTexture0( render_api_texture_ref *r )
    {
        this->t0 = r;
    }
    
    //set current texture 1
    void render_api_commandlist::cmd_setTexture1( render_api_texture_ref *r )
    {
        this->t1 = r;
    }
    
    //set current vertex buffer
    void render_api_commandlist::cmd_setVertexBuffer( render_api_vertexbuffer_ref *r )
    {
        this->vb = r;
    }
    
    //set current index buffer
    void render_api_commandlist::cmd_setIndexBuffer( render_api_indexbuffer_ref *r )
    {
        this->ib = r;
    }
    
    //draw
    void render_api_commandlist::cmd_draw( render_api_context_writelock *ctx )
    {
        this->drawCompile( ctx, this->sdr, this->t0, this->t1, this->vb, this->ib, &this->m );
    }
    
    //set current matrix
    void render_api_commandlist::cmd_setMatrix( dpmatrix *m )
    {
        this->m.copy( m );
    }
    
    //delete commands
    void render_api_commandlist::deleteCommands( void )
    {
        std::list<render_api_command *> *l, d;
        std::list<render_api_command *>::iterator i;
        render_api_command *p;
        
        l = &this->cmds;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            d.push_back( p );
        }
        l->clear();

        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            delete p;
        }
    }
    
};
