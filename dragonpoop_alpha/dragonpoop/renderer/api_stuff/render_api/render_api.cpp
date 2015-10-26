
#include "render_api.h"
#include "render_api_ref.h"
#include "render_api_readlock.h"
#include "render_api_writelock.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include "render_api_context.h"
#include "render_api_context_ref.h"
#include "render_api_context_writelock.h"
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
    render_api::render_api( window *w, dpmutex_master *mm ) : shared_obj( mm )
    {
        this->w = w;
        this->mm = mm;
    }
    
    //dtor
    render_api::~render_api( void )
    {
        shared_obj_guard o;
        
        o.tryWriteLock( this, 5000, "render_api_context::~render_api_context" );
        o.unlock();
        this->unlink();

        this->deleteIndexBuffers();
        this->deleteShaders();
        this->deleteTextures();
        this->deleteVertexBuffers();
        this->deleteContexts();
        delete this->w;
    }
    
    //generate read lock
    shared_obj_readlock *render_api::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new render_api_readlock( (render_api *)p, l );
    }
    
    //generate write lock
    shared_obj_writelock *render_api::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new render_api_writelock( (render_api *)p, l );
    }
    
    //generate ref
    shared_obj_ref *render_api::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new render_api_ref( (render_api *)p, k );
    }
    
    //run api
    void render_api::run( void )
    {
        this->w->run();
        
        this->crun++;
        if( this->crun < 20 )
            return;
        this->crun = 0;

        this->runContexts();
        this->runShaders();
        this->runTextures();
    }
    
    //returns true if window is open
    bool render_api::isOpen( void )
    {
        return this->w->isOpen();
    }
    
    //returns window width
    float render_api::getWidth( void )
    {
        return this->w->getWidth();
    }
    
    //returns window height
    float render_api::getHeight( void )
    {
        return this->w->getHeight();
    }

    //returns window
    window *render_api::getWindow( void )
    {
        return this->w;
    }
    
    //make context
    render_api_context_ref *render_api::getContext( render_api_writelock *al )
    {
        render_api_context *c;
        render_api_context_writelock *l;
        shared_obj_guard o;
        
        c = this->genContext( al, this->mm );
        if( !c )
            return 0;
        this->contexts.push_back( c );
        
        l = (render_api_context_writelock *)o.tryWriteLock( c, 1000, "render_api::getContext" );
        if( !l )
            return 0;
        
        return (render_api_context_ref *)l->getRef();
    }
    
    //generate context
    render_api_context *render_api::genContext( render_api_writelock *al, dpmutex_master *mm )
    {
        return new render_api_context( al, mm );
    }
    
    //delete contexts
    void render_api::deleteContexts( void )
    {
        std::list<render_api_context *> *l, d;
        std::list<render_api_context *>::iterator i;
        render_api_context *p;
        
        l = &this->contexts;
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
    
    //run contexts
    void render_api::runContexts( void )
    {
        std::list<render_api_context *> *l, d;
        std::list<render_api_context *>::iterator i;
        render_api_context *p;
        render_api_context_writelock *pl;
        shared_obj_guard o;
        
        l = &this->contexts;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            if( !p->isLinked() )
                d.push_back( p );
            pl = (render_api_context_writelock *)o.tryWriteLock( p, 100, "render_api::runContexts" );
            if( !pl )
                continue;
            pl->run();
        }
        
        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            this->contexts.remove( p );
            delete p;
        }
    }
    
    //run shaders
    void render_api::runShaders( void )
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
    void render_api::runTextures( void )
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
    
    //gen shader
    render_api_shader *render_api::genShader( dpmutex_master *mm, render_api_writelock *al, render_api_context_writelock *cl, const char *cname )
    {
        return 0;
    }
    
    //gen texture
    render_api_texture *render_api::genTexture( dpmutex_master *mm, render_api_writelock *al, render_api_context_writelock *cl, dpbitmap *bm )
    {
        return 0;
    }
    
    //gen vertex buffer
    render_api_vertexbuffer *render_api::genVertexBuffer( dpmutex_master *mm, render_api_writelock *al, render_api_context_writelock *cl, dpvertex_buffer *vb )
    {
        return 0;
    }
    
    //gen index buffer
    render_api_indexbuffer *render_api::genIndexBuffer( dpmutex_master *mm, render_api_writelock *al, render_api_context_writelock *cl, dpindex_buffer *vb )
    {
        return 0;
    }
    
    //make shader
    render_api_shader_ref *render_api::makeShader( render_api_writelock *al, render_api_context_writelock *cl, const char *cname )
    {
        shared_obj_guard o;
        render_api_shader_writelock *l;
        render_api_shader *c;
        
        c = this->genShader( this->mm, al, cl, cname );
        if( !c )
            return 0;
        this->shaders.push_back( c );
        
        l = (render_api_shader_writelock *)o.tryWriteLock( c, 1000, "render_api::makeShader" );
        if( !l )
            return 0;
        
        return (render_api_shader_ref *)l->getRef();
    }
    
    //make texture
    render_api_texture_ref *render_api::makeTexture( render_api_writelock *al, render_api_context_writelock *cl, dpbitmap *bm )
    {
        shared_obj_guard o;
        render_api_texture_writelock *l;
        render_api_texture *c;
        
        c = this->genTexture( this->mm, al, cl, bm );
        if( !c )
            return 0;
        this->textures.push_back( c );
        
        l = (render_api_texture_writelock *)o.tryWriteLock( c, 1000, "render_api::makeTexture" );
        if( !l )
            return 0;
        
        return (render_api_texture_ref *)l->getRef();
    }
    
    //make vertex buffer
    render_api_vertexbuffer_ref *render_api::makeVertexBuffer( render_api_writelock *al, render_api_context_writelock *cl, dpvertex_buffer *vb )
    {
        shared_obj_guard o;
        render_api_vertexbuffer_writelock *l;
        render_api_vertexbuffer *c;
        
        c = this->genVertexBuffer( this->mm, al, cl, vb );
        if( !c )
            return 0;
        this->vertexbuffers.push_back( c );
        
        l = (render_api_vertexbuffer_writelock *)o.tryWriteLock( c, 1000, "render_api::makeVertexBuffer" );
        if( !l )
            return 0;
        
        return (render_api_vertexbuffer_ref *)l->getRef();
    }
    
    //make index buffer
    render_api_indexbuffer_ref *render_api::makeIndexBuffer( render_api_writelock *al, render_api_context_writelock *cl, dpindex_buffer *vb )
    {
        shared_obj_guard o;
        render_api_indexbuffer_writelock *l;
        render_api_indexbuffer *c;
        
        c = this->genIndexBuffer( this->mm, al, cl, vb );
        if( !c )
            return 0;
        this->indexbuffers.push_back( c );
        
        l = (render_api_indexbuffer_writelock *)o.tryWriteLock( c, 1000, "render_api::makeIndexBuffer" );
        if( !l )
            return 0;
        
        return (render_api_indexbuffer_ref *)l->getRef();
    }
    
    //delete shaders
    void render_api::deleteShaders( void )
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
    void render_api::deleteTextures( void )
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
    void render_api::deleteVertexBuffers( void )
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
    void render_api::deleteIndexBuffers( void )
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
    
    //returns true if mouse input is waiting
    bool render_api::hasMouseInput( void )
    {
        return this->w->hasMouseInput();
    }
    
    //fetches mouse input
    bool render_api::getMouseInput( window_mouse_input *m )
    {
        return this->w->getMouseInput( m );
    }
    
    //adds mouse input
    void render_api::addMouseInput( window_mouse_input *m )
    {
        this->w->addMouseInput( m );
    }
    
    //returns true if kb input is waiting
    bool render_api::hasKBInput( void )
    {
        return this->w->hasKBInput();
    }
    
    //fetches kb input
    bool render_api::getKBInput( window_kb_input *m )
    {
        return this->w->getKBInput( m );
    }
    
    //adds kb input
    void render_api::addKBInput( window_kb_input *m )
    {
        this->w->addKBInput( m );
    }
    
};
