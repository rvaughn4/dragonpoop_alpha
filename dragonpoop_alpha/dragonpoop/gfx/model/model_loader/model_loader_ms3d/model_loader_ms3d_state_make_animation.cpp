
#include "model_loader_ms3d_state_make_animation.h"
//#include "model_loader_ms3d_state_make_frames.h"
#include "../../../../core/dpbuffer/dpbuffer.h"
#include "model_loader_ms3d_animation.h"
#include "model_loader_ms3d_state_cleanup.h"
#include "model_loader_ms3d.h"
#include "../model_loader_writelock.h"
#include "../../../../core/shared_obj/shared_obj_guard.h"
#include "../../model_animation/model_animation.h"
#include "../../model_ref.h"
#include "../../model_writelock.h"
#include "../../../../core/dpthread/dpthread_lock.h"

namespace dragonpoop
{
    
    //ctor
    model_loader_ms3d_state_make_animation::model_loader_ms3d_state_make_animation( dpbuffer *b, model_ref *m )
    {
        this->b = b;
        this->m = m;
    }
    
    //dtor
    model_loader_ms3d_state_make_animation::~model_loader_ms3d_state_make_animation( void )
    {
        
    }
    
    //run state, returns next state
    model_loader_state *model_loader_ms3d_state_make_animation::run( dpthread_lock *thd, model_loader_writelock *ml )
    {
        shared_obj_guard o;
        model_writelock *m;
        model_animation *ma;
        model_loader_ms3d *t;
        
        m = (model_writelock *)o.writeLock( this->m );
        if( !m )
            return new model_loader_ms3d_state_cleanup( this->b, this->m, 0 );
        
        t = (model_loader_ms3d *)ml->getLoader();
        
        ma = m->makeAnimation( thd->genId() );
        if( !ma )
            return new model_loader_ms3d_state_cleanup( this->b, this->m, 0 );
        ma->setAutoPlay( 1 );
        ma->setRepeatDelay( 0 );
        ma->setRepeated( 1 );
        t->anim_id = m->getId();
        
        //return new model_loader_ms3d_state_make_frames( this->b, this->m );
        return new model_loader_ms3d_state_cleanup( this->b, this->m, 1 );
    }
    
};
