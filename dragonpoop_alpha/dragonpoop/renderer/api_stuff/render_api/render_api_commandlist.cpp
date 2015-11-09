
#include "render_api_commandlist.h"
#include "render_api_commandlist_readlock.h"
#include "render_api_commandlist_writelock.h"
#include "render_api_commandlist_ref.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include "render_api_context_ref.h"
#include "render_api_context_writelock.h"
#include "render_api_command.h"
#include "render_api_command_draw.h"
#include "render_api_command_set_indexbuffer.h"
#include "render_api_command_set_shader.h"
#include "render_api_command_set_alpha.h"
#include "render_api_command_set_matrix.h"
#include "render_api_command_set_texture.h"
#include "render_api_command_set_vertexbuffer.h"

namespace dragonpoop
{
    
    //ctor
    render_api_commandlist::render_api_commandlist( dpmutex_master *mm ) : shared_obj( mm )
    {
        this->bIsCompiled = 0;
        this->sdr = 0;
        this->t0 = 0;
        this->t1 = 0;
        this->vb = 0;
        this->ib = 0;
        this->alpha = 1;
        this->r = 1;
        this->g = 1;
        this->b = 0;
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
    
    //compile command list
    bool render_api_commandlist::compile( render_api_context_writelock *ctxl )
    {
        std::list<render_api_command *> *l;
        std::list<render_api_command *>::iterator i;
        render_api_command *p;
        
        if( !this->beginCompile( ctxl ) )
            return 0;
        
        l = &this->cmds;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            p->compile( ctxl );
        }
        
        this->endCompile( ctxl );
        return 1;
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
    bool render_api_commandlist::drawCompile( render_api_context_writelock *ctx, render_api_shader_ref *sdr, render_api_texture_ref *t0, render_api_texture_ref *t1, render_api_vertexbuffer_ref *vb, render_api_indexbuffer_ref *ib, dpmatrix *m, float alpha, float r, float g, float b )
    {
        return 1;
    }
    
    //execute command list
    bool render_api_commandlist::execute( render_api_context_writelock *r, dpmatrix *m_world )
    {
        return 1;
    }
    
    //set current shader
    bool render_api_commandlist::cmd_setShader( render_api_shader_ref *r )
    {
        this->sdr = r;
        return 1;
    }
    
    //set current texture 0
    bool render_api_commandlist::cmd_setTexture( render_api_texture_ref *r, int index )
    {
        switch( index )
        {
            case 0:
                this->t0 = r;
                break;
            case 1:
                this->t1 = r;
                break;
        }
        return 1;
    }
    
    //set current vertex buffer
    bool render_api_commandlist::cmd_setVertexBuffer( render_api_vertexbuffer_ref *r )
    {
        this->vb = r;
        return 1;
    }
    
    //set current index buffer
    bool render_api_commandlist::cmd_setIndexBuffer( render_api_indexbuffer_ref *r )
    {
        this->ib = r;
        return 1;
    }
    
    //set current alpha
    bool render_api_commandlist::cmd_setAlpha( float a )
    {
        this->alpha = a;
        return 1;
    }
    
    //set current color
    bool render_api_commandlist::cmd_setColor( float r, float g, float b )
    {
        this->r = r;
        this->g = g;
        this->b = b;
        return 1;
    }
    
    //draw
    bool render_api_commandlist::cmd_draw( render_api_context_writelock *ctx )
    {
        return this->drawCompile( ctx, this->sdr, this->t0, this->t1, this->vb, this->ib, &this->m, this->alpha, this->r, this->g, this->b );
    }
    
    //set current matrix
    bool render_api_commandlist::cmd_setMatrix( dpmatrix *m )
    {
        this->m.copy( m );
        return 1;
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
    
    //set shader command
    void render_api_commandlist::setShader( render_api_shader_ref *p )
    {
        render_api_command *c;
        c = new render_api_command_set_shader( this, p );
        this->cmds.push_back( c );
    }
    
    //set texture command
    void render_api_commandlist::setTexture( render_api_texture_ref *p, int level )
    {
        render_api_command *c;
        c = new render_api_command_set_texture( this, p, level );
        this->cmds.push_back( c );
    }
    
    //set vertex buffer command
    void render_api_commandlist::setVertexBuffer( render_api_vertexbuffer_ref *p )
    {
        render_api_command *c;
        c = new render_api_command_set_vertexbuffer( this, p );
        this->cmds.push_back( c );
    }
    
    //set index buffer command
    void render_api_commandlist::setIndexBuffer( render_api_indexbuffer_ref *p )
    {
        render_api_command *c;
        c = new render_api_command_set_indexbuffer( this, p );
        this->cmds.push_back( c );
    }
    
    //set matrix command
    void render_api_commandlist::setMatrix( dpmatrix *m )
    {
        render_api_command *c;
        c = new render_api_command_set_matrix( this, m );
        this->cmds.push_back( c );
    }
    
    //set alpha command
    void render_api_commandlist::setAlpha( float a )
    {
        render_api_command *c;
        c = new render_api_command_set_alpha( this, a );
        this->cmds.push_back( c );
    }
    
    //set color command
    void render_api_commandlist::setColor( float r, float g, float b )
    {
        
    }
    
    //draw command
    void render_api_commandlist::draw( void )
    {
        render_api_command *c;
        c = new render_api_command_draw( this );
        this->cmds.push_back( c );
    }
    
    //set position
    void render_api_commandlist::setPosition( dpposition *p )
    {
        this->pos.copy( p );
    }
    
    //get position
    void render_api_commandlist::getPosition( dpposition *p )
    {
        p->copy( &this->pos );
    }
 
};
