
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
        
        this->old_m_in.setIdentity();
        this->old_m_out.setIdentity();
        this->m_in.setIdentity();
        this->m_out.setIdentity();
        this->redoMatrixUpOld( m, &this->old_m_in );
        this->redoMatrixUp( m, &this->m_in );
        this->redoMatrixDownOld( m, &this->old_m_out );
        this->redoMatrixDown( m, &this->m_out );
    }
    
    //redo matrix
    void model_instance_joint::redoMatrixUpOld( model_instance_writelock *m, dpmatrix *t )
    {
        model_instance_joint *j;
        
        j = 0;
        if( !dpid_isZero( &this->parent_id ) )
            j = (model_instance_joint *)m->findComponent( model_component_type_joint, this->parent_id );
        
        if( j )
            j->redoMatrixUpOld( m, t );
        
        t->translate( this->pos.x, this->pos.y, this->pos.z );
        t->rotateZrad( this->rot.z );
        t->rotateYrad( this->rot.y );
        t->rotateXrad( this->rot.x );
        
        t->translate( -this->apos_old.x, -this->apos_old.y, -this->apos_old.z );
        t->rotateZrad( this->arot_old.z );
        t->rotateYrad( this->arot_old.y );
        t->rotateXrad( this->arot_old.x );
        t->translate( this->apos_old.x, this->apos_old.y, this->apos_old.z );
    }
    
    //redo matrix
    void model_instance_joint::redoMatrixUp( model_instance_writelock *m, dpmatrix *t )
    {
        model_instance_joint *j;
        
        j = 0;
        if( !dpid_isZero( &this->parent_id ) )
            j = (model_instance_joint *)m->findComponent( model_component_type_joint, this->parent_id );
        
        if( j )
            j->redoMatrixUp( m, t );
        
        t->translate( this->pos.x, this->pos.y, this->pos.z );
        t->rotateZrad( this->rot.z );
        t->rotateYrad( this->rot.y );
        t->rotateXrad( this->rot.x );
        
        t->translate( -this->apos.x, -this->apos.y, -this->apos.z );
        t->rotateZrad( this->arot.z );
        t->rotateYrad( this->arot.y );
        t->rotateXrad( this->arot.x );
        t->translate( this->apos.x, this->apos.y, this->apos.z );
    }

    //redo matrix
    void model_instance_joint::redoMatrixDownOld( model_instance_writelock *m, dpmatrix *t )
    {
        model_instance_joint *j;
        
        j = 0;
        if( !dpid_isZero( &this->parent_id ) )
            j = (model_instance_joint *)m->findComponent( model_component_type_joint, this->parent_id );
        
        t->rotateXrad( -this->rot.x );
        t->rotateYrad( -this->rot.y );
        t->rotateZrad( -this->rot.z );
        t->translate( -this->pos.x, -this->pos.y, -this->pos.z );
        
        if( j )
            j->redoMatrixDownOld( m, t );
    }
    
    //redo matrix
    void model_instance_joint::redoMatrixDown( model_instance_writelock *m, dpmatrix *t )
    {
        model_instance_joint *j;
        
        j = 0;
        if( !dpid_isZero( &this->parent_id ) )
            j = (model_instance_joint *)m->findComponent( model_component_type_joint, this->parent_id );

        t->rotateXrad( -this->rot.x );
        t->rotateYrad( -this->rot.y );
        t->rotateZrad( -this->rot.z );
        t->translate( -this->pos.x, -this->pos.y, -this->pos.z );

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
        if( this->p_index == this->index )
            this->p_index = -1;
        e->id = this->index;
        e->pid = this->p_index;
        e->end.copy( &this->m_in );
        e->start.copy( &this->old_m_in );
        e->bone_end.copy( &this->m_out );
        e->bone_start.copy( &this->old_m_out );
    }
    
};
