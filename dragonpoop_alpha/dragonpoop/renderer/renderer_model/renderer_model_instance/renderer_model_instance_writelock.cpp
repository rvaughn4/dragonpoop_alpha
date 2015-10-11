
#include "renderer_model_instance_writelock.h"
#include "renderer_model_instance.h"

namespace dragonpoop
{
    
    //ctor
    renderer_model_instance_writelock::renderer_model_instance_writelock( renderer_model_instance *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    renderer_model_instance_writelock::~renderer_model_instance_writelock( void )
    {
        
    }
    
    //get id
    dpid renderer_model_instance_writelock::getId( void )
    {
        return this->t->getId();
    }
    
    //find component by type and id
    model_component *renderer_model_instance_writelock::findComponent( uint16_t mtype, dpid id )
    {
        return this->t->findComponent( mtype, id );
    }
    
    //find components by type
    void renderer_model_instance_writelock::getComponents( uint16_t mtype, std::list<model_component *> *l )
    {
        this->t->getComponents( mtype, l );
    }
    
    //find components by type and 1 parent
    void renderer_model_instance_writelock::getComponentsByParent( uint16_t mtype, dpid p1, std::list<model_component *> *l )
    {
        this->t->getComponentsByParent( mtype, p1, l );
    }
    
    //find components by type and 2 parents
    void renderer_model_instance_writelock::getComponentsByParents( uint16_t mtype, dpid p1, dpid p2, std::list<model_component *> *l )
    {
        this->t->getComponentsByParents( mtype, p1, p2, l );
    }
    
    //find group
    renderer_model_instance_group *renderer_model_instance_writelock::findGroup( dpid id )
    {
        return this->t->findGroup( id );
    }
    
    //find joint
    renderer_model_instance_joint *renderer_model_instance_writelock::findJoint( dpid id )
    {
        return this->t->findJoint( id );
    }
    
    //get groups
    void renderer_model_instance_writelock::getGroups( std::list<renderer_model_instance_group *> *l )
    {
        this->t->getGroups( l );
    }
    
    //get joints
    void renderer_model_instance_writelock::getJoints( std::list<renderer_model_instance_joint *> *l )
    {
        this->t->getJoints( l );
    }
    
    //sync
    void renderer_model_instance_writelock::sync( void )
    {
        this->t->sync();
    }

    //animate
    void renderer_model_instance_writelock::animate( void )
    {
        this->t->animate();
    }
    
    //run model from task
    void renderer_model_instance_writelock::run( dpthread_lock *thd )
    {
        this->t->run( thd, this );
    }

    //returns joint cache
    model_instance_joint_cache *renderer_model_instance_writelock::getJointCache( void )
    {
        return this->t->getJointCache();
    }
    
    //transform vertex using joints
    void renderer_model_instance_writelock::transform( dpvertex *v )
    {
        this->t->transform( v );
    }
    
    //recompute animation joint matrixes
    void renderer_model_instance_writelock::redoMatrixes( uint64_t t )
    {
        this->t->redoMatrixes( this, t );
    }
    
    //get dimensions
    model_gui_pos *renderer_model_instance_writelock::getGuiDimensions( void )
    {
        return this->t->getGuiDimensions();
    }
    
    //returns true if gui
    bool renderer_model_instance_writelock::isGui( void )
    {
        return this->t->isGui();
    }
    
    //sync position
    void renderer_model_instance_writelock::syncPosition( void )
    {
        this->t->syncPosition();
    }
   
    //returns true if alive
    bool renderer_model_instance_writelock::isAlive( void )
    {
        return this->t->isAlive();
    }
    
    //kills instance
    void renderer_model_instance_writelock::kill( void )
    {
        this->t->kill();
    }
    
    //retuns opacity
    float renderer_model_instance_writelock::getAlpha( void )
    {
        return this->t->getAlpha();
    }
    
};
