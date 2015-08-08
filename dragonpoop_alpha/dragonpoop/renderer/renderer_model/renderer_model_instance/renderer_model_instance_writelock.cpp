
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
    
    //get groups
    void renderer_model_instance_writelock::getGroups( std::list<renderer_model_instance_group *> *l )
    {
        this->t->getGroups( l );
    }
    
    //sync
    void renderer_model_instance_writelock::sync( void )
    {
        this->t->sync();
    }

    //run model from task
    void renderer_model_instance_writelock::run( dpthread_lock *thd )
    {
        this->t->run( thd, this );
    }
};
