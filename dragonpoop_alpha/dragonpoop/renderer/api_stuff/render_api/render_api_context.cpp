
#include "render_api_context.h"
#include "render_api_ref.h"
#include "render_api_writelock.h"
#include "render_api_context_ref.h"
#include "render_api_context_readlock.h"
#include "render_api_context_writelock.h"
#include "render_api_commandlist.h"
#include "render_api_commandlist_ref.h"
#include "render_api_commandlist_writelock.h"
#include "../../../core/shared_obj/shared_obj_guard.h"

namespace dragonpoop
{

    //ctor
    render_api_context::render_api_context( render_api_writelock *r, dpmutex_master *mm ) : shared_obj( mm )
    {
        this->r = (render_api_ref *)r->getRef();
        this->mm = mm;
    }
    
    //dtor
    render_api_context::~render_api_context( void )
    {
        shared_obj_guard o;

        o.tryWriteLock( this, 5000, "render_api_context::~render_api_context" );
        o.unlock();
        this->unlink();
        
        this->deleteCmdLists();
        delete this->r;
    }
    
    //generate read lock
    shared_obj_readlock *render_api_context::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new render_api_context_readlock( (render_api_context *)p, l );
    }
    
    //generate write lock
    shared_obj_writelock *render_api_context::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new render_api_context_writelock( (render_api_context *)p, l );
    }
    
    //generate ref
    shared_obj_ref *render_api_context::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new render_api_context_ref( (render_api_context *)p, k );
    }

    //generate commandlist
    render_api_commandlist *render_api_context::genCmdList( render_api_context_writelock *cl, dpmutex_master *mm )
    {
        return new render_api_commandlist( mm );
    }
    
    //make commandlist
    render_api_commandlist_ref *render_api_context::makeCmdList( render_api_context_writelock *cl )
    {
        shared_obj_guard o;
        render_api_commandlist *c;
        render_api_commandlist_writelock *l;
        
        
        c = this->genCmdList( cl, this->mm );
        if( !c )
            return 0;
        this->cmds.push_back( c );
        
        l = (render_api_commandlist_writelock *)o.tryWriteLock( c, 1000, "render_api_context::makeCmdList" );
        if( !l )
            return 0;
        
        return (render_api_commandlist_ref *)l->getRef();
    }
    
    //delete commandlists
    void render_api_context::deleteCmdLists( void )
    {
        std::list<render_api_commandlist *> *l, d;
        std::list<render_api_commandlist *>::iterator i;
        render_api_commandlist *p;
        
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
    
    //clear screen
    void render_api_context::clearColor( float r, float g, float b )
    {
        
    }
    
    //clear depth
    void render_api_context::clearDepth( float f )
    {
        
    }
    
    //present backbuffer
    void render_api_context::flipBackBuffer( void )
    {
        
    }
    
    //make context active in thread
    void render_api_context::makeActive( void )
    {
        
    }
    
    //set viewport
    void render_api_context::setViewport( float w, float h )
    {
        
    }
    
    //make a texture
    render_api_texture_ref *render_api_context::makeTexture( render_api_context_writelock *cl, dpbitmap *bm )
    {
        shared_obj_guard o;
        render_api_writelock *l;
        
        l = (render_api_writelock *)o.tryWriteLock( this->r, 1000, "render_api_context::makeTexture" );
        if( !l )
            return 0;
        
        return l->makeTexture( cl, bm );
    }
    
    //make a shader
    render_api_shader_ref *render_api_context::makeShader( render_api_context_writelock *cl, const char *cname )
    {
        shared_obj_guard o;
        render_api_writelock *l;
        
        l = (render_api_writelock *)o.tryWriteLock( this->r, 1000, "render_api_context::makeTexture" );
        if( !l )
            return 0;
        
        return l->makeShader( cl, cname );
    }
    
    //make a vertex buffer
    render_api_vertexbuffer_ref *render_api_context::makeVertexBuffer( render_api_context_writelock *cl, dpvertex_buffer *vb )
    {
        shared_obj_guard o;
        render_api_writelock *l;
        
        l = (render_api_writelock *)o.tryWriteLock( this->r, 1000, "render_api_context::makeTexture" );
        if( !l )
            return 0;
        
        return l->makeVertexBuffer( cl, vb );
    }
    
    //make a index buffer
    render_api_indexbuffer_ref *render_api_context::makeIndexBuffer( render_api_context_writelock *cl, dpindex_buffer *ib )
    {
        shared_obj_guard o;
        render_api_writelock *l;
        
        l = (render_api_writelock *)o.tryWriteLock( this->r, 1000, "render_api_context::makeTexture" );
        if( !l )
            return 0;
        
        return l->makeIndexBuffer( cl, ib );
    }
    
};
