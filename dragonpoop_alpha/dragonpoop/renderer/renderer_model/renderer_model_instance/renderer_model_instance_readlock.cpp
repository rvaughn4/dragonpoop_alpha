
#include "renderer_model_instance_readlock.h"
#include "renderer_model_instance.h"

namespace dragonpoop
{
    
    //ctor
    renderer_model_instance_readlock::renderer_model_instance_readlock( renderer_model_instance *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    renderer_model_instance_readlock::~renderer_model_instance_readlock( void )
    {
        
    }
    
    //get id
    dpid renderer_model_instance_readlock::getId( void )
    {
        return this->t->getId();
    }
    
    //find component by type and id
    model_component *renderer_model_instance_readlock::findComponent( uint16_t mtype, dpid id )
    {
        return this->t->findComponent( mtype, id );
    }
    
    //find components by type
    void renderer_model_instance_readlock::getComponents( uint16_t mtype, std::list<model_component *> *l )
    {
        this->t->getComponents( mtype, l );
    }
    
    //find components by type and 1 parent
    void renderer_model_instance_readlock::getComponentsByParent( uint16_t mtype, dpid p1, std::list<model_component *> *l )
    {
        this->t->getComponentsByParent( mtype, p1, l );
    }
    
    //find components by type and 2 parents
    void renderer_model_instance_readlock::getComponentsByParents( uint16_t mtype, dpid p1, dpid p2, std::list<model_component *> *l )
    {
        this->t->getComponentsByParents( mtype, p1, p2, l );
    }
    
    //find group
    renderer_model_instance_group *renderer_model_instance_readlock::findGroup( dpid id )
    {
        return this->t->findGroup( id );
    }
    
    //find joint
    renderer_model_instance_joint *renderer_model_instance_readlock::findJoint( dpid id )
    {
        return this->t->findJoint( id );
    }
    
    //returns joint cache
    model_instance_joint_cache *renderer_model_instance_readlock::getJointCache( void )
    {
        return this->t->getJointCache();
    }
    
    //get groups
    void renderer_model_instance_readlock::getGroups( std::list<renderer_model_instance_group *> *l )
    {
        this->t->getGroups( l );
    }
    
    //get joints
    void renderer_model_instance_readlock::getJoints( std::list<renderer_model_instance_joint *> *l )
    {
        this->t->getJoints( l );
    }
    
    //sync
    void renderer_model_instance_readlock::sync( void )
    {
        this->t->sync();
    }
    
    //animate
    void renderer_model_instance_readlock::animate( void )
    {
        this->t->animate();
    }
    
    //render model
    void renderer_model_instance_readlock::render( dpthread_lock *thd, renderer_writelock *r, renderer_model_readlock *m, dpmatrix *m_world )
    {
        this->t->render( thd, r, m, this, m_world );
    }
    
    //transform vertex using joints
    void renderer_model_instance_readlock::transform( dpvertex *v )
    {
        this->t->transform( v );
    }
    
    //recompute animation joint matrixes
    void renderer_model_instance_readlock::redoMatrixes( uint64_t t )
    {
        this->t->redoMatrixes( this, t );
    }
    
    //get model view matrix
    void renderer_model_instance_readlock::getModelViewMatrix( dpthread_lock *thd, renderer_writelock *r, renderer_model_readlock *m, dpmatrix *in_world_matrix, dpmatrix *out_model_matrix )
    {
        this->t->getModelViewMatrix( thd, r, m, in_world_matrix, out_model_matrix );
    }

    //get dimensions
    model_gui_pos *renderer_model_instance_readlock::getGuiDimensions( void )
    {
        return this->t->getGuiDimensions();
    }
    
    //returns true if gui
    bool renderer_model_instance_readlock::isGui( void )
    {
        return this->t->isGui();
    }
    
};
