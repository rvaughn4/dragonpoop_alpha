
#include "model_loader_ms3d_state_make_joints.h"
#include "model_loader_ms3d_state_make_animation.h"
#include "../../../../core/dpbuffer/dpbuffer.h"
#include "../model_loader_writelock.h"
#include "model_loader_ms3d.h"
#include "../../model_ref.h"
#include "../../model_writelock.h"
#include "../../../../core/shared_obj/shared_obj_guard.h"
#include "model_loader_ms3d_state_cleanup.h"
#include "../../model_joint/model_joint.h"
#include "../../../../core/dpthread/dpthread_lock.h"

#include <iostream>

namespace dragonpoop
{
    
    //ctor
    model_loader_ms3d_state_make_joints::model_loader_ms3d_state_make_joints( dpbuffer *b, model_ref *m )
    {
        this->b = b;
        this->m = m;
    }
    
    //dtor
    model_loader_ms3d_state_make_joints::~model_loader_ms3d_state_make_joints( void )
    {
        
    }
    
    //run state, returns next state
    model_loader_state *model_loader_ms3d_state_make_joints::run( dpthread_lock *thd, model_loader_writelock *ml )
    {
        shared_obj_guard o;
        model_writelock *m;
        unsigned int i, e;
        model_loader_ms3d *t;
        ms3d_model_joint_m *v;
        std::vector<ms3d_model_joint_m> *l;
        
        m = (model_writelock *)o.writeLock( this->m );
        if( !m )
            return new model_loader_ms3d_state_cleanup( this->b, this->m, 0 );
        
        t = (model_loader_ms3d *)ml->getLoader();
        
        l = t->joints;
        e = (unsigned int)l->size();
        for( i = 0; i < e; i++ )
        {
            v = &( ( *l )[ i ] );
            this->makejoint( thd, ml, v, m );
        }
        o.unlock();
        
        return new model_loader_ms3d_state_make_animation( this->b, this->m );
    }
    
    
    //make triangle
    void model_loader_ms3d_state_make_joints::makejoint( dpthread_lock *thd, model_loader_writelock *ml, ms3d_model_joint_m *t, model_writelock *m )
    {
        model_joint *mv;
        std::string s;
        dpxyzw x;
        
        mv = m->makeJoint( thd->genId() );
        if( !mv )
            return;
        t->id = mv->getId();
        s.assign( (char *)t->f.name, sizeof( t->f.name ) );
        mv->setName( &s );
        
        x.x = t->f.pos.x;
        x.y = t->f.pos.y;
        x.z = t->f.pos.z;
        x.w = 1;
        mv->setPosition( &x );

        x.x = t->f.rot.x;
        x.y = t->f.rot.y;
        x.z = t->f.rot.z;
        x.w = 1;
        mv->setRotation( &x );
    }
    
};
