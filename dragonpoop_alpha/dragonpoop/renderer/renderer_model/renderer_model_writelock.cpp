
#include "renderer_model_writelock.h"
#include "renderer_model.h"

namespace dragonpoop
{
    
    //ctor
    renderer_model_writelock::renderer_model_writelock( renderer_model *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    renderer_model_writelock::~renderer_model_writelock( void )
    {
        
    }
    
    //get id
    dpid renderer_model_writelock::getId( void )
    {
        return this->t->getId();
    }
    
    //add component to list and trees
    void renderer_model_writelock::addComponent( model_component *c )
    {
        this->t->addComponent( c );
    }
    
    //find component by type and id
    model_component *renderer_model_writelock::findComponent( uint16_t mtype, dpid id )
    {
        return this->t->findComponent( mtype, id );
    }
    
    //find components by type
    void renderer_model_writelock::getComponents( uint16_t mtype, std::list<model_component *> *l )
    {
        this->t->getComponents( mtype, l );
    }
    
    //find components by type and 1 parent
    void renderer_model_writelock::getComponentsByParent( uint16_t mtype, dpid p1, std::list<model_component *> *l )
    {
        this->t->getComponentsByParent( mtype, p1, l );
    }
    
    //find components by type and 2 parents
    void renderer_model_writelock::getComponentsByParents( uint16_t mtype, dpid p1, dpid p2, std::list<model_component *> *l )
    {
        this->t->getComponentsByParents( mtype, p1, p2, l );
    }
    
    //remove component
    void renderer_model_writelock::removeComponent( model_component *c )
    {
        this->t->removeComponent( c );
    }
    
    //create instance
    void renderer_model_writelock::makeInstance( model_instance_writelock *ml )
    {
        this->t->makeInstance( ml );
    }
    
    //sync model instance with changes
    void renderer_model_writelock::sync( void )
    {
        this->t->sync();
    }
    
    //find material
    renderer_model_material *renderer_model_writelock::findMaterial( dpid id )
    {
        return this->t->findMaterial( id );
    }
    
    //get materials
    void renderer_model_writelock::getMaterials( std::list<renderer_model_material *> *l )
    {
        this->t->getMaterials( l );
    }
    
    //get size
    void renderer_model_writelock::getSize( dpxyz_f *x )
    {
        this->t->getSize( x );
    }
    
    //get center
    void renderer_model_writelock::getCenter( dpxyz_f *x )
    {
        this->t->getCenter( x );
    }
    
    //returns true if alive
    bool renderer_model_writelock::isAlive( void )
    {
        return this->t->isAlive();
    }
    
    //kill model
    void renderer_model_writelock::kill( void )
    {
        this->t->kill();
    }

};
