
#include "render_api_context.h"
#include "render_api_ref.h"
#include "render_api_writelock.h"
#include "render_api_readlock.h"
#include "render_api_context_ref.h"
#include "render_api_context_readlock.h"
#include "render_api_context_writelock.h"
#include "render_api_commandlist.h"
#include "render_api_commandlist_ref.h"
#include "render_api_commandlist_writelock.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include "render_api_shader.h"
#include "render_api_shader_ref.h"
#include "render_api_shader_writelock.h"
#include "render_api_texture.h"
#include "render_api_texture_ref.h"
#include "render_api_texture_writelock.h"
#include "render_api_vertexbuffer.h"
#include "render_api_vertexbuffer_ref.h"
#include "render_api_vertexbuffer_writelock.h"
#include "render_api_indexbuffer.h"
#include "render_api_indexbuffer_ref.h"
#include "render_api_indexbuffer_writelock.h"

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
        
        this->deleteIndexBuffers();
        this->deleteShaders();
        this->deleteTextures();
        this->deleteVertexBuffers();
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
    
    //run command lists
    void render_api_context::runLists( void )
    {
        std::list<render_api_commandlist *> *l, d;
        std::list<render_api_commandlist *>::iterator i;
        render_api_commandlist *p;
        
        l = &this->cmds;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            if( !p->isLinked() )
                d.push_back( p );
        }
        
        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            this->cmds.remove( p );
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
    
    //run context, delete old lists
    void render_api_context::run( void )
    {
        this->runLists();
        this->runShaders();
        this->runTextures();
        this->runIndexBuffers();
        this->runVertexBuffers();
    }
    
    //return width and height
    void render_api_context::getDimensions( float *w, float *h )
    {
        shared_obj_guard o;
        render_api_readlock *l;
        
        l = (render_api_readlock *)o.tryReadLock( this->r, 1000, "render_api_context::getDimensions" );
        if( !l )
            return;
        
        *w = l->getWidth();
        *h = l->getHeight();
    }
    
    //run shaders
    void render_api_context::runShaders( void )
    {
        std::list<render_api_shader *> *l, d;
        std::list<render_api_shader *>::iterator i;
        render_api_shader *p;
        
        l = &this->shaders;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            if( !p->isLinked() )
                d.push_back( p );
        }
        
        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            this->shaders.remove( p );
            delete p;
        }
    }
    
    //run textures
    void render_api_context::runTextures( void )
    {
        std::list<render_api_texture *> *l, d;
        std::list<render_api_texture *>::iterator i;
        render_api_texture *p;
        
        l = &this->textures;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            if( !p->isLinked() )
                d.push_back( p );
        }
        
        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            this->textures.remove( p );
            delete p;
        }
    }
    
    //run vertex buffers
    void render_api_context::runVertexBuffers( void )
    {
        std::list<render_api_vertexbuffer *> *l, d;
        std::list<render_api_vertexbuffer *>::iterator i;
        render_api_vertexbuffer *p;
        
        l = &this->vertexbuffers;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            if( !p->isLinked() )
                d.push_back( p );
        }
        
        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            this->vertexbuffers.remove( p );
            delete p;
        }
    }
    
    //run index buffers
    void render_api_context::runIndexBuffers( void )
    {
        std::list<render_api_indexbuffer *> *l, d;
        std::list<render_api_indexbuffer *>::iterator i;
        render_api_indexbuffer *p;
        
        l = &this->indexbuffers;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            if( !p->isLinked() )
                d.push_back( p );
        }
        
        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            this->indexbuffers.remove( p );
            delete p;
        }
    }
    
    //gen shader
    render_api_shader *render_api_context::genShader( dpmutex_master *mm, render_api_context_writelock *cl, const char *cname )
    {
        return 0;
    }
    
    //gen texture
    render_api_texture *render_api_context::genTexture( dpmutex_master *mm, render_api_context_writelock *cl, dpbitmap *bm )
    {
        return 0;
    }
    
    //gen vertex buffer
    render_api_vertexbuffer *render_api_context::genVertexBuffer( dpmutex_master *mm, render_api_context_writelock *cl, dpvertex_buffer *vb )
    {
        return 0;
    }
    
    //gen index buffer
    render_api_indexbuffer *render_api_context::genIndexBuffer( dpmutex_master *mm, render_api_context_writelock *cl, dpindex_buffer *vb )
    {
        return 0;
    }
    
    //make shader
    render_api_shader_ref *render_api_context::makeShader( render_api_context_writelock *cl, const char *cname )
    {
        shared_obj_guard o;
        render_api_shader_writelock *l;
        render_api_shader *c;
        
        c = this->genShader( this->mm, cl, cname );
        if( !c )
            return 0;
        this->shaders.push_back( c );
        
        l = (render_api_shader_writelock *)o.tryWriteLock( c, 1000, "render_api_context::makeShader" );
        if( !l )
            return 0;
        
        return (render_api_shader_ref *)l->getRef();
    }
    
    //make texture
    render_api_texture_ref *render_api_context::makeTexture( render_api_context_writelock *cl, dpbitmap *bm )
    {
        shared_obj_guard o;
        render_api_texture_writelock *l;
        render_api_texture *c;
        
        c = this->genTexture( this->mm, cl, bm );
        if( !c )
            return 0;
        this->textures.push_back( c );
        
        l = (render_api_texture_writelock *)o.tryWriteLock( c, 1000, "render_api_context::makeTexture" );
        if( !l )
            return 0;
        
        return (render_api_texture_ref *)l->getRef();
    }
    
    //make vertex buffer
    render_api_vertexbuffer_ref *render_api_context::makeVertexBuffer( render_api_context_writelock *cl, dpvertex_buffer *vb )
    {
        shared_obj_guard o;
        render_api_vertexbuffer_writelock *l;
        render_api_vertexbuffer *c;
        
        c = this->genVertexBuffer( this->mm, cl, vb );
        if( !c )
            return 0;
        this->vertexbuffers.push_back( c );
        
        l = (render_api_vertexbuffer_writelock *)o.tryWriteLock( c, 1000, "render_api_context::makeVertexBuffer" );
        if( !l )
            return 0;
        
        return (render_api_vertexbuffer_ref *)l->getRef();
    }
    
    //make index buffer
    render_api_indexbuffer_ref *render_api_context::makeIndexBuffer( render_api_context_writelock *cl, dpindex_buffer *vb )
    {
        shared_obj_guard o;
        render_api_indexbuffer_writelock *l;
        render_api_indexbuffer *c;
        
        c = this->genIndexBuffer( this->mm, cl, vb );
        if( !c )
            return 0;
        this->indexbuffers.push_back( c );
        
        l = (render_api_indexbuffer_writelock *)o.tryWriteLock( c, 1000, "render_api_context::makeIndexBuffer" );
        if( !l )
            return 0;
        
        return (render_api_indexbuffer_ref *)l->getRef();
    }
    
    //delete shaders
    void render_api_context::deleteShaders( void )
    {
        std::list<render_api_shader *> *l, d;
        std::list<render_api_shader *>::iterator i;
        render_api_shader *p;
        
        l = &this->shaders;
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
    
    //delete textures
    void render_api_context::deleteTextures( void )
    {
        std::list<render_api_texture *> *l, d;
        std::list<render_api_texture *>::iterator i;
        render_api_texture *p;
        
        l = &this->textures;
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
    
    //delete vertex buffers
    void render_api_context::deleteVertexBuffers( void )
    {
        std::list<render_api_vertexbuffer *> *l, d;
        std::list<render_api_vertexbuffer *>::iterator i;
        render_api_vertexbuffer *p;
        
        l = &this->vertexbuffers;
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
    
    //delete index buffers
    void render_api_context::deleteIndexBuffers( void )
    {
        std::list<render_api_indexbuffer *> *l, d;
        std::list<render_api_indexbuffer *>::iterator i;
        render_api_indexbuffer *p;
        
        l = &this->indexbuffers;
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
