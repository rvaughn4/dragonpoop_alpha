
#include "renderer_model.h"
#include "renderer_model_ref.h"
#include "renderer_model_readlock.h"
#include "renderer_model_writelock.h"
#include "../../gfx/model/model_writelock.h"
#include "../../gfx/model/model_ref.h"
#include "../../core/core.h"
#include "renderer_model_instance/renderer_model_instance.h"
#include "renderer_model_instance/renderer_model_instance_writelock.h"
#include "renderer_model_instance/renderer_model_instance_readlock.h"
#include "../../core/bytetree/dpid_bytetree.h"
#include "../../gfx/model/model_instance/model_instance_writelock.h"
#include "../../gfx/model/model_instance/model_instance_ref.h"
#include "../../core/shared_obj/shared_obj_guard.h"
#include "../../core/dpthread/dpthread_lock.h"
#include "renderer_model_material/renderer_model_material.h"
#include "../../gfx/model/model_material/model_material.h"

namespace dragonpoop
{
    
    //ctor
    renderer_model::renderer_model( model_writelock *ml ) : shared_obj( ml->getCore()->getMutexMaster() )
    {
        this->m = (model_ref *)ml->getRef();
        this->bIsSynced = 0;
        this->bIsRenSynced = 0;
        this->bIsAlive = 1;
        this->id = ml->getId();
        ml->setRenderer( this );
    }
    
    //dtor
    renderer_model::~renderer_model( void )
    {
        shared_obj_guard o;
        
        o.tryWriteLock( this, 5000, "renderer_model::~renderer_model" );
        o.unlock();
        this->unlink();
        
        o.tryWriteLock( this, 5000, "renderer_model::~renderer_model" );
        this->deleteInstances();
        this->deleteComponents();
        delete this->m;
        o.unlock();
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
        uint16_t k;
        this->comps.lst.push_back( c );
        this->comps.byid.addLeaf( c->getId(), c );
        k = c->getType();
        this->comps.bytype.addLeaf( (char *)&k, sizeof( k ), c );
    }
    
    //find component by type and id
    model_component *renderer_model::findComponent( uint16_t mtype, dpid id )
    {
        model_component *c;
        
        c = (model_component *)this->comps.byid.findLeaf( id );
        if( !c || c->getType() != mtype )
            return 0;

        return c;
    }
    
    //find components by type
    void renderer_model::getComponents( uint16_t mtype, std::list<model_component *> *l )
    {
        this->comps.bytype.findLeaves( (char *)&mtype, sizeof( mtype ), (std::list<void *> *)l );
    }
    
    //find components by type and 1 parent
    void renderer_model::getComponentsByParent( uint16_t mtype, dpid p1, std::list<model_component *> *ll )
    {
        std::list<model_component *> l;
        std::list<model_component *>::iterator i;
        model_component *c;
        
        this->comps.byowner.findLeaves( p1, (std::list<void *> *)&l );
        
        for( i = l.begin(); i != l.end(); ++i )
        {
            c = *i;
            if( c->getType() != mtype )
                continue;
            ll->push_back( c );
        }        
    }
    
    //find components by type and 2 parents
    void renderer_model::getComponentsByParents( uint16_t mtype, dpid p1, dpid p2, std::list<model_component *> *ll )
    {
        std::list<model_component *> l;
        std::list<model_component *>::iterator i;
        model_component *c;
        
        this->comps.byowner.findLeaves( p1, (std::list<void *> *)&l );
        
        for( i = l.begin(); i != l.end(); ++i )
        {
            c = *i;
            if( c->getType() != mtype )
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
        this->comps.byid.removeLeaf( c );
        this->comps.byowner.removeLeaf( c );
        this->comps.bytype.removeLeaf( c );
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
        this->comps.byid.clear();
        this->comps.byowner.clear();
        this->comps.bytype.clear();

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
        renderer_model_instance_writelock *pil;
        model_instance_ref *p;
        dpid_btree t;
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
            pl = (model_instance_writelock *)o.tryWriteLock( p, 10, "renderer_model::syncInstances" );
            delete p;
            if( !pl || !pl->isAlive() )
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
            pil = (renderer_model_instance_writelock *)o.tryWriteLock( pi, 100, "renderer_model::syncInstances" );
            if( !pil )
                continue;
            pil->kill();
            o.unlock();
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
            pl = (renderer_model_instance_writelock *)o.tryWriteLock( p, 10, "renderer_model::runInstances" );
            if( !pl )
                continue;
            pl->run( thd );
            if( !pl->isAlive() )
                d.push_back( p );
        }
        o.unlock();
        
        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            this->instances.remove( p );
            delete p;
        }
    }
    
    //render model
    void renderer_model::render( dpthread_lock *thd, dpposition *campos, renderer_model_readlock *m, dpmatrix *m_world, render_api_context_writelock *ctx, render_api_commandlist_writelock *clist  )
    {
        std::list<renderer_model_instance *> *l;
        std::list<renderer_model_instance *>::iterator i;
        renderer_model_instance *p;
        renderer_model_instance_readlock *pl;
        shared_obj_guard o;
        
        l = &this->instances;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (renderer_model_instance_readlock *)o.tryReadLock( p, 3, "renderer_model::render" );
            if( !pl )
                continue;
            pl->render( thd, campos, m, m_world, ctx, clist );
        }
    }
    
    //handle sync
    void renderer_model::onSync( dpthread_lock *thd, model_writelock *ml )
    {
        
    }

    //generate instance
    renderer_model_instance *renderer_model::genInstance( model_instance_writelock *ml )
    {
        return new renderer_model_instance( ml );
    }
    
    //sync materials
    void renderer_model::syncMaterials( model_writelock *ml, render_api_context_writelock *cl )
    {
        std::list<renderer_model_material *> li, d;
        std::list<renderer_model_material *>::iterator ii;
        std::list<model_material *> l;
        std::list<model_material *>::iterator i;
        renderer_model_material *pi;
        model_material *p;
        dpid_btree t;
        
        //build index
        this->getMaterials( &li );
        for( ii = li.begin(); ii != li.end(); ++ii )
        {
            pi = *ii;
            t.addLeaf( pi->getId(), pi );
        }
        
        //pair intances and sync them (or make them)
        ml->getMaterials( &l );
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            pi = (renderer_model_material *)t.findLeaf( p->getId() );
            if( pi )
            {
                pi->sync( ml, p, cl );
                t.removeLeaf( pi );
                continue;
            }
            this->makeMaterial( ml, p, cl );
        }
        
        //find leaves in index not paired with a model_instance
        for( ii = li.begin(); ii != li.end(); ++ii )
        {
            pi = *ii;
            if( t.findLeaf( pi->getId() ) )
                d.push_back( pi );
        }
        
        //delete them
        for( ii = d.begin(); ii != d.end(); ++ii )
        {
            pi = *ii;
            this->removeComponent( pi );
            delete pi;
        }
    }
    
    //generate material
    renderer_model_material *renderer_model::genMaterial( model_writelock *ml, model_material *m, render_api_context_writelock *cl )
    {
        return new renderer_model_material( ml, m, cl );
    }

    //add material
    renderer_model_material *renderer_model::makeMaterial( model_writelock *ml, model_material *m, render_api_context_writelock *cl )
    {
        renderer_model_material *c;
        c = this->genMaterial( ml, m, cl );
        if( !c )
            return 0;
        this->addComponent( c );
        return c;
    }
    
    //find material
    renderer_model_material *renderer_model::findMaterial( dpid id )
    {
        return (renderer_model_material *)this->findComponent( model_component_type_material, id );
    }
    
    //get materials
    void renderer_model::getMaterials( std::list<renderer_model_material *> *l )
    {
        this->getComponents( model_component_type_material, (std::list<model_component *> *)l );
    }
    
    //get size
    void renderer_model::getSize( dpxyz_f *x )
    {
        *x = this->size;
    }

    //get center
    void renderer_model::getCenter( dpxyz_f *x )
    {
        *x = this->center;
    }

    //returns true if alive
    bool renderer_model::isAlive( void )
    {
        return this->bIsAlive;
    }
    
    //kill model
    void renderer_model::kill( void )
    {
        this->bIsAlive = 0;
    }
    
    //run model from renderer
    void renderer_model::run( dpthread_lock *thd, render_api_context_writelock *ctx )
    {
        model_writelock *ml;
        shared_obj_guard o;
        
        if( !this->bIsSynced && this->m )
        {
            ml = (model_writelock *)o.tryWriteLock( this->m, 100, "renderer_model::run" );
            if( ml )
            {
                ml->getSize( &this->size );
                ml->getCenter( &this->center );
                this->syncMaterials( ml, ctx );
                this->syncInstances( ml );
                this->onSync( thd, ml );
                this->bIsSynced = 1;
            }
        }
        
        this->runInstances( thd );
    }
    
};
