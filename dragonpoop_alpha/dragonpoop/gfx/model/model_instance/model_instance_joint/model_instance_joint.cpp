
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
        this->old_m.copy( &this->m );
        this->m.setIdentity();
        this->redoMatrixUp( m, &this->m );
        this->redoMatrixDown( m, &this->m );
    }
    
    //redo matrix
    void model_instance_joint::redoMatrixUp( model_instance_writelock *m, dpmatrix *t )
    {
        model_instance_joint *j;
        
        j = 0;
        if( !dpid_isZero( &this->parent_id ) )
            j = (model_instance_joint *)m->findComponent( model_component_type_joint, this->parent_id );
        
        if( j )
        {
            this->p_index = j->getIndex();
            j->redoMatrixUp( m, t );
        }
        
        t->rotateZrad( -this->rot.z );
        t->rotateYrad( -this->rot.y );
        t->rotateXrad( -this->rot.x );
        t->translate( -this->pos.x, -this->pos.y, -this->pos.z );
        
        t->translate( -this->apos.x, -this->apos.y, -this->apos.z );
        t->rotateZrad( this->arot.x );
        t->rotateYrad( this->arot.y );
        t->rotateXrad( this->arot.z );
        t->translate( this->apos.x, this->apos.y, this->apos.z );
        
    }
    
    //redo matrix
    void model_instance_joint::redoMatrixDown( model_instance_writelock *m, dpmatrix *t )
    {
        model_instance_joint *j;
        
        j = 0;
        if( !dpid_isZero( &this->parent_id ) )
            j = (model_instance_joint *)m->findComponent( model_component_type_joint, this->parent_id );
        
        t->translate( this->pos.x, this->pos.y, this->pos.z );
        t->rotateXrad( this->rot.x );
        t->rotateYrad( this->rot.y );
        t->rotateZrad( this->rot.z );

        if( j )
            j->redoMatrixDown( m, t );
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
        e->pid = this->p_index;
        e->end.copy( &this->m );
        e->start.copy( &this->old_m );
    }
    
};
