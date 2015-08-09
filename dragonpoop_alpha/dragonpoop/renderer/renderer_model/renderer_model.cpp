
#include "renderer_model.h"
#include "renderer_model_ref.h"
#include "renderer_model_readlock.h"
#include "renderer_model_writelock.h"
#include "../../gfx/model/model_writelock.h"
#include "../../gfx/model/model_ref.h"
#include "../../core/core.h"
#include "renderer_model_instance/renderer_model_instance.h"
#include "renderer_model_instance/renderer_model_instance_writelock.h"
#include "../../core/bytetree/dpid_bytetree.h"
#include "../../gfx/model/model_instance/model_instance_writelock.h"
#include "../../gfx/model/model_instance/model_instance_ref.h"
#include "../../core/shared_obj/shared_obj_guard.h"
#include "../../core/dpthread/dpthread_lock.h"

namespace dragonpoop
{
    
    //ctor
    renderer_model::renderer_model( model_writelock *ml ) : shared_obj( ml->getCore()->getMutexMaster() )
    {
        this->m = (model_ref *)ml->getRef();
        this->bIsSynced = 0;
        this->id = ml->getId();
        ml->setRenderer( this );
    }
    
    //dtor
    renderer_model::~renderer_model( void )
    {
        this->deleteInstances();
        this->deleteComponents();
        delete this->m;
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
    void renderer_model::makeInstance( model_instance_writelock *ml )
    {
        renderer_model_instance *i;
        
        i = this->genInstance( ml );
        
        this->instances.push_back( i );
    }
    
    //sync model instance with changes
    void renderer_model::sync( void )
    {
        this->bIsSynced = 0;
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
        std::list<renderer_model_instance *> *l, d;
        std::list<renderer_model_instance *>::iterator i;
        renderer_model_instance *p;
        
        l = &this->instances;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            d.push_back( p );
        }
        l->clear();
        
        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            delete p;
        }
    }
    
    //sync instances
    void renderer_model::syncInstances( model_writelock *g )
    {
        std::list<renderer_model_instance *> *li, d;
        std::list<renderer_model_instance *>::iterator ii;
        std::list<model_instance_ref *> l;
        std::list<model_instance_ref *>::iterator i;
        renderer_model_instance *pi;
        model_instance_ref *p;
        dpid_bytetree t;
        shared_obj_guard o;
        model_instance_writelock *pl;
        
        //build index
        li = &this->instances;
        for( ii = li->begin(); ii != li->end(); ++ii )
        {
            pi = *ii;
            t.addLeaf( pi->getId(), pi );
        }
        
        //pair intances and sync them (or make them)
        g->getInstances( &l );
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            pl = (model_instance_writelock *)o.tryWriteLock( p, 100 );
            delete p;
            if( !pl )
                continue;
            pi = (renderer_model_instance *)t.findLeaf( pl->getId() );
            if( pi )
            {
                t.removeLeaf( pi );
                continue;
            }
            this->makeInstance( pl );
        }
        
        //find leaves in index not paired with a model_instance
        li = &this->instances;
        for( ii = li->begin(); ii != li->end(); ++ii )
        {
            pi = *ii;
            if( t.findLeaf( pi->getId() ) )
                d.push_back( pi );
        }
        
        //delete them
        li = &d;
        for( ii = li->begin(); ii != li->end(); ++ii )
        {
            pi = *ii;
            this->instances.remove( pi );
            delete pi;
        }        
    }
    
    //run instances
    void renderer_model::runInstances( dpthread_lock *thd )
    {
        std::list<renderer_model_instance *> *l, d;
        std::list<renderer_model_instance *>::iterator i;
        renderer_model_instance *p;
        renderer_model_instance_writelock *pl;
        shared_obj_guard o;
        
        //build index
        l = &this->instances;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (renderer_model_instance_writelock *)o.tryWriteLock( p, 100 );
            if( !pl )
                continue;
            pl->run( thd );
        }
        
    }
    
    //run model from task
    void renderer_model::run( dpthread_lock *thd, renderer_model_writelock *g, model_writelock *ml )
    {
        uint64_t t;
        
        t = thd->getTicks();
        
        if( !this->bIsSynced )
        {
            this->syncInstances( ml );
            this->onSync( thd, g, ml );
            this->bIsSynced = 1;
        }
        
        if( t - this->t_last_i_ran > 1000 )
        {
            this->t_last_i_ran = t;
            this->runInstances( thd );
        }
    }
    
    //handle sync
    void renderer_model::onSync( dpthread_lock *thd, renderer_model_writelock *g, model_writelock *ml )
    {
        
    }

    //generate instance
    renderer_model_instance *renderer_model::genInstance( model_instance_writelock *ml )
    {
        return new renderer_model_instance( ml );
    }
    
};
