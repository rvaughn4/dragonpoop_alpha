
#include "renderer_commandlist_passer.h"
#include "renderer_commandlist_passer_ref.h"
#include "renderer_commandlist_passer_writelock.h"
#include "api_stuff/render_api/render_api_commandlist_ref.h"
#include "api_stuff/render_api/render_api_commandlist_writelock.h"
#include "../core/shared_obj/shared_obj_guard.h"

namespace dragonpoop
{
    
    //ctor
    renderer_commandlist_passer::renderer_commandlist_passer( dpmutex_master *mm ) : shared_obj( mm )
    {
        this->rgui = 0;
        this->rmodel = 0;
        this->rland = 0;
        this->land_ready = 0;
        this->model_ready = 0;
        this->gui_ready = 0;
    }
    
    //dtor
    renderer_commandlist_passer::~renderer_commandlist_passer( void )
    {
        render_api_commandlist_ref *r;
        shared_obj_guard o;
        
        o.tryWriteLock( this, 5000, "renderer_commandlist_passer::~renderer_commandlist_passer" );
        o.unlock();
        this->unlink();
        
        o.tryWriteLock( this, 5000, "renderer_commandlist_passer::~renderer_commandlist_passer" );
        r = this->rgui;
        if( r )
            delete r;
        r = this->rmodel;
        if( r )
            delete r;
        r = this->rland;
        if( r )
            delete r;
        o.unlock();
    }
    
    //generate write lock
    shared_obj_writelock *renderer_commandlist_passer::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new renderer_commandlist_passer_writelock( (renderer_commandlist_passer *)p, l );
    }
    
    //generate ref
    shared_obj_ref *renderer_commandlist_passer::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new renderer_commandlist_passer_ref( (renderer_commandlist_passer *)p, k );
    }
    
    
    //set camera position
    void renderer_commandlist_passer::setPosition( dpposition *p )
    {
        this->pos.copy( p );
    }
    
    //get camera position
    dpposition *renderer_commandlist_passer::getPosition( void )
    {
        return &this->pos;
    }
    
    //set model commandlist
    void renderer_commandlist_passer::setModel( render_api_commandlist_ref *r )
    {
        render_api_commandlist_writelock *l;
        shared_obj_guard o;
        
        l = (render_api_commandlist_writelock *)o.tryWriteLock( r, 100, "renderer_commandlist_passer::setModel" );
        if( !l )
            return;
        
        r = this->rmodel;
        if( r )
            delete r;
        this->rmodel = 0;
        
        r = (render_api_commandlist_ref *)l->getRef();
        this->rmodel = r;
    }
    
    //get model commandlist
    render_api_commandlist_ref *renderer_commandlist_passer::getModel( void )
    {
        render_api_commandlist_writelock *l;
        shared_obj_guard o;
        render_api_commandlist_ref *r;
        
        r = this->rmodel;
        l = (render_api_commandlist_writelock *)o.tryWriteLock( r, 100, "renderer_commandlist_passer::setGui" );
        if( !l )
            return 0;
        delete r;
        this->rmodel = 0;
        
        r = (render_api_commandlist_ref *)l->getRef();
        return r;
    }
    
    //set gui commandlist
    void renderer_commandlist_passer::setGui( render_api_commandlist_ref *r )
    {
        render_api_commandlist_writelock *l;
        shared_obj_guard o;
        
        l = (render_api_commandlist_writelock *)o.tryWriteLock( r, 100, "renderer_commandlist_passer::setGui" );
        if( !l )
            return;
        
        r = this->rgui;
        if( r )
            delete r;
        this->rgui = 0;
        
        r = (render_api_commandlist_ref *)l->getRef();
        this->rgui = r;
    }
    
    //get gui commandlist
    render_api_commandlist_ref *renderer_commandlist_passer::getGui( void )
    {
        render_api_commandlist_writelock *l;
        shared_obj_guard o;
        render_api_commandlist_ref *r;
        
        r = this->rgui;
        l = (render_api_commandlist_writelock *)o.tryWriteLock( r, 100, "renderer_commandlist_passer::setGui" );
        if( !l )
            return 0;
        delete r;
        this->rgui = 0;
        
        r = (render_api_commandlist_ref *)l->getRef();
        return r;
    }
    
    //set land commandlist
    void renderer_commandlist_passer::setLand( render_api_commandlist_ref *r )
    {
        render_api_commandlist_writelock *l;
        shared_obj_guard o;
        
        l = (render_api_commandlist_writelock *)o.tryWriteLock( r, 100, "renderer_commandlist_passer::setLand" );
        if( !l )
            return;
        
        r = this->rland;
        if( r )
            delete r;
        this->rland = 0;
        
        r = (render_api_commandlist_ref *)l->getRef();
        this->rland = r;
    }
    
    //get land commandlist
    render_api_commandlist_ref *renderer_commandlist_passer::getLand( void )
    {
        render_api_commandlist_writelock *l;
        shared_obj_guard o;
        render_api_commandlist_ref *r;
        
        r = this->rland;
        l = (render_api_commandlist_writelock *)o.tryWriteLock( r, 100, "renderer_commandlist_passer::getLand" );
        if( !l )
            return 0;
        delete r;
        this->rland = 0;
        
        r = (render_api_commandlist_ref *)l->getRef();
        return r;
    }
    
};
