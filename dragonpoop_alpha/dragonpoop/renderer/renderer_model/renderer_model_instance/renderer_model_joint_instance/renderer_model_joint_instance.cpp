
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
        this->sync( ml, g, thd );
    }
    
    //dtor
    renderer_model_instance_joint::~renderer_model_instance_joint( void )
    {
        
    }
    
    //sync with joint
    void renderer_model_instance_joint::sync( model_instance_writelock *ml, model_instance_joint *g, dpthread_lock *thd )
    {
        g->getPosition( &this->orig.pos );
        g->getRotation( &this->orig.rot );
        this->start.t = g->getStartData( &this->start.pos, &this->start.rot );
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
        
        this->anim_global.copy( &this->bone_global );

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

};
