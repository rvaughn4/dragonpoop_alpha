
#include "dpactor.h"
#include "dpactor_ref.h"
#include "dpactor_readlock.h"
#include "dpactor_writelock.h"
#include "../../core/core.h"
#include "../../core/dpthread/dpthread_lock.h"
#include "../gfx_ref.h"
#include "../gfx_readlock.h"
#include "../gfx_writelock.h"
#include "../../core/shared_obj/shared_obj_guard.h"
#include "../model/model_ref.h"
#include "../model/model_writelock.h"
#include "../model/model_instance/model_instance_ref.h"
#include "../model/model_instance/model_instance_writelock.h"
#include "../dpvertex/dpxyz_f.h"
#include "dpactor_model_state.h"
#include "dpactor_model_state_run_low.h"

#include <math.h>

namespace dragonpoop
{
    
    //ctor
    dpactor::dpactor( core *c ) : shared_obj( c->getMutexMaster() )
    {
        this->c = c;
        this->g = 0;
        memset( &this->models, 0, sizeof( this->models ) );
        this->model_state = new dpactor_model_state_run_low( this );
    }
    
    //dtor
    dpactor::~dpactor( void )
    {
        shared_obj_guard o;
        
        o.tryWriteLock( this, 5000, "dpactor::~dpactor" );
        o.unlock();
        this->unlink();
        
        o.tryWriteLock( this, 5000, "dpactor::~dpactor" );
        if( this->model_state )
            delete this->model_state;
        if( this->models.low.m )
            delete this->models.low.m;
        if( this->models.low.mi )
            delete this->models.low.mi;
        if( this->models.med.m )
            delete this->models.med.m;
        if( this->models.med.mi )
            delete this->models.med.mi;
        if( this->models.high.m )
            delete this->models.high.m;
        if( this->models.high.mi )
            delete this->models.high.mi;
        if( this->g )
            delete this->g;
        o.unlock();
    }
    
    //return core
    core *dpactor::getCore( void )
    {
        return this->c;
    }
    
    //generate read lock
    shared_obj_readlock *dpactor::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new dpactor_readlock( (dpactor *)p, l );
    }
    
    //generate write lock
    shared_obj_writelock *dpactor::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new dpactor_writelock( (dpactor *)p, l );
    }
    
    //generate ref
    shared_obj_ref *dpactor::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new dpactor_ref( (dpactor *)p, k );
    }
    
    //run gui
    void dpactor::run( dpthread_lock *thd, dpactor_writelock *g )
    {
        uint64_t t;
        dpactor_model_state *nms;
        
        if( !this->g )
            this->g = this->c->getGfx();
        
        t = thd->getTicks();
        
        if( t - this->t_dis > 1000 )
        {
            this->t_dis = t;
            this->dis = this->_getCameraDistance( t );
        }
        
        if( t - this->t_anim_state > 500 )
        {
            this->t_anim_state = t;
            //
        }
        
        if( t - this->t_model_state > 2000 )
        {
            this->t_model_state = t;
            if( this->model_state )
            {
                nms = 0;
                this->model_state->run( thd, this, &nms );
                if( nms )
                {
                    delete this->model_state;
                    this->model_state = nms;
                }
            }
        }
    }
    
    //get position
    void dpactor::getPosition( dpposition *p )
    {
        p->copy( &this->pos );
    }
    
    //set position
    void dpactor::setPosition( dpposition *p )
    {
        shared_obj_guard o;
        model_instance_writelock *l;
        
        this->pos.copy( p );
        
        if( this->models.low.mi )
        {
            l = (model_instance_writelock *)o.tryWriteLock( this->models.low.mi, 3000, "dpactor::setPosition" );
            if( l )
                l->setPosition( &this->pos );
        }
        if( this->models.med.mi )
        {
            l = (model_instance_writelock *)o.tryWriteLock( this->models.med.mi, 3000, "dpactor::setPosition" );
            if( l )
                l->setPosition( &this->pos );
        }
        if( this->models.high.mi )
        {
            l = (model_instance_writelock *)o.tryWriteLock( this->models.high.mi, 3000, "dpactor::setPosition" );
            if( l )
                l->setPosition( &this->pos );
        }
    }
    
    //returns distance from camera
    float dpactor::getCameraDistance( void )
    {
        return this->dis;
    }

    //returns distance from camera
    float dpactor::_getCameraDistance( uint64_t t )
    {
        dpposition p;
        shared_obj_guard o;
        gfx_readlock *l;
        dpxyz_f x;
        float d;
        
        if( !this->g )
            return 0;
        l = (gfx_readlock *)o.tryReadLock( this->g, 1000, "dpactor::_getCameraDistance" );
        if( !l )
            return 0;
        l->getCameraPosition( &p );
        p.getDifference( &this->pos, t, &x );
        
        d = x.x * x.x + x.y * x.y + x.z * x.z;
        if( d > 0 )
            d = sqrtf( d );
        else
            d = 0;
        
        return d;
    }
    
    //load low model
    bool dpactor::loadLow( model_loader_ref **ldr )
    {
        gfx_writelock *gl;
        shared_obj_guard o;
        
        *ldr = 0;
        if( !this->g )
            return 0;
        gl = (gfx_writelock *)o.tryWriteLock( this->g, 3000, "dpactor::loadLow" );
        
        if( this->models.low.m )
            delete this->models.low.m;
        this->models.low.m = gl->findModel( "test_low" );
        if( this->models.low.m )
            return 1;
        
        if( gl->loadModel( "test_low", "", "3drt_dragon_low.dpmodel", &this->models.low.m, ldr ) )
            return 1;
        
        if( this->models.low.m )
            delete this->models.low.m;
        this->models.low.m = 0;
        return 0;
    }
    
    //make low model instance
    bool dpactor::makeLow( dpthread_lock *thd )
    {
        shared_obj_guard o;
        model_writelock *l;
        model_instance_writelock *il;
        
        if( this->models.low.mi )
            delete this->models.low.mi;
        this->models.low.mi = 0;
        
        if( !this->models.low.m )
            return 0;
        l = (model_writelock *)o.tryWriteLock( this->models.low.m, 1000, "dpactor::makeLow" );
        if( !l )
            return 0;
        
        this->models.low.mi = l->makeInstance( thd, thd->genId() );
        if( !this->models.low.mi )
            return 0;

        il = (model_instance_writelock *)o.tryWriteLock( this->models.low.mi, 1000, "dpactor::makeLow" );
        if( !il )
        {
            if( this->models.low.mi )
                delete this->models.low.mi;
            this->models.low.mi = 0;
            return 0;
        }
        il->setPosition( &this->pos );
        
        return 1;
    }
 
};
