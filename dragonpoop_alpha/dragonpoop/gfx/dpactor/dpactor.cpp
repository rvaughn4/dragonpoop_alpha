
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
#include "../model/model_readlock.h"
#include "../model/model_instance/model_instance_ref.h"
#include "../model/model_instance/model_instance_writelock.h"
#include "../dpvertex/dpxyz_f.h"
#include "dpactor_model_state.h"
#include "dpactor_model_state_run_low.h"
#include "dpactor_animate_state.h"
#include "dpactor_animate_state_idle.h"
#include "../model/model_man_writelock.h"
#include "../model/model_man_ref.h"

#include <math.h>

namespace dragonpoop
{
    
    //ctor
    dpactor::dpactor( core *c ) : shared_obj( c->getMutexMaster() )
    {
        this->m = 0;
        this->g = 0;
        this->c = c;
        memset( &this->models, 0, sizeof( this->models ) );
        this->model_state = new dpactor_model_state_run_low( this );
        this->anim_state = new dpactor_animate_state_idle( this );
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
        if( this->anim_state )
            delete this->anim_state;
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
        if( this->m )
            delete this->m;
        if( this->g )
            delete this->g;
        o.unlock();
    }
    
    //get model man ref
    void dpactor::_getModelMan( void )
    {
        gfx_ref *g;
        gfx_readlock *gl;
        shared_obj_guard o;

        if( this->m )
            return;
        
        g = c->getGfx();
        if( !g )
            return;
        gl = (gfx_readlock *)o.readLock( g, "dpactor::dpactor" );
        if( gl )
            gl->getModels( &this->m );
        delete g;
        o.unlock();
    }

    //get gfx ref
    void dpactor::_getGfxMan( void )
    {
        if( this->g )
            return;
        this->g = this->c->getGfx();
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
        dpactor_animate_state *nas;
        
        this->_getModelMan();
        this->_getGfxMan();
        
        t = thd->getTicks();
        
        if( t - this->t_dis > 100 )
        {
            this->t_dis = t;
            this->dis = this->_getCameraDistance( t );
        }
        
        if( t - this->t_anim_state > 100 )
        {
            this->t_anim_state = t;
            if( this->anim_state )
            {
                nas = 0;
                this->anim_state->run( thd, this, &nas );
                if( nas )
                {
                    delete this->anim_state;
                    this->anim_state = nas;
                }
            }
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
        return this->_load( &this->models.low.m, &this->models.low.mi, ldr, "test_low", "", "3drt_dragon_low.dpmodel" );
    }
    
    //make low model instance
    bool dpactor::makeLow( dpthread_lock *thd )
    {
        return this->_make( &this->models.low.m, &this->models.low.mi, thd );
    }
    
    //destroy low
    void dpactor::killLow( void )
    {
        this->_kill( &this->models.low.m, &this->models.low.mi );
    }
    
    //load med model
    bool dpactor::loadMed( model_loader_ref **ldr )
    {
        return this->_load( &this->models.med.m, &this->models.med.mi, ldr, "test_med", "", "3drt_dragon_medium.dpmodel" );
    }
    
    //make med model instance
    bool dpactor::makeMed( dpthread_lock *thd )
    {
        return this->_make( &this->models.med.m, &this->models.med.mi, thd );
    }
    
    //destroy med
    void dpactor::killMed( void )
    {
        this->_kill( &this->models.med.m, &this->models.med.mi );
    }
    
    //load high model
    bool dpactor::loadHigh( model_loader_ref **ldr )
    {
        return this->_load( &this->models.high.m, &this->models.high.mi, ldr, "test_high", "", "3drt_dragon_high.dpmodel" );
    }
    
    //make high model instance
    bool dpactor::makeHigh( dpthread_lock *thd )
    {
        return this->_make( &this->models.high.m, &this->models.high.mi, thd );
    }
    
    //destroy high
    void dpactor::killHigh( void )
    {
        this->_kill( &this->models.high.m, &this->models.high.mi );
    }
    
    //load model
    bool dpactor::_load( model_ref **m, model_instance_ref **mi, model_loader_ref **ldr, const char *mname, const char *path_name, const char *file_name )
    {
        shared_obj_guard o;
        model_man_writelock *ml;
        
        *ldr = 0;
        if( !this->m )
            return 0;
        ml = (model_man_writelock *)o.tryWriteLock( this->m, 3000, "dpactor::load" );
        if( !ml )
            return 0;
        
        if( *m )
            delete *m;
        *m = ml->findModel( mname );
        if( *m )
            return 1;
        
        if( ml->loadModel( mname, path_name, file_name, m, ldr ) )
            return 1;
        
        if( *m )
            delete *m;
        *m = 0;
        return 0;
    }
    
    //make model instance
    bool dpactor::_make( model_ref **m, model_instance_ref **mi, dpthread_lock *thd )
    {
        shared_obj_guard o, o1;
        model_writelock *l;
        model_instance_writelock *il;
        
        if( *mi )
            delete *mi;
        *mi = 0;
        
        if( !*m )
            return 0;
        l = (model_writelock *)o.tryWriteLock( *m, 1000, "dpactor::make" );
        if( !l )
            return 0;
        
        *mi = l->makeInstance( thd, thd->genId() );
        if( !*mi )
            return 0;
        
        il = (model_instance_writelock *)o1.tryWriteLock( *mi, 1000, "dpactor::make" );
        if( !il )
        {
            if( *mi )
                delete *mi;
            *mi = 0;
            return 0;
        }
        il->setPosition( &this->pos );
        
        if( this->models.low.m && this->models.low.mi && *mi != this->models.low.mi )
            il->cloneAnimations( this->models.low.mi );
        if( this->models.med.m && this->models.med.mi && *mi != this->models.med.mi )
            il->cloneAnimations( this->models.med.mi );
        if( this->models.high.m && this->models.high.mi && *mi != this->models.high.mi )
            il->cloneAnimations( this->models.high.mi );
        
        il->sync();
        l->sync();
        
        return 1;
    }
    
    //destroy model instance
    void dpactor::_kill( model_ref **m, model_instance_ref **mi )
    {
        shared_obj_guard o;
        model_writelock *ml;
        model_instance_writelock *mil;
        
        if( *mi )
        {
            mil = (model_instance_writelock *)o.tryWriteLock( *mi, 2000, "dpactor::_kill" );
            if( mil )
            {
                mil->kill();
                mil->sync();
            }
        }
        if( *m )
        {
            ml = (model_writelock *)o.tryWriteLock( *m, 2000, "dpactor::_kill" );
            if( ml )
            {
                ml->decRefCount();
                ml->sync();
            }
        }
  
        if( *mi )
            delete *mi;
        *mi = 0;
        if( *m )
            delete *m;
        *m = 0;
    }
    
    //stop all animations
    void dpactor::stopAnimations( void )
    {
        if( this->models.low.mi )
            this->_stopAnimations( this->models.low.mi );
        if( this->models.med.mi )
            this->_stopAnimations( this->models.med.mi );
        if( this->models.high.mi )
            this->_stopAnimations( this->models.high.mi );
    }
    
    //convert animation name
    bool dpactor::convertName( std::string *sin, std::string *sout )
    {
        if( sin->compare( "idle" ) == 0 )
            sout->assign( "idle-ground" );
        if( sin->compare( "walk" ) == 0 )
            sout->assign( "walk-1" );
        if( sin->compare( "fly_start" ) == 0 )
            sout->assign( "fly start" );
        if( sin->compare( "fly_stop" ) == 0 )
            sout->assign( "fly end" );
        if( sin->compare( "fly_idle" ) == 0 )
            sout->assign( "flying idle" );
        if( sin->compare( "fly" ) == 0 )
            sout->assign( "fly_forward" );
        return 1;
    }
    
    //play animation
    void dpactor::playAnimation( const char *cname, float spd, bool doRepeat )
    {
        std::string sname, sres;
        
        sname.assign( cname );
        this->convertName( &sname, &sres );
        
        if( this->models.low.m && this->models.low.mi )
            this->_playAnimation( this->models.low.m, this->models.low.mi, sres.c_str(), spd, doRepeat );
        if( this->models.med.m && this->models.med.mi )
            this->_playAnimation( this->models.med.m, this->models.med.mi, sres.c_str(), spd, doRepeat );
        if( this->models.high.m && this->models.high.mi )
            this->_playAnimation( this->models.high.m, this->models.high.mi, sres.c_str(), spd, doRepeat );
    }
    
    //returns true if animation is playing
    bool dpactor::isPlaying( const char *cname )
    {
        std::string sname, sres;
        
        sname.assign( cname );
        this->convertName( &sname, &sres );
        
        if( this->models.low.m && this->models.low.mi )
            return this->_isPlaying( this->models.low.mi, sres.c_str() );
        if( this->models.med.m && this->models.med.mi )
            return this->_isPlaying( this->models.med.mi, sres.c_str() );
        if( this->models.high.m && this->models.high.mi )
            return this->_isPlaying( this->models.high.mi, sres.c_str() );
        return 0;
    }
    
    //stop all animations
    void dpactor::_stopAnimations( model_instance_ref *mi )
    {
        shared_obj_guard o;
        model_instance_writelock *mil;

        mil = (model_instance_writelock *)o.tryWriteLock( mi, 2000, "dpactor::_kill" );
        if( !mil )
            return;
        
        mil->stopAllAnimations();
    }
    
    //play animation
    void dpactor::_playAnimation( model_ref *m, model_instance_ref *mi, const char *cname, float spd, bool doRepeat )
    {
        shared_obj_guard o, o1;
        model_readlock *ml;
        model_instance_writelock *mil;
        
        ml = (model_readlock *)o.tryReadLock( m, 2000, "dpactor::_kill" );
        if( !ml )
            return;
        
        mil = (model_instance_writelock *)o1.tryWriteLock( mi, 2000, "dpactor::_kill" );
        if( !mil )
            return;
        
        mil->playAnimation( ml->getId(), ml, cname, doRepeat, spd );
    }
  
    //returns true if animation is playing
    bool dpactor::_isPlaying( model_instance_ref *mi, const char *cname )
    {
        shared_obj_guard o;
        model_instance_writelock *mil;
        
        mil = (model_instance_writelock *)o.tryWriteLock( mi, 2000, "dpactor::_kill" );
        if( !mil )
            return 0;
        
        return mil->isAnimationPlaying( cname );
    }
    
};
