
#include "model_saver_ms3d_state_make_joints.h"
#include "../../../../core/dpbuffer/dpbuffer.h"
#include "model_saver_ms3d_joint.h"
#include "model_saver_ms3d_state_cleanup.h"
#include "model_saver_ms3d.h"
#include "../model_saver_writelock.h"
#include "../../../../core/shared_obj/shared_obj_guard.h"
#include "../../model_joint/model_joint.h"
#include "../../model_ref.h"
#include "../../model_readlock.h"
#include "../../../../core/dpthread/dpthread_lock.h"
#include "../../model_frame_joint/model_frame_joint.h"

namespace dragonpoop
{
    
    //ctor
    model_saver_ms3d_state_make_joints::model_saver_ms3d_state_make_joints( dpbuffer *b, model_ref *m )
    {
        this->b = b;
        this->m = m;
    }
    
    //dtor
    model_saver_ms3d_state_make_joints::~model_saver_ms3d_state_make_joints( void )
    {
        
    }
    
    //run state, returns next state
    model_saver_state *model_saver_ms3d_state_make_joints::run( dpthread_lock *thd, model_saver_writelock *ml )
    {
        shared_obj_guard o;
        model_readlock *m;
        model_saver_ms3d *t;
        std::list<model_joint *> l;
        std::list<model_joint *>::iterator i;
        model_joint *p;
        std::vector<ms3d_model_joint_m> *lf;
        
        m = (model_readlock *)o.readLock( this->m );
        if( !m )
            return new model_saver_ms3d_state_cleanup( this->b, this->m, 0 );
        t = (model_saver_ms3d *)ml->getSaver();
        
        lf = t->joints;
        if( lf )
            delete lf;
        lf = new std::vector<ms3d_model_joint_m>();
        t->joints = lf;
        
        m->getJoints( &l );
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            this->makeJoint( t, p, m );
        }
        
        return new model_saver_ms3d_state_cleanup( this->b, this->m, 1 );
    }
    
    //makes joints for animation
    void model_saver_ms3d_state_make_joints::makeJoint( model_saver_ms3d *t, model_joint *j, model_readlock *m )
    {
        unsigned int i, e;
        std::vector<ms3d_model_frame> *l;
        std::vector<ms3d_model_joint_m> *lf;
        ms3d_model_frame *p;
        ms3d_model_joint_m v;
        dpxyz_f x;
        std::string s;
        
        v.id = j->getId();
        v.pid = j->getParent();
        v.f.flags = 0;
        v.f.cnt_rot_frames = 0;
        v.f.cnt_pos_frames = 0;
        
        j->getPosition( &x );
        v.f.pos.x = x.x;
        v.f.pos.y = x.y;
        v.f.pos.z = x.z;
        
        j->getRotation( &x );
        v.f.rot.x = x.x;
        v.f.rot.y = x.y;
        v.f.rot.z = x.z;
        
        memset( v.f.name, 0, sizeof( v.f.name ) );
        memset( v.f.parent_name, 0, sizeof( v.f.parent_name ) );
        
        j->getName( &s );
        s.copy( (char *)v.f.name, sizeof( v.f.name ) );
        
        j = m->findJoint( v.pid );
        if( j )
        {
            j->getName( &s );
            s.copy( (char *)v.f.parent_name, sizeof( v.f.parent_name ) );
        }
        
        lf = t->joints;
        l = t->frames;
        if( !l )
        {
            lf->push_back( v );
            return;
        }
        e = (unsigned int)l->size();
        
        for( i = 0; i < e; i++ )
        {
            p = &( *l )[ i ];
            this->makeKeyframes( t, &v, p, m );
        }

        lf->push_back( v );
    }
  
    //make joint keyframes
    void model_saver_ms3d_state_make_joints::makeKeyframes( model_saver_ms3d *t, ms3d_model_joint_m *j, ms3d_model_frame *f, model_readlock *m )
    {
        std::list<model_frame_joint *> l;
        std::list<model_frame_joint *>::iterator i;
        model_frame_joint *p;
        ms3d_model_joint_keyframe kf;
        dpxyz_f x;
        
        m->getFrameJoints( &l, f->id, j->id );
        
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            
            p->getTranslation( &x );
            kf.time = f->ot;
            kf.x = x.x;
            kf.y = x.y;
            kf.z = x.z;
            j->translate_frames.push_back( kf );
            j->f.cnt_pos_frames++;
            
            p->getRotation( &x );
            kf.time = f->ot;
            kf.x = x.x;
            kf.y = x.y;
            kf.z = x.z;
            j->rotate_frames.push_back( kf );
            j->f.cnt_rot_frames++;
        }
        
    }
    
};
