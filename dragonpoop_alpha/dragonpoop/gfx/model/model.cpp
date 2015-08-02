
#include "model.h"
#include "model_task.h"
#include "model_readlock.h"
#include "model_writelock.h"
#include "model_ref.h"
#include "../../core/shared_obj/shared_obj_guard.h"
#include "../../core/core.h"
#include "../../core/dptask/dptask.h"
#include "../../core/dptask/dptask_writelock.h"
#include "../../core/dptaskpool/dptaskpool_writelock.h"
#include "model_vertex/model_vertex.h"
#include "model_triangle/model_triangle.h"
#include "model_group/model_group.h"

namespace dragonpoop
{

    //ctor
    model::model( core *c, dptaskpool_writelock *tp, dpid id ) : shared_obj( c->getMutexMaster() )
    {
        this->c = c;
        this->id = id;
        this->gtsk = new model_task( this );
        this->tsk = new dptask( c->getMutexMaster(), this->gtsk, 1000, 1 );
        tp->addTask( this->tsk );
    }

    //dtor
    model::~model( void )
    {
        this->kill();
        delete this->gtsk;
    }

    //return core
    core *model::getCore( void )
    {
        return this->c;
    }

    //generate read lock
    shared_obj_readlock *model::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new model_readlock( (model *)p, l );
    }

    //generate write lock
    shared_obj_writelock *model::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new model_writelock( (model *)p, l );
    }

    //generate ref
    shared_obj_ref *model::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new model_ref( (model *)p, k );
    }

    //stop model task
    void model::kill( void )
    {
        dptask_writelock *tl;
        shared_obj_guard o;

        if( !this->tsk )
            return;

        tl = (dptask_writelock *)o.writeLock( this->tsk );
        tl->kill();
        o.unlock();

        delete this->tsk;
        this->tsk = 0;
    }

    //run model from task
    void model::run( dpthread_lock *thd, model_writelock *g )
    {
    }

    //set name
    void model::setName( std::string *sname )
    {
        this->sname.assign( *sname );
    }

    //get name
    void model::getName( std::string *sname )
    {
        sname->assign( this->sname );
    }

    //compare name
    bool model::compareName( std::string *sname )
    {
        return this->sname.compare( *sname ) == 0;
    }

    //set id
    void model::setId( dpid id )
    {
        dpid_copy( &this->id, &id );
    }

    //get id
    dpid model::getId( void )
    {
        return this->id;
    }

    //compare id
    bool model::compareId( dpid id )
    {
        return dpid_compare( &this->id, &id );
    }

    //set comment
    void model::setComment( std::string *s )
    {
        this->scmmt.assign( *s );
    }

    //get comment
    void model::getComment( std::string *s )
    {
        s->assign( this->scmmt );
    }

    //append to comment
    void model::appendComment( std::string *s )
    {
        this->scmmt.append( *s );
    }

    //add component to list and trees
    void model::addComponent( model_component *c )
    {
        this->comps.lst.push_back( c );
       // this->comps.bytype.addLeaf( c->getType(), c );
        //this->comps.bytypeid.addLeaf( c->getType(), c->getId(), c );
    }
    
    //add component, 1 parent
    void model::addComponent( model_component *c, dpid p1 )
    {
        this->addComponent( c );
        //this->comps.bytypeowner.addLeaf( c->getType(), p1, c );
    }
    
    //add component, 2 parents
    void model::addComponent( model_component *c, dpid p1, dpid p2 )
    {
        this->addComponent( c, p1 );
        //this->comps.bytypeowner.addLeaf( c->getType(), p2, c );
    }
    
    //find component by type and id
    model_component *model::findComponent( uint16_t mtype, dpid id )
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
    void model::getComponents( uint16_t mtype, std::list<model_component *> *ll )
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
    void model::getComponentsByParent( uint16_t mtype, dpid p1, std::list<model_component *> *ll )
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
    void model::getComponentsByParents( uint16_t mtype, dpid p1, dpid p2, std::list<model_component *> *ll )
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
    void model::removeComponent( model_component *c )
    {
        this->comps.lst.remove( c );
    }
    
    //add vertex
    model_vertex *model::makeVertex( dpid id )
    {
        model_vertex *c;
        c = new model_vertex( id );
        this->addComponent( c );
        return c;
    }
    
    //find vertex
    model_vertex *model::findVertex( dpid id )
    {
        return (model_vertex *)this->findComponent( model_component_type_vertex, id );
    }
    
    //get vertexes
    void model::getVertexes( std::list<model_vertex *> *l )
    {
        this->getComponents( model_component_type_vertex, (std::list<model_component *> *)l );
    }

    //add triangle
    model_triangle *model::makeTriangle( dpid id )
    {
        model_triangle *c;
        c = new model_triangle( id );
        this->addComponent( c );
        return c;
    }
    
    //find triangle
    model_triangle *model::findTriangle( dpid id )
    {
        return (model_triangle *)this->findComponent( model_component_type_triangle, id );
    }
    
    //get triangles
    void model::getTriangles( std::list<model_triangle *> *l )
    {
        this->getComponents( model_component_type_triangle, (std::list<model_component *> *)l );
    }
    
    //add group
    model_group *model::makeGroup( dpid id )
    {
        model_group *c;
        c = new model_group( id );
        this->addComponent( c );
        return c;
    }
    
    //find group
    model_group *model::findGroup( dpid id )
    {
        return (model_group *)this->findComponent( model_component_type_group, id );
    }
    
    //get groups
    void model::getGroups( std::list<model_group *> *l )
    {
        this->getComponents( model_component_type_group, (std::list<model_component *> *)l );
    }
    
};
