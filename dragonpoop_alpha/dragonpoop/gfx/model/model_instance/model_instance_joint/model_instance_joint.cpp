
#include "model_instance_joint.h"
#include "../model_instance_writelock.h"
#include "../../model_matrix/model_vector.h"
#include "../../model_matrix/model_quaternion.h"
#include "../../../dpvertex/dpxyz_f.h"
#include "../model_instance_joint_cache/model_instance_joint_cache.h"

namespace dragonpoop
{
    
    //ctor
    model_instance_joint::model_instance_joint( model_joint *j ) : model_component( j->getId(), model_component_type_joint )
    {
        std::string s;
        
        j->getPosition( &this->pos );
        j->getRotation( &this->rot );
        this->parent_id = j->getParent();
        this->p_index = -1;
        
        j->getName( &s );
        this->setName( &s );
    }
    
    //dtor
    model_instance_joint::~model_instance_joint( void )
    {
        
    }
    
    //set position
    void model_instance_joint::setPosition( dpxyz_f *x )
    {
        this->pos = *x;
    }
    
    //get position
    void model_instance_joint::getPosition( dpxyz_f *x )
    {
        x->x = this->pos.x + this->apos.x;
        x->y = this->pos.y + this->apos.y;
        x->z = this->pos.z + this->apos.z;
    }
    
    //set rotation
    void model_instance_joint::setRotation( dpxyz_f *x )
    {
        this->rot = *x;
    }
    
    //get rotation
    void model_instance_joint::getRotation( dpxyz_f *x )
    {
        x->x = this->rot.x + this->arot.x;
        x->y = this->rot.y + this->arot.y;
        x->z = this->rot.z + this->arot.z;
    }
    
    //set animation position
    void model_instance_joint::setStartPosition( dpxyz_f *x )
    {
        this->arot_old = *x;
    }
    
    //set animation rotation
    void model_instance_joint::setStartRotation( dpxyz_f *x )
    {
        this->apos_old = *x;
    }
    
    //set animation position
    void model_instance_joint::setStopPosition( dpxyz_f *x )
    {
        this->arot = *x;
    }
    
    //set animation rotation
    void model_instance_joint::setStopRotation( dpxyz_f *x )
    {
        this->apos = *x;
    }
   
    //redo matrix
    void model_instance_joint::redoMatrix( model_instance_writelock *m )
    {
        model_instance_joint *j;
        
        j = 0;
        if( !dpid_isZero( &this->parent_id ) )
            j = (model_instance_joint *)m->findComponent( model_component_type_joint, this->parent_id );
        if( j )
            this->p_index = j->getIndex();
    }
    
    //set joint index
    void model_instance_joint::setIndex( int16_t i )
    {
        this->index = i;
    }
    
    //get joint index
    int16_t model_instance_joint::getIndex( void )
    {
        return this->index;
    }
  
    //populate joint cache entity
    void model_instance_joint::fillCacheEntity( model_instance_joint_cache_element *e )
    {
        if( this->p_index == this->index )
            this->p_index = -1;
        e->id = this->index;
        e->pid = this->p_index;

        e->bone_pos = this->pos;
        e->bone_rot = this->rot;
        e->rot_start = this->arot_old;
        e->rot_end = this->arot;
        e->pos_start = this->apos_old;
        e->pos_end = this->apos;
    }
    
};
