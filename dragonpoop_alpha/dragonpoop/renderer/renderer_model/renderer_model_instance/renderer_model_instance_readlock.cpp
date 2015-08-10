
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
    
    //get groups
    void renderer_model_instance_readlock::getGroups( std::list<renderer_model_instance_group *> *l )
    {
        this->t->getGroups( l );
    }
    
    //sync
    void renderer_model_instance_readlock::sync( void )
    {
        this->t->sync();
    }
    
    //render model
    void renderer_model_instance_readlock::render( dpthread_lock *thd, renderer_writelock *r, renderer_model_readlock *m )
    {
        this->t->render( thd, r, m, this );
    }
    
};
