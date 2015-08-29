
#include "model_instance_joint.h"
#include "../model_instance_writelock.h"

namespace dragonpoop
{
    
    //ctor
    model_instance_joint::model_instance_joint( model_joint *j ) : model_component( j->getId(), model_component_type_joint )
    {
        std::string s;
        
        j->getPosition( &this->pos );
        j->getRotation( &this->rot );
        this->parent_id = j->getParent();
        
        j->getName( &s );
        this->setName( &s );
    }
    
    //dtor
    model_instance_joint::~model_instance_joint( void )
    {
        
    }
    
    //set position
    void model_instance_joint::setPosition( dpxyzw *x )
    {
        this->pos = *x;
    }
    
    //get position
    void model_instance_joint::getPosition( dpxyzw *x )
    {
        x->x = this->pos.x + this->apos.x;
        x->y = this->pos.y + this->apos.y;
        x->z = this->pos.z + this->apos.z;
        x->w = this->pos.w + this->apos.w;
    }
    
    //set rotation
    void model_instance_joint::setRotation( dpxyzw *x )
    {
        this->rot = *x;
    }
    
    //get rotation
    void model_instance_joint::getRotation( dpxyzw *x )
    {
        x->x = this->rot.x + this->arot.x;
        x->y = this->rot.y + this->arot.y;
        x->z = this->rot.z + this->arot.z;
        x->w = this->rot.w + this->arot.w;
    }
    
    //set animation position
    void model_instance_joint::setAnimationPosition( dpxyzw *x )
    {
        this->arot = *x;
    }
    
    //set animation rotation
    void model_instance_joint::setAnimationRotation( dpxyzw *x )
    {
        this->apos = *x;
    }
    
    //transform using matrix
    void model_instance_joint::transform( model_instance_writelock *m, dpxyzw *x )
    {
        this->redoMatrix( m );
/*
        x->x -= this->global_bone.x;
        x->y -= this->global_bone.y;
        x->z -= this->global_bone.z;
  */
        this->global.transform( x );
/*
        x->x += this->global_bone.x;
        x->y += this->global_bone.y;
        x->z += this->global_bone.z;
  */  }
    
    //reset matrix
    void model_instance_joint::reset( void )
    {
        this->isChained = 0;
    }
    
    //redo matrix
    void model_instance_joint::redoMatrix( model_instance_writelock *m )
    {
        model_instance_joint *j;
        
        if( this->isChained )
            return;
        
        this->global_bone = this->pos;
        
        this->local.setIdentity();
        this->local.rotateRad( this->arot.x, this->arot.y, this->arot.z );
        this->local.translate( this->apos.x, this->apos.y, this->apos.z );
        
        j = 0;
        if( !dpid_isZero( &this->parent_id ) )
            j = (model_instance_joint *)m->findComponent( model_component_type_joint, this->parent_id );
        if( !j )
        {
            this->isChained = 1;
            this->global.copy( &this->local );
            return;
        }

        j->redoMatrix( m );
        
        this->global.setIdentity();
        this->global.multiply( &j->global );
        this->global.multiply( &this->local );

        this->global_bone.x += j->global_bone.x;
        this->global_bone.y += j->global_bone.y;
        this->global_bone.z += j->global_bone.z;
        
        this->isChained = 1;
    }
    
};
