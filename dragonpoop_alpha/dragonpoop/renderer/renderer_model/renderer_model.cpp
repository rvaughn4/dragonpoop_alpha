
#include "renderer_model.h"
#include "renderer_model_ref.h"
#include "renderer_model_readlock.h"
#include "renderer_model_writelock.h"
#include "../../gfx/model/model_writelock.h"
#include "../../core/core.h"

namespace dragonpoop
{
    
    //ctor
    renderer_model::renderer_model( model_writelock *ml ) : shared_obj( ml->getCore()->getMutexMaster() )
    {
        
    }
    
    //dtor
    renderer_model::~renderer_model( void )
    {
        
    }
    
    //generate read lock
    shared_obj_readlock *renderer_model::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new renderer_model_readlock( (renderer_model *)p, l );
    }
    
    //generate write lock
    shared_obj_writelock *renderer_model::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new renderer_model_writelock( (renderer_model *)p, l );
    }
    
    //generate ref
    shared_obj_ref *renderer_model::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new renderer_model_ref( (renderer_model *)p, k );
    }
    
    //get id
    dpid renderer_model::getId( void )
    {
        return this->id;
    }
    
    //add component to list and trees
    void renderer_model::addComponent( model_component *c )
    {
        this->comps.lst.push_back( c );
    }
    
    //find component by type and id
    model_component *renderer_model::findComponent( uint16_t mtype, dpid id )
    {
        std::list<model_component *> *l;
        std::list<model_component *>::iterator i;
        model_component *c;
        
        //return this->comps.bytypeid.findLeaf( mtype, id );
        
        l = &this->comps.lst;
        for( i = l->begin(); i != l->end(); ++i )
        {
            c = *i;
            if( c->getType() != mtype )
                continue;
            if( !c->compareId( id ) )
                continue;
            return c;
        }
        
        return 0;
    }
    
    //find components by type
    void renderer_model::getComponents( uint16_t mtype, std::list<model_component *> *ll )
    {
        std::list<model_component *> *l;
        std::list<model_component *>::iterator i;
        model_component *c;
        
        //this->comps.bytype.findLeaves( mtype, ll );
        l = &this->comps.lst;
        for( i = l->begin(); i != l->end(); ++i )
        {
            c = *i;
            if( c->getType() != mtype )
                continue;
            ll->push_back( c );
        }
    }
    
    //find components by type and 1 parent
    void renderer_model::getComponentsByParent( uint16_t mtype, dpid p1, std::list<model_component *> *ll )
    {
        std::list<model_component *> *l;
        std::list<model_component *>::iterator i;
        model_component *c;
        
        //this->comps.bytypeowner.findLeaves( mtype, p1, l );
        l = &this->comps.lst;
        for( i = l->begin(); i != l->end(); ++i )
        {
            c = *i;
            if( c->getType() != mtype )
                continue;
            if( !c->hasParent( p1 ) )
                continue;
            ll->push_back( c );
        }        
    }
    
    //find components by type and 2 parents
    void renderer_model::getComponentsByParents( uint16_t mtype, dpid p1, dpid p2, std::list<model_component *> *ll )
    {
        std::list<model_component *> *l;
        std::list<model_component *>::iterator i;
        model_component *c;
        
        //this->comps.bytypeowner.findLeaves( mtype, p1, l );
        l = &this->comps.lst;
        for( i = l->begin(); i != l->end(); ++i )
        {
            c = *i;
            if( c->getType() != mtype )
                continue;
            if( !c->hasParent( p1 ) )
                continue;
            if( !c->hasParent( p2 ) )
                continue;
            ll->push_back( c );
        }
    }
    
    //remove component
    void renderer_model::removeComponent( model_component *c )
    {
        this->comps.lst.remove( c );
    }
    
    //create instance
    renderer_model_instance_ref *renderer_model::makeInstance( model_instance_writelock *ml, renderer_model_writelock *rl )
    {
        return 0;
    }
    
    //sync model instance with changes
    void renderer_model::sync( void )
    {
        
    }
    
    //delete all components
    void renderer_model::deleteComponents( void )
    {
        std::list<model_component *> *l, d;
        std::list<model_component *>::iterator i;
        model_component *c;
        
        l = &this->comps.lst;
        for( i = l->begin(); i != l->end(); ++i )
        {
            c = *i;
            d.push_back( c );
        }
        l->clear();
        
        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            c = *i;
            delete c;
        }
    }
    
    //delete instances
    void renderer_model::deleteInstances( void )
    {
        
    }
    
    //sync instances
    void renderer_model::syncInstances( renderer_model_writelock *g )
    {
        
    }
    
};
