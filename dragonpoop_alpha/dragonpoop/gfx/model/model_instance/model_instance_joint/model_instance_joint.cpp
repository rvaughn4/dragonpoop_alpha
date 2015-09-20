
#include "model_instance_joint.h"
#include "../model_instance_writelock.h"
#include "../../model_writelock.h"
#include "../../model_matrix/model_vector.h"
#include "../../model_matrix/model_quaternion.h"
#include "../../../dpvertex/dpxyz_f.h"
#include "../model_instance_joint_cache/model_instance_joint_cache.h"
#include "../../../../core/dpthread/dpthread_lock.h"
#include "../../../dpmatrix/dpmatrixes.h"
#include "../model_instance_animation/model_instance_animation.h"
#include "../../model_frame_joint/model_frame_joint.h"

#include <iostream>
#include <sstream>

namespace dragonpoop
{
    
    //ctor
    model_instance_joint::model_instance_joint( model_joint *j, int16_t index ) : model_component( j->getId(), model_component_type_joint )
    {
        std::string s;
        j->getPosition( &this->orig.pos );
        j->getRotation( &this->orig.rot );
        this->orig.t = 0;
        this->parent_id = j->getParent();
        j->getName( &s );
        this->setName( &s );
        this->end = this->start = this->orig;
        this->index = index;
    }
    
    //dtor
    model_instance_joint::~model_instance_joint( void )
    {
        
    }
    
    //get position
    void model_instance_joint::getPosition( dpxyz_f *x )
    {
        *x = this->orig.pos;
    }
    
    //get rotation
    void model_instance_joint::getRotation( dpxyz_f *x )
    {
        *x = this->orig.rot;
    }
  
    //run animation
    void model_instance_joint::run( model_instance_writelock *mi, model_writelock *m, dpthread_lock *thd )
    {
        uint64_t t, te;
        
        t = thd->getTicks();
        this->blendStart( t );
        
        te = this->findLowestEndTime( mi );
        if( te <= t )
            te = t + 200;
        
        this->combineAllTransforms( te, mi, m, &this->end.pos, &this->end.rot );
        this->end.t = te;
    }
    
    //blend end into start
    void model_instance_joint::blendStart( uint64_t t )
    {
        dpquaternion q, qs, qe;
        float rs, re;
        uint64_t td, tt;
        
        //not yet started
        if( !this->start.t )
        {
            memset( &this->start, 0, sizeof( this->start ) );
            this->end = this->start;
            this->start.t = t;
            this->end.t = t;
            return;
        }
        
        //compute ratios for lerp
        td = this->end.t - this->start.t;
        tt = t - this->start.t;
        if( !td )
            re = 0.1f;
        else
        {
            if( tt >= td )
                re = 1.0f;
            else
                re = (float)tt / (float)td;
        }
        rs = 1.0f - re;
        
        //lerp the pos
        this->start.pos.x = this->start.pos.x * rs + this->end.pos.x * re;
        this->start.pos.y = this->start.pos.y * rs + this->end.pos.y * re;
        this->start.pos.z = this->start.pos.z * rs + this->end.pos.z * re;
        
        //slerp the rotation
        qs.setAngle( &this->start.rot );
        qe.setAngle( &this->end.rot );
        q.slerp( &qs, &qe, re );
        q.getAngle( &this->start.rot );
        
        this->start.t = t;
    }
    
    //find lowest end time
    uint64_t model_instance_joint::findLowestEndTime( model_instance_writelock *m )
    {
        std::list<model_instance_animation *> l;
        std::list<model_instance_animation *>::iterator i;
        model_instance_animation *p;
        uint64_t r, t;
        
        r = 0;
        m->getAnimations( &l );
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            if( !p->isPlaying() )
                continue;
            t = p->getEndTime();
            if( !r || r > t )
                r = t;
        }
        
        return r;
    }
    
    //get pos and rot for the given end time from all animations
    void model_instance_joint::combineAllTransforms( uint64_t t, model_instance_writelock *mi, model_writelock *m, dpxyz_f *pos, dpxyz_f *rot )
    {
        std::list<model_instance_animation *> l;
        std::list<model_instance_animation *>::iterator i;
        model_instance_animation *p;
        dpxyz_f ppos, prot;
        
        pos->x = pos->y = pos->z = 0;
        *rot = *pos;
        
        mi->getAnimations( &l );
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            if( !p->isPlaying() )
                continue;
            this->getTransforms( p, t, mi, m, &ppos, &prot );
            
            pos->x += ppos.x;
            pos->y += ppos.y;
            pos->z += ppos.z;
            
            rot->x += prot.x;
            rot->y += prot.y;
            rot->z += prot.z;
        }
    }
    
    //get pos and rot for the given end time from all frame_joints for the given animation and end time
    void model_instance_joint::getTransforms( model_instance_animation *a, uint64_t t, model_instance_writelock *mi, model_writelock *m, dpxyz_f *pos, dpxyz_f *rot )
    {
        dpid sfid, efid;
        dpxyz_f pos_s, pos_e, rot_s, rot_e;
        dpquaternion q, qs, qe;
        uint64_t ts, te, tt, td;
        float rs, re;
        
        //get frames and times
        efid = a->getEndFrame();
        sfid = a->getStartFrame();
        ts = a->getStartTime();
        te = a->getEndTime();
        
        //find ratios
        td = te - ts;
        tt = t - ts;
        if( !td )
            re = 0;
        else
        {
            if( tt >= td )
                re = 1.0f;
            else
                re = (float)tt / (float)td;
        }
        rs = 1.0f - re;
        
        //get transforms for both frames
        this->getTransforms( sfid, m, &pos_s, &rot_s );
        this->getTransforms( efid, m, &pos_e, &rot_e );
        
        //lerp position
        pos->x = pos_s.x * rs + pos_e.x * re;
        pos->y = pos_s.y * rs + pos_e.y * re;
        pos->z = pos_s.z * rs + pos_e.z * re;
        
        //slerp rotation
        qs.setAngle( &rot_s );
        qe.setAngle( &rot_e );
        q.slerp( &qs, &qe, re );
        q.getAngle( rot );
    }
    
    //get pos and rot for the given frame
    void model_instance_joint::getTransforms( dpid frame_id, model_writelock *m, dpxyz_f *pos, dpxyz_f *rot )
    {
        std::list<model_frame_joint *> l;
        std::list<model_frame_joint *>::iterator i;
        model_frame_joint *p;
        dpxyz_f x;
        
        pos->x = pos->y = pos->z = 0;
        *rot = *pos;
        
        m->getFrameJoints( &l, frame_id, this->getId() );
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            
            p->getTranslation( &x );
            pos->x += x.x;
            pos->y += x.y;
            pos->z += x.z;

            p->getRotation( &x );
            rot->x += x.x;
            rot->y += x.y;
            rot->z += x.z;
        }
    }
    
    //get start position, rotation, and time
    uint64_t model_instance_joint::getStartData( dpxyz_f *pos, dpxyz_f *rot )
    {
        *pos = this->start.pos;
        *rot = this->start.rot;
        return this->start.t;
    }
    
    //get end position, rotation, and time
    uint64_t model_instance_joint::getEndData( dpxyz_f *pos, dpxyz_f *rot )
    {
        *pos = this->end.pos;
        *rot = this->end.rot;
        return this->end.t;
    }
  
    //return index
    int16_t model_instance_joint::getIndex( void )
    {
        return this->index;
    }
    
    //return parent id
    dpid model_instance_joint::getParentId( void )
    {
        return this->parent_id;
    }
    
};
