
#include "render_api.h"
#include "render_api_ref.h"
#include "render_api_readlock.h"
#include "render_api_writelock.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include "render_api_context.h"
#include "render_api_context_ref.h"
#include "render_api_context_writelock.h"

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
        if( this->crun < 10 )
            return;
        this->crun = 0;

        this->runContexts();
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

};
