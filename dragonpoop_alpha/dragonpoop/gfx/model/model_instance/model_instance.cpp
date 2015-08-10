
#include "model_instance.h"
#include "model_instance_ref.h"
#include "model_instance_readlock.h"
#include "model_instance_writelock.h"
#include "../../../core/core.h"
#include "model_instance_animation/model_instance_animation.h"
#include "model_instance_group/model_instance_group.h"
#include "model_instance_triangle_vertex/model_instance_triangle_vertex.h"
#include "model_instance_vertex/model_instance_vertex.h"
#include "model_instance_triangle/model_instance_triangle.h"
#include "../model_writelock.h"
#include "../model_ref.h"
#include "../../../core/bytetree/dpid_bytetree.h"
#include "../../../core/bytetree/dpid_multibytetree.h"
#include "../../../renderer/renderer_model/renderer_model_instance/renderer_model_instance.h"
#include "../../../renderer/renderer_model/renderer_model_instance/renderer_model_instance_ref.h"
#include "../../../renderer/renderer_model/renderer_model_instance/renderer_model_instance_readlock.h"
#include "../../../renderer/renderer_model/renderer_model_instance/renderer_model_instance_writelock.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include "../../dpvertex/dpvertex.h"
#include "../../dpvertex/dpvertexindex_buffer.h"

namespace dragonpoop
{
    
    //ctor
    model_instance::model_instance( dpid id, model_writelock *ml ) : shared_obj( ml->getCore()->getMutexMaster() )
    {
        this->r = 0;
        this->id = id;
        this->c = ml->getCore();
        this->m = (model_ref *)ml->getRef();
        this->sync( ml );
    }
    
    //dtor
    model_instance::~model_instance( void )
    {
        this->deleteComponents();
        delete this->m;
        delete this->r;
    }
    
    //return core
    core *model_instance::getCore( void )
    {
        return this->c;
    }
    
    //delete all components
    void model_instance::deleteComponents( void )
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

    //generate read lock
    shared_obj_readlock *model_instance::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new model_instance_readlock( (model_instance *)p, l );
    }
    
    //generate write lock
    shared_obj_writelock *model_instance::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new model_instance_writelock( (model_instance *)p, l );
    }
    
    //generate ref
    shared_obj_ref *model_instance::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new model_instance_ref( (model_instance *)p, k );
    }
    
    //run model from task
    void model_instance::run( dpthread_lock *thd, model_instance_writelock *g )
    {
        
    }
    
    //get id
    dpid model_instance::getId( void )
    {
        return this->id;
    }
    
    //add component to list and trees
    void model_instance::addComponent( model_component *c )
    {
        this->comps.lst.push_back( c );
        // this->comps.bytype.addLeaf( c->getType(), c );
        //this->comps.bytypeid.addLeaf( c->getType(), c->getId(), c );
    }
    
    //add component, 1 parent
    void model_instance::addComponent( model_component *c, dpid p1 )
    {
        this->addComponent( c );
        //this->comps.bytypeowner.addLeaf( c->getType(), p1, c );
    }
    
    //add component, 2 parents
    void model_instance::addComponent( model_component *c, dpid p1, dpid p2 )
    {
        this->addComponent( c, p1 );
        //this->comps.bytypeowner.addLeaf( c->getType(), p2, c );
    }
    
    //find component by type and id
    model_component *model_instance::findComponent( uint16_t mtype, dpid id )
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
    void model_instance::getComponents( uint16_t mtype, std::list<model_component *> *ll )
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
    void model_instance::getComponentsByParent( uint16_t mtype, dpid p1, std::list<model_component *> *ll )
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
    void model_instance::getComponentsByParents( uint16_t mtype, dpid p1, dpid p2, std::list<model_component *> *ll )
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
    void model_instance::removeComponent( model_component *c )
    {
        this->comps.lst.remove( c );
    }
    
    //add vertex
    model_instance_vertex *model_instance::makeVertex( model_vertex *v )
    {
        model_instance_vertex *c;
        c = new model_instance_vertex( v );
        this->addComponent( c );
        return c;
    }
    
    //find vertex
    model_instance_vertex *model_instance::findVertex( dpid id )
    {
        return (model_instance_vertex *)this->findComponent( model_component_type_vertex, id );
    }
    
    //get vertexes
    void model_instance::getVertexes( std::list<model_instance_vertex *> *l )
    {
        this->getComponents( model_component_type_vertex, (std::list<model_component *> *)l );
    }
    
    //add group
    model_instance_group *model_instance::makeGroup( model_group *g )
    {
        model_instance_group *c;
        c = new model_instance_group( g );
        this->addComponent( c );
        return c;
    }
    
    //find group
    model_instance_group *model_instance::findGroup( dpid id )
    {
        return (model_instance_group *)this->findComponent( model_component_type_group, id );
    }
    
    //get groups
    void model_instance::getGroups( std::list<model_instance_group *> *l )
    {
        this->getComponents( model_component_type_group, (std::list<model_component *> *)l );
    }
    
    //add triangle vertex
    model_instance_triangle_vertex *model_instance::makeTriangleVertex( model_triangle_vertex *v )
    {
        model_instance_triangle_vertex *c;
        c = new model_instance_triangle_vertex( v );
        this->addComponent( c );//, triangle_id, vertex_id );
        return c;
    }
    
    //find triangle vertex
    model_instance_triangle_vertex *model_instance::findTriangleVertex( dpid id )
    {
        return (model_instance_triangle_vertex *)this->findComponent( model_component_type_triangle_vertex, id );
    }
    
    //find triangle vertex
    model_instance_triangle_vertex *model_instance::findTriangleVertex( dpid triangle_id, dpid vertex_id )
    {
        std::list<model_instance_triangle_vertex *> l;
        
        this->getComponentsByParents( model_component_type_triangle_vertex, triangle_id, vertex_id, (std::list<model_component *> *)&l );
        
        if( l.size() < 1 )
            return 0;
        return l.front();
    }
    
    //get triangle vertexes
    void model_instance::getTriangleVertexes( std::list<model_instance_triangle_vertex *> *l )
    {
        this->getComponents( model_component_type_triangle_vertex, (std::list<model_component *> *)l );
    }
    
    //get triangle vertexes by triangle or vertex id
    void model_instance::getTriangleVertexes( std::list<model_instance_triangle_vertex *> *l, dpid pid )
    {
        this->getComponentsByParent( model_component_type_triangle_vertex, pid, (std::list<model_component *> *)l );
    }
    
    //make verts
    void model_instance::makeVertexes( model_writelock *ml )
    {
        std::list<model_vertex *> l;
        std::list<model_vertex *>::iterator i;
        model_vertex *p;
        std::list<model_instance_vertex *> li;
        std::list<model_instance_vertex *>::iterator ii;
        model_instance_vertex *pi;
        dpid_bytetree t;
        
        this->getVertexes( &li );
        
        for( ii = li.begin(); ii != li.end(); ++ii )
        {
            pi = *ii;
            t.addLeaf( pi->getId(), pi );
        }
        
        ml->getVertexes( &l );
        
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            if( t.findLeaf( p->getId() ) )
                continue;
            this->makeVertex( p );
        }
    }
    
    //make triangle verts
    void model_instance::makeTriangleVertexes( model_writelock *ml )
    {
        std::list<model_triangle_vertex *> l;
        std::list<model_triangle_vertex *>::iterator i;
        model_triangle_vertex *p;
        std::list<model_instance_triangle_vertex *> li;
        std::list<model_instance_triangle_vertex *>::iterator ii;
        model_instance_triangle_vertex *pi;
        dpid_bytetree t;
        
        this->getTriangleVertexes( &li );
        
        for( ii = li.begin(); ii != li.end(); ++ii )
        {
            pi = *ii;
            t.addLeaf( pi->getId(), pi );
        }
        
        ml->getTriangleVertexes( &l );
        
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            if( t.findLeaf( p->getId() ) )
                continue;
            this->makeTriangleVertex( p );
        }
    }
    
    //make groups
    void model_instance::makeGroups( model_writelock *ml )
    {
        std::list<model_group *> l;
        std::list<model_group *>::iterator i;
        model_group *p;
        std::list<model_instance_group *> li;
        std::list<model_instance_group *>::iterator ii;
        model_instance_group *pi;
        dpid_bytetree t;
        
        this->getGroups( &li );
        
        for( ii = li.begin(); ii != li.end(); ++ii )
        {
            pi = *ii;
            t.addLeaf( pi->getId(), pi );
        }
        
        ml->getGroups( &l );
        
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            if( !t.findLeaf( p->getId() ) )
                this->makeGroup( p );
        }
    }
    
    //sync
    void model_instance::sync( model_writelock *ml )
    {
        shared_obj_guard o;
        renderer_model_instance_readlock *rl;

        this->makeVertexes( ml );
        this->makeTriangleVertexes( ml );
        this->makeTriangles( ml );
        this->makeGroups( ml );
        this->computeMeshes();
        
        if( !this->r )
            return;
        rl = (renderer_model_instance_readlock *)o.tryReadLock( this->r, 400 );
        if( !rl )
            return;
        rl->sync();
    }
 
    //set renderer model
    void model_instance::setRenderer( renderer_model_instance *r )
    {
        shared_obj_guard o;
        renderer_model_instance_writelock *rl;
        
        if( this->r )
            delete this->r;
        this->r = 0;
        
        rl = (renderer_model_instance_writelock *)o.tryWriteLock( r, 1000 );
        if( !rl )
            return;
        this->r = (renderer_model_instance_ref *)rl->getRef();
    }
    
    //add triangle
    model_instance_triangle *model_instance::makeTriangle( model_group_triangle *gt )
    {
        model_instance_triangle *c;
        c = new model_instance_triangle( gt );
        this->addComponent( c );//, triangle_id, vertex_id );
        return c;
    }
    
    //find triangle
    model_instance_triangle *model_instance::findTriangle( dpid id )
    {
        return (model_instance_triangle *)this->findComponent( model_component_type_triangle, id );
    }
    
    //find triangle
    model_instance_triangle *model_instance::findTriangle( dpid group_id, dpid triangle_id )
    {
        std::list<model_instance_triangle *> l;
        
        this->getComponentsByParents( model_component_type_triangle, group_id, triangle_id, (std::list<model_component *> *)&l );
        
        if( l.size() < 1 )
            return 0;
        return l.front();
    }
    
    //get triangles
    void model_instance::getTriangles( std::list<model_instance_triangle *> *l )
    {
        this->getComponents( model_component_type_triangle, (std::list<model_component *> *)l );
    }
    
    //get triangles by triangle or group id
    void model_instance::getTriangles( std::list<model_instance_triangle *> *l, dpid pid )
    {
        this->getComponentsByParent( model_component_type_triangle, pid, (std::list<model_component *> *)l );
    }

    //make triangles
    void model_instance::makeTriangles( model_writelock *ml )
    {
        std::list<model_group_triangle *> l;
        std::list<model_group_triangle *>::iterator i;
        model_group_triangle *p;
        std::list<model_instance_triangle *> li;
        std::list<model_instance_triangle *>::iterator ii;
        model_instance_triangle *pi;
        dpid_bytetree t;
        
        this->getTriangles( &li );
        
        for( ii = li.begin(); ii != li.end(); ++ii )
        {
            pi = *ii;
            t.addLeaf( pi->getId(), pi );
        }
        
        ml->getGroupTriangles( &l );
        
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            if( t.findLeaf( p->getId() ) )
                continue;
            this->makeTriangle( p );
        }
    }
    
    //populate vertex buffer for rendering
    void model_instance::fillVertexBuffer( dpid group_id, dpvertexindex_buffer *vb )
    {
        model_instance_group *g = this->findGroup( group_id );
        if( !g )
            return;
        vb->clear();
        vb->copy( g->getVertexBuffer() );
        vb->clearCache();
    }
    
    void model_instance__computeMeshes_0( dpid group_id, dpid_multibytetree *t_t, dpid_multibytetree *t_tv, dpid_bytetree *t_v, dpvertexindex_buffer *vb );
    
    //populate vertex buffer for rendering
    void model_instance::computeMeshes( void )
    {
        dpid_bytetree t_v;
        dpid_multibytetree t_tv, t_t;
        std::list<model_component *> *l;
        std::list<model_component *>::iterator i;
        model_component *p;
        model_instance_group *g;
        std::list<model_instance_group *> lg;
        std::list<model_instance_group *>::iterator it;
        dpvertexindex_buffer *vb;
        
        l = &this->comps.lst;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            switch( p->getType() )
            {
                case model_component_type_group:
                    lg.push_back( (model_instance_group *)p );
                    break;
                case model_component_type_triangle:
                    t_t.addLeaf( ( (model_instance_triangle *)p )->getGroupId(), p );
                    break;
                case model_component_type_triangle_vertex:
                    t_tv.addLeaf( ( (model_instance_triangle_vertex *)p )->getTriangleId(), p );
                    break;
                case model_component_type_vertex:
                    t_v.addLeaf( p->getId(), p );
                    break;
            }
        }
        
        for( it = lg.begin(); it != lg.end(); ++it )
        {
            g = *it;
            vb = g->getVertexBuffer();
            vb->clear();
            model_instance__computeMeshes_0( g->getId(), &t_t, &t_tv, &t_v, vb );
            vb->clearCache();
        }
        
    }
    
    void model_instance__computeMeshes_1( model_instance_triangle *t, dpid_multibytetree *t_tv, dpid_bytetree *t_v, dpvertexindex_buffer *vb );
    
    void model_instance__computeMeshes_0( dpid group_id, dpid_multibytetree *t_t, dpid_multibytetree *t_tv, dpid_bytetree *t_v, dpvertexindex_buffer *vb )
    {
        std::list<model_instance_triangle *> lt;
        std::list<model_instance_triangle *>::iterator it;
        model_instance_triangle *p;
        
        t_t->findLeaves( group_id, (std::list<void *> *)&lt );
        
        for( it = lt.begin(); it != lt.end(); ++it )
        {
            p = *it;
            model_instance__computeMeshes_1( p, t_tv, t_v, vb );
        }
    }

    void model_instance__computeMeshes_2( model_instance_triangle *t, model_instance_triangle_vertex *tv, dpid_bytetree *v, dpvertexindex_buffer *vb );
    
    void model_instance__computeMeshes_1( model_instance_triangle *t, dpid_multibytetree *t_tv, dpid_bytetree *t_v, dpvertexindex_buffer *vb )
    {
        std::list<model_instance_triangle_vertex *> ltv;
        std::list<model_instance_triangle_vertex *>::iterator it;
        model_instance_triangle_vertex *p;

        t_tv->findLeaves( t->getTriangleId(), (std::list<void *> *)&ltv );
        
        if( ltv.size() != 3 )
            return;
        
        for( it = ltv.begin(); it != ltv.end(); ++it )
        {
            p = *it;
            model_instance__computeMeshes_2( t, p, t_v, vb );
        }
    }

    void model_instance__computeMeshes_2( model_instance_triangle *t, model_instance_triangle_vertex *tv, dpid_bytetree *v, dpvertexindex_buffer *vb )
    {
        model_instance_vertex *p;
        dpvertex vt;
        
        p = (model_instance_vertex *)v->findLeaf( tv->getVertexId() );
        if( !p )
            return;
        
        p->getPosition( &vt.start.pos );
        tv->getNormal( &vt.start.normal );
        tv->getTexCoord0( &vt.start.texcoords[ 0 ] );
        tv->getTexCoord1( &vt.start.texcoords[ 1 ] );

        vt.end = vt.start;
        
        vb->addVertex( &vt, tv->getId() );
    }
    
};
