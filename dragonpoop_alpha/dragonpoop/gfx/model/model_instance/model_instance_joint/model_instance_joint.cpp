
#include "model_instance_joint.h"
#include "../model_instance_writelock.h"
#include "../../model_matrix/model_vector.h"
#include "../../../dpvertex/dpxyz_f.h"
#include "../model_instance_joint_cache/model_instance_joint_cache.h"

namespace dragonpoop
{
    
    //ctor
    model_instance_joint::model_instance_joint( model_joint *j ) : model_component( j->getId(), model_component_type_joint )
    {
        std::string s;
        model_vector v;
        
        j->getPosition( &this->pos );
        j->getRotation( &this->rot );
        this->parent_id = j->getParent();
        
        this->isOldFilled = 0;
        this->isChained = 0;
        v.setPosition( &this->rot );
        this->local_bone.setAngle( &v );
        v.setPosition( &this->pos );
        this->local_bone.setPosition( &v );
        
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
    void model_instance_joint::setAnimationPosition( dpxyz_f *x )
    {
        this->arot = *x;
    }
    
    //set animation rotation
    void model_instance_joint::setAnimationRotation( dpxyz_f *x )
    {
        this->apos = *x;
    }
    
    //reset matrix
    void model_instance_joint::reset( void )
    {
        //this->isChained = 0;
    }
    
    //redo matrix
    void model_instance_joint::redoMatrix( model_instance_writelock *m )
    {
        model_instance_joint *j;
        model_vector v;
        
        if( this->isChained )
            return;
        
        *( this->old_global.getData() ) = *( this->global.getData() );
        v.setPosition( &this->arot );
        v.setPosition( 0, 0, 0 );
        this->local.setAngle( &v );
        v.setPosition( &this->apos );
        v.setPosition( 0, 0, 0 );
        this->local.setPosition( &v );
        
        j = 0;
        if( !dpid_isZero( &this->parent_id ) )
            j = (model_instance_joint *)m->findComponent( model_component_type_joint, this->parent_id );
        
        if( !j )
        {
            *( this->global.getData() ) = *( this->local.getData() );
            *( this->global_bone.getData() ) = *( this->local_bone.getData() );
        }
        else
        {
            j->redoMatrix( m );
            this->global_bone.concat( &this->local_bone, &j->global_bone );
            this->global.concat( &this->local, &j->global );
        }

        if( !this->isOldFilled )
        {
            *( this->old_global.getData() ) = *( this->global.getData() );
            this->isOldFilled = 1;
        }
        this->isChained = 1;
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
        e->id = this->index;
        e->start.bones = *this->global_bone.getData();
        e->start.animation = *this->global.getData();
    }
    
};
