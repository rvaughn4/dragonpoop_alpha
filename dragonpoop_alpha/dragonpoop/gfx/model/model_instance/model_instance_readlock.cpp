
#include "model_instance_readlock.h"
#include "model_instance.h"

namespace dragonpoop
{
    
    //ctor
    model_instance_readlock::model_instance_readlock( model_instance *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    model_instance_readlock::~model_instance_readlock( void )
    {
        
    }
    
    //return core
    core *model_instance_readlock::getCore( void )
    {
        return this->t->getCore();
    }
    
    //get id
    dpid model_instance_readlock::getId( void )
    {
        return this->t->getId();
    }
    
    //find component by type and id
    model_component *model_instance_readlock::findComponent( uint16_t mtype, dpid id )
    {
        return this->t->findComponent( mtype, id );
    }
    
    //find components by type
    void model_instance_readlock::getComponents( uint16_t mtype, std::list<model_component *> *l )
    {
        this->t->getComponents( mtype, l );
    }
    
    //find components by type and 1 parent
    void model_instance_readlock::getComponentsByParent( uint16_t mtype, dpid p1, std::list<model_component *> *l )
    {
        this->t->getComponentsByParent( mtype, p1, l );
    }
    
    //find components by type and 2 parents
    void model_instance_readlock::getComponentsByParents( uint16_t mtype, dpid p1, dpid p2, std::list<model_component *> *l )
    {
        this->t->getComponentsByParents( mtype, p1, p2, l );
    }
    
    //find group
    model_instance_group *model_instance_readlock::findGroup( dpid id )
    {
        return this->t->findGroup( id );
    }
    
    //get groups
    void model_instance_readlock::getGroups( std::list<model_instance_group *> *l )
    {
        this->t->getGroups( l );
    }

    //populate vertex buffer for rendering
    void model_instance_readlock::fillVertexBuffer( dpid group_id, dpvertexindex_buffer *vb )
    {
        this->t->fillVertexBuffer( group_id, vb );
    }
    
    //find animation
    model_instance_animation *model_instance_readlock::findAnimation( dpid id )
    {
        return this->t->findAnimation( id );
    }
    
    //get animations
    void model_instance_readlock::getAnimations( std::list<model_instance_animation *> *l )
    {
        this->t->getAnimations( l );
    }
    
    //sync
    void model_instance_readlock::sync( void )
    {
        this->t->sync();
    }
    
    //get start time
    uint64_t model_instance_readlock::getStartTime( void )
    {
        return this->t->getStartTime();
    }
    
    //get end time
    uint64_t model_instance_readlock::getEndTime( void )
    {
        return this->t->getEndTime();
    }
    
};
