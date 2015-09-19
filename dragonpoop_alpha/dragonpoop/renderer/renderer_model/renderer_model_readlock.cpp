
#include "renderer_model_readlock.h"
#include "renderer_model.h"

namespace dragonpoop
{
    
    //ctor
    renderer_model_readlock::renderer_model_readlock( renderer_model *t, dpmutex_readlock *l ) :shared_obj_readlock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    renderer_model_readlock::~renderer_model_readlock( void )
    {
        
    }
    
    //get id
    dpid renderer_model_readlock::getId( void )
    {
        return this->t->getId();
    }
    
    //find component by type and id
    model_component *renderer_model_readlock::findComponent( uint16_t mtype, dpid id )
    {
        return this->t->findComponent( mtype, id );
    }
    
    //find components by type
    void renderer_model_readlock::getComponents( uint16_t mtype, std::list<model_component *> *l )
    {
        this->t->getComponents( mtype, l );
    }
    
    //find components by type and 1 parent
    void renderer_model_readlock::getComponentsByParent( uint16_t mtype, dpid p1, std::list<model_component *> *l )
    {
        this->t->getComponentsByParent( mtype, p1, l );
    }
    
    //find components by type and 2 parents
    void renderer_model_readlock::getComponentsByParents( uint16_t mtype, dpid p1, dpid p2, std::list<model_component *> *l )
    {
        this->t->getComponentsByParents( mtype, p1, p2, l );
    }
    
    //sync model instance with changes
    void renderer_model_readlock::sync( void )
    {
        this->t->sync();
    }
    
    //render model
    void renderer_model_readlock::render( dpthread_lock *thd, renderer_writelock *r, bool doGui, dpmatrix *m_world )
    {
        this->t->render( thd, r, this, doGui, m_world );
    }
    
    //find material
    renderer_model_material *renderer_model_readlock::findMaterial( dpid id )
    {
        return this->t->findMaterial( id );
    }
    
    //get materials
    void renderer_model_readlock::getMaterials( std::list<renderer_model_material *> *l )
    {
        this->t->getMaterials( l );
    }
    
    //get size
    void renderer_model_readlock::getSize( dpxyz_f *x )
    {
        this->t->getSize( x );
    }

};
