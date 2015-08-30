
#include "renderer_model_instance.h"
#include "renderer_model_instance_ref.h"
#include "renderer_model_instance_readlock.h"
#include "renderer_model_instance_writelock.h"
#include "../../../gfx/model/model_instance/model_instance_writelock.h"
#include "../../../gfx/model/model_instance/model_instance_ref.h"
#include "../../../core/core.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include "../../../core/bytetree/dpid_bytetree.h"
#include "../../../gfx/model/model_instance/model_instance_group/model_instance_group.h"
#include "renderer_model_group_instance/renderer_model_group_instance.h"
#include "../../renderer_writelock.h"
#include "../renderer_model_material/renderer_model_material.h"
#include "../renderer_model_readlock.h"

namespace dragonpoop
{
    
    //ctor
    renderer_model_instance::renderer_model_instance( model_instance_writelock *ml ) : shared_obj( ml->getCore()->getMutexMaster() )
    {
        this->m = (model_instance_ref *)ml->getRef();
        this->id = ml->getId();
        this->bIsSynced = 0;
        ml->setRenderer( this );
    }
    
    //dtor
    renderer_model_instance::~renderer_model_instance( void )
    {
        this->deleteComponents();
        delete this->m;
    }
    
    //generate read lock
    shared_obj_readlock *renderer_model_instance::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new renderer_model_instance_readlock( (renderer_model_instance *)p, l );
    }
    
    //generate write lock
    shared_obj_writelock *renderer_model_instance::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new renderer_model_instance_writelock( (renderer_model_instance *)p, l );
    }
    
    //generate ref
    shared_obj_ref *renderer_model_instance::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new renderer_model_instance_ref( (renderer_model_instance *)p, k );
    }
    
    //delete all components
    void renderer_model_instance::deleteComponents( void )
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
    
    //get id
    dpid renderer_model_instance::getId( void )
    {
        return this->id;
    }
    
    //add component to list and trees
    void renderer_model_instance::addComponent( model_component *c )
    {
        uint16_t k;
        this->comps.lst.push_back( c );
        this->comps.byid.addLeaf( c->getId(), c );
        k = c->getType();
        this->comps.bytype.addLeaf( (char *)&k, sizeof( k ), c );
    }
    
    //find component by type and id
    model_component *renderer_model_instance::findComponent( uint16_t mtype, dpid id )
    {
        model_component *c;
        
        c = (model_component *)this->comps.byid.findLeaf( id );
        if( !c || c->getType() != mtype )
            return 0;
        
        return c;
    }
    
    //find components by type
    void renderer_model_instance::getComponents( uint16_t mtype, std::list<model_component *> *l )
    {
        this->comps.bytype.findLeaves( (char *)&mtype, sizeof( mtype ), (std::list<void *> *)l );
    }
    
    //find components by type and 1 parent
    void renderer_model_instance::getComponentsByParent( uint16_t mtype, dpid p1, std::list<model_component *> *ll )
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
    void renderer_model_instance::getComponentsByParents( uint16_t mtype, dpid p1, dpid p2, std::list<model_component *> *ll )
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
    void renderer_model_instance::removeComponent( model_component *c )
    {
        this->comps.lst.remove( c );
        this->comps.byid.removeLeaf( c );
        this->comps.byowner.removeLeaf( c );
        this->comps.bytype.removeLeaf( c );
    }
    
    //add group
    renderer_model_instance_group *renderer_model_instance::makeGroup( model_instance_writelock *ml, model_instance_group *g )
    {
        renderer_model_instance_group *c;
        
        c = this->genGroup( ml, g );
        this->addComponent( c );
        c->sync( ml, g );
        
        return c;
    }
    
    //find group
    renderer_model_instance_group *renderer_model_instance::findGroup( dpid id )
    {
        return (renderer_model_instance_group *)this->findComponent( model_component_type_group, id );
    }
    
    //get groups
    void renderer_model_instance::getGroups( std::list<renderer_model_instance_group *> *l )
    {
        this->getComponents( model_component_type_group, (std::list<model_component *> *)l );
    }
    
    //make groups
    void renderer_model_instance::makeGroups( model_instance_writelock *ml )
    {
        std::list<renderer_model_instance_group *> li, d;
        std::list<renderer_model_instance_group *>::iterator ii;
        std::list<model_instance_group *> l;
        std::list<model_instance_group *>::iterator i;
        renderer_model_instance_group *pi;
        model_instance_group *p;
        dpid_btree t;
        
        //build index
        this->getGroups( &li );
        for( ii = li.begin(); ii != li.end(); ++ii )
        {
            pi = *ii;
            t.addLeaf( pi->getId(), pi );
        }
        
        //pair intances and sync them (or make them)
        ml->getGroups( &l );
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            pi = (renderer_model_instance_group *)t.findLeaf( p->getId() );
            if( pi )
            {
                pi->sync( ml, p );
                t.removeLeaf( pi );
                continue;
            }
            this->makeGroup( ml, p );
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
    
    //animate groups
    void renderer_model_instance::syncGroups( model_instance_writelock *ml )
    {
        std::list<renderer_model_instance_group *> l;
        std::list<renderer_model_instance_group *>::iterator i;
        renderer_model_instance_group *p;
        std::list<model_instance_group *> lg;
        std::list<model_instance_group *>::iterator ig;
        model_instance_group *pg;
        dpid_btree t;

        ml->getGroups( &lg );
        for( ig = lg.begin(); ig != lg.end(); ++ig )
        {
            pg = *ig;
            t.addLeaf( pg->getId(), pg );
        }
        
        this->getGroups( &l );
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            pg = (model_instance_group *)t.findLeaf( p->getId() );
            if( pg )
                p->sync( ml, pg );
        }
    }
    
    //animate groups
    void renderer_model_instance::animateGroups( model_instance_writelock *ml )
    {
        std::list<renderer_model_instance_group *> l;
        std::list<renderer_model_instance_group *>::iterator i;
        renderer_model_instance_group *p;
        std::list<model_instance_group *> lg;
        std::list<model_instance_group *>::iterator ig;
        model_instance_group *pg;
        dpid_btree t;
        
        ml->getGroups( &lg );
        for( ig = lg.begin(); ig != lg.end(); ++ig )
        {
            pg = *ig;
            t.addLeaf( pg->getId(), pg );
        }
        
        this->getGroups( &l );
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            pg = (model_instance_group *)t.findLeaf( p->getId() );
            if( pg )
                p->animate( ml, pg );
        }
    }
    
    //run model from task
    void renderer_model_instance::run( dpthread_lock *thd, renderer_model_instance_writelock *g )
    {
        model_instance_writelock *ml;
        shared_obj_guard o;
        
        if( !this->bIsSynced )
        {
            ml = (model_instance_writelock *)o.tryWriteLock( this->m, 300 );
            if( ml )
            {
                this->makeGroups( ml );
                this->bIsSynced = 1;
                this->onSync( thd, g, ml );
                this->syncGroups( ml );
                this->syncJoints( ml );
            }
        }

        if( !this->bIsAnimated )
        {
            ml = (model_instance_writelock *)o.tryWriteLock( this->m, 300 );
            if( ml )
            {
                this->syncJoints( ml );
                this->animateGroups( ml );
                this->bIsAnimated = 1;
            }
        }
    }
    
    //sync
    void renderer_model_instance::sync( void )
    {
        this->bIsSynced = 0;
    }
    
    //animate
    void renderer_model_instance::animate( void )
    {
        this->bIsAnimated = 0;
    }
    
    //handle sync
    void renderer_model_instance::onSync( dpthread_lock *thd, renderer_model_instance_writelock *g, model_instance_writelock *ml )
    {
        
    }
    
    //genertae group
    renderer_model_instance_group *renderer_model_instance::genGroup( model_instance_writelock *ml, model_instance_group *g )
    {
        return new renderer_model_instance_group( ml, g );
    }

    //render model
    void renderer_model_instance::render( dpthread_lock *thd, renderer_writelock *r, renderer_model_readlock *m, renderer_model_instance_readlock *mi )
    {
        std::list<renderer_model_instance_group *> l;
        std::list<renderer_model_instance_group *>::iterator i;
        renderer_model_instance_group *g;
        renderer_model_material *mat;
        
        this->getGroups( &l );
        
        for( i = l.begin(); i != l.end(); ++i )
        {
            g = *i;
            mat = m->findMaterial( g->getMaterialId() );
            r->renderGroup( thd, m, mi, g, mat );
        }
    }
    
    //returns joint cache
    model_instance_joint_cache *renderer_model_instance::getJointCache( void )
    {
        return &this->jnts;
    }
    
    //sync joint cache
    void renderer_model_instance::syncJoints( model_instance_writelock *ml )
    {
        std::list<model_instance_joint *> l;
        std::list<model_instance_joint *>::iterator i;
        model_instance_joint *g;
        
        ml->getComponents( model_component_type_joint, (std::list<model_component *> *)&l );
        
        for( i = l.begin(); i != l.end(); ++i )
        {
            g = *i;
            this->jnts.addJoint( g );
        }
    }
    
};
