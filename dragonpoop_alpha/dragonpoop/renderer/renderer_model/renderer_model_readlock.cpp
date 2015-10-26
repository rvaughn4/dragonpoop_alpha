
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
    void renderer_model_readlock::render( dpthread_lock *thd, dpposition *campos, dpmatrix *m_world, render_api_context_writelock *ctx, render_api_commandlist_writelock *clist )
    {
        this->t->render( thd, campos, this, m_world, ctx, clist );
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

    //get center
    void renderer_model_readlock::getCenter( dpxyz_f *x )
    {
        this->t->getCenter( x );
    }
    
    //returns true if alive
    bool renderer_model_readlock::isAlive( void )
    {
        return this->t->isAlive();
    }
    
};
