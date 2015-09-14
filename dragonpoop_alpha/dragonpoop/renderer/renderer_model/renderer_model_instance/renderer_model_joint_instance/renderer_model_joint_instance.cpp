
#include "renderer_model_joint_instance.h"
#include "../../../../gfx/model/model_instance/model_instance_joint/model_instance_joint.h"
#include "../../../../gfx/model/model_instance/model_instance_writelock.h"
#include "../../../../core/dpthread/dpthread_lock.h"
#include "../renderer_model_instance_writelock.h"
#include "../renderer_model_instance_readlock.h"

namespace dragonpoop
{
    
    //ctor
    renderer_model_instance_joint::renderer_model_instance_joint( model_instance_writelock *ml, model_instance_joint *g, dpthread_lock *thd ) : model_component( g->getId(), model_component_type_joint )
    {
        this->index = g->getIndex();
        this->parent_id = g->getParentId();
        this->start.t = g->getStartData( &this->start.pos, &this->start.rot );
        this->end = this->start;
        this->sync( ml, g, thd );
    }
    
    //dtor
    renderer_model_instance_joint::~renderer_model_instance_joint( void )
    {
        
    }
    
    //sync with joint
    void renderer_model_instance_joint::sync( model_instance_writelock *ml, model_instance_joint *g, dpthread_lock *thd )
    {
        dpquaternion q, qs, qe;
        float rs, re;
        uint64_t tt, td, t;

        //bone data
        g->getPosition( &this->orig.pos );
        g->getRotation( &this->orig.rot );
        
        //find ratio for lerping start
        t = thd->getTicks();
        tt = t - this->start.t;
        td = this->end.t - this->start.t;
        if( !td )
            re = 0;
        else
        {
            if( tt >= td )
                re = 1;
            else
                re = (float)tt / (float)td;
        }
        rs = 1.0f - re;
        
        //lerp position
        this->start.pos.x = this->start.pos.x * rs + this->end.pos.x * re;
        this->start.pos.y = this->start.pos.y * rs + this->end.pos.y * re;
        this->start.pos.z = this->start.pos.z * rs + this->end.pos.z * re;
        
        //slerp rotation
        qs.setAngle( &this->start.rot );
        qe.setAngle( &this->end.rot );
        q.slerp( &qs, &qe, re );
        q.getAngle( &this->start.rot );
        
        this->start.t = t;
        this->end.t = g->getEndData( &this->end.pos, &this->end.rot );
    }
    
    //recompute matrix
    void renderer_model_instance_joint::redoMatrix( renderer_model_instance_writelock *m, uint64_t t )
    {
        renderer_model_instance_joint *p;
        
        //called recursively by children, dont recompute matrix unecessarily
        if( t == this->t_matrix )
            return;
        this->t_matrix = t;

        //find parent
        p = 0;
        if( !dpid_isZero( &this->parent_id ) )
            p = m->findJoint( this->parent_id );
        if( p )
            p->redoMatrix( m, t );
        
        this->__redoMatrix( p, t );
    }
    
    //recompute matrix
    void renderer_model_instance_joint::redoMatrix( renderer_model_instance_readlock *m, uint64_t t )
    {
        renderer_model_instance_joint *p;
        
        //called recursively by children, dont recompute matrix unecessarily
        if( t == this->t_matrix )
            return;
        this->t_matrix = t;

        //find parent
        p = 0;
        if( !dpid_isZero( &this->parent_id ) )
            p = m->findJoint( this->parent_id );
        if( p )
            p->redoMatrix( m, t );
        
        this->__redoMatrix( p, t );
    }
    
    //recompute matrix
    void renderer_model_instance_joint::__redoMatrix( renderer_model_instance_joint *p, uint64_t t )
    {
        dpmatrix m;
        dpxyz_f pos, rot;
        dpquaternion q, qs, qe;
        float rs, re;
        uint64_t tt, td;
        
        //compute bone local
        this->bone_local.setAngleRadAndPosition( &this->orig.rot, &this->orig.pos );
        
        //compute bone global
        if( p )
        {
            this->bone_global.copy( &p->bone_global );
            this->bone_global.multiply( &this->bone_local );
        }
        else
            this->bone_global.copy( &this->bone_local );
        this->bone_global_inv.inverse( &this->bone_global );
        
        //compute animation local
        td = this->end.t - this->start.t;
        tt = t - this->start.t;
        if( !td )
            re = 0;
        else
        {
            if( tt >= td )
                re = 1;
            else
                re = (float)tt / (float)td;
        }
        rs = 1.0f - re;
        
        pos.x = this->start.pos.x * rs + this->end.pos.x * re;
        pos.y = this->start.pos.y * rs + this->end.pos.y * re;
        pos.z = this->start.pos.z * rs + this->end.pos.z * re;
        
        qs.setAngle( &this->start.rot );
        qe.setAngle( &this->end.rot );
        q.slerp( &qs, &qe, re );
        q.getAngle( &rot );
        
        this->anim_local.setAngleRadAndPosition( &rot, &pos );
        
        //compute animation global
        m.copy( &this->bone_local );
        m.multiply( &this->anim_local );
        if( p )
        {
            this->anim_global.copy( &p->anim_global );
            this->anim_global.multiply( &m );
        }
        else
            this->anim_global.copy( &m );

        //this->anim_global.copy( &this->bone_global );
        this->anim_global_inv.inverse( &this->anim_global );
    }
    
    //transform vertex by joint matrixes
    void renderer_model_instance_joint::transform( dpvertex *v )
    {
        this->bone_global_inv.transform( &v->pos );
        this->bone_global_inv.transform( &v->normal );
        this->anim_global.transform( &v->pos );
        this->anim_global.transform( &v->normal );
    }
    
    //return index
    int16_t renderer_model_instance_joint::getIndex( void )
    {
        return this->index;
    }

    //get joint position
    void renderer_model_instance_joint::getTransformedPosition( dpxyz_f *x )
    {
        this->anim_global.getPosition( x );
    }
    
    //return parent id
    dpid renderer_model_instance_joint::getParentId( void )
    {
        return this->parent_id;
    }
    
};
