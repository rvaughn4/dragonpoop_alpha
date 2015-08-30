
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
#include "model_triangle_vertex/model_triangle_vertex.h"
#include "model_group_triangle/model_group_triangle.h"
#include "model_material/model_material.h"
#include "model_instance/model_instance.h"
#include "model_instance/model_instance_ref.h"
#include "model_instance/model_instance_writelock.h"
#include "model_instance/model_instance_readlock.h"
#include "../../renderer/renderer_model/renderer_model.h"
#include "../../renderer/renderer_model/renderer_model_ref.h"
#include "../../renderer/renderer_model/renderer_model_readlock.h"
#include "../../renderer/renderer_model/renderer_model_writelock.h"
#include "model_animation/model_animation.h"
#include "model_joint/model_joint.h"
#include "model_vertex_joint/model_vertex_joint.h"
#include "model_frame/model_frame.h"
#include "model_animation_frame/model_animation_frame.h"
#include "model_frame_joint/model_frame_joint.h"
#include "../../core/dpthread/dpthread_lock.h"

namespace dragonpoop
{

    //ctor
    model::model( core *c, dptaskpool_writelock *tp, dpid id ) : shared_obj( c->getMutexMaster() )
    {
        this->c = c;
        this->id = id;
        this->r = 0;
        this->gtsk = new model_task( this );
        this->tsk = new dptask( c->getMutexMaster(), this->gtsk, 200, 1 );
        tp->addTask( this->tsk );
    }

    //dtor
    model::~model( void )
    {
        this->kill();
        delete this->gtsk;
        this->deleteInstances();
        this->deleteComponents();
        delete this->r;
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
        this->ran_time = thd->getTicks();
        this->runInstances( thd, g );
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
        uint16_t k;
        this->comps.lst.push_back( c );
        k = c->getType();
        this->comps.bytype.addLeaf( (char *)&k, sizeof( k ), c );
        this->comps.byid.addLeaf( c->getId(), c );
    }

    //add component, 1 parent
    void model::addComponent( model_component *c, dpid p1 )
    {
        this->addComponent( c );
        this->comps.byowner.addLeaf( p1, c );
    }

    //add component, 2 parents
    void model::addComponent( model_component *c, dpid p1, dpid p2 )
    {
        this->addComponent( c, p1 );
        this->comps.byowner.addLeaf( p2, c );
    }

    //delete all components
    void model::deleteComponents( void )
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
        this->comps.bytype.clear();
        this->comps.byowner.clear();

        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            c = *i;
            delete c;
        }
    }

    //find component by type and id
    model_component *model::findComponent( uint16_t mtype, dpid id )
    {
        model_component *c;

        c = (model_component *)this->comps.byid.findLeaf( id );
        
        if( !c || c->getType() != mtype )
            return 0;
        
        return c;
    }

    //find components by type
    void model::getComponents( uint16_t mtype, std::list<model_component *> *l )
    {
        this->comps.bytype.findLeaves( (char *)&mtype, sizeof( mtype ), (std::list<void *> *)l );
    }

    //find components by type and 1 parent
    void model::getComponentsByParent( uint16_t mtype, dpid p1, std::list<model_component *> *ll )
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
    void model::getComponentsByParents( uint16_t mtype, dpid p1, dpid p2, std::list<model_component *> *ll )
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
    void model::removeComponent( model_component *c )
    {
        this->comps.lst.remove( c );
        this->comps.byid.removeLeaf( c );
        this->comps.byowner.removeLeaf( c );
        this->comps.bytype.removeLeaf( c );
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

    //add triangle vertex
    model_triangle_vertex *model::makeTriangleVertex( dpid id, dpid triangle_id, dpid vertex_id )
    {
        model_triangle_vertex *c;
        c = new model_triangle_vertex( id, triangle_id, vertex_id );
        this->addComponent( c, triangle_id, vertex_id );
        return c;
    }

    //find triangle vertex
    model_triangle_vertex *model::findTriangleVertex( dpid id )
    {
        return (model_triangle_vertex *)this->findComponent( model_component_type_triangle_vertex, id );
    }

    //find triangle vertex
    model_triangle_vertex *model::findTriangleVertex( dpid triangle_id, dpid vertex_id )
    {
        std::list<model_triangle_vertex *> l;

        this->getComponentsByParents( model_component_type_triangle_vertex, triangle_id, vertex_id, (std::list<model_component *> *)&l );

        if( l.size() < 1 )
            return 0;
        return l.front();
    }

    //get triangle vertexes
    void model::getTriangleVertexes( std::list<model_triangle_vertex *> *l )
    {
        this->getComponents( model_component_type_triangle_vertex, (std::list<model_component *> *)l );
    }

    //get triangle vertexes by triangle or vertex id
    void model::getTriangleVertexes( std::list<model_triangle_vertex *> *l, dpid pid )
    {
        this->getComponentsByParent( model_component_type_triangle_vertex, pid, (std::list<model_component *> *)l );
    }

    //add a group triangle
    model_group_triangle *model::makeGroupTriangle( dpid id, dpid group_id, dpid triangle_id )
    {
        model_group_triangle *c;
        c = new model_group_triangle( id, group_id, triangle_id );
        this->addComponent( c, group_id, triangle_id );
        return c;
    }

    //find a group triangle
    model_group_triangle *model::findGroupTriangle( dpid id )
    {
        return (model_group_triangle *)this->findComponent( model_component_type_group_triangle, id );
    }

    //find a group triangle
    model_group_triangle *model::findGroupTriangle( dpid group_id, dpid triangle_id )
    {
        std::list<model_group_triangle *> l;

        this->getComponentsByParents( model_component_type_group_triangle, group_id, triangle_id, (std::list<model_component *> *)&l );

        if( l.size() < 1 )
            return 0;
        return l.front();
    }

    //get all group triangles
    void model::getGroupTriangles( std::list<model_group_triangle *> *l )
    {
        this->getComponents( model_component_type_group_triangle, (std::list<model_component *> *)l );
    }

    //get all group triangles belonging to a group or triangle id
    void model::getGroupTriangles( std::list<model_group_triangle *> *l, dpid pid )
    {
        this->getComponentsByParent( model_component_type_group_triangle, pid, (std::list<model_component *> *)l );
    }

    //delete instances
    void model::deleteInstances( void )
    {
        std::list<model_instance *> *l, d;
        std::list<model_instance *>::iterator i;
        model_instance *p;

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

    //run instances
    void model::runInstances( dpthread_lock *thd, model_writelock *g )
    {
        std::list<model_instance *> *l, d;
        std::list<model_instance *>::iterator i;
        model_instance *p;
        model_instance_writelock *pl;
        shared_obj_guard o;

        l = &this->instances;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = ( model_instance_writelock *)o.tryWriteLock( p, 10 );
            if( !pl )
                continue;
            pl->run( thd, g );
            //
            //d.push_back( p );
        }

        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            delete p;
        }
    }

    //sync instances
    void model::syncInstances( dpthread_lock *thd, model_writelock *g )
    {
        std::list<model_instance *> *l;
        std::list<model_instance *>::iterator i;
        model_instance *p;
        model_instance_writelock *pl;
        shared_obj_guard o;

        l = &this->instances;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = ( model_instance_writelock *)o.tryReadLock( p, 300 );
            if( !pl )
                continue;
            pl->sync();
            pl->run( thd, g );
        }
    }

    //create instance
    model_instance_ref *model::makeInstance( dpid id, model_writelock *ml )
    {
        model_instance *p;
        model_instance_writelock *pl;
        shared_obj_guard o;

        p = new model_instance( id, ml );
        if( !p )
            return 0;
        this->instances.push_back( p );
        pl = (model_instance_writelock *)o.writeLock( p );
        if( !pl )
            return 0;
        return (model_instance_ref *)pl->getRef();
    }

    //get instances
    void model::getInstances( std::list<model_instance_ref *> *ll )
    {
        std::list<model_instance *> *l;
        std::list<model_instance *>::iterator i;
        model_instance *p;
        model_instance_writelock *pl;
        shared_obj_guard o;

        l = &this->instances;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (model_instance_writelock *)o.tryWriteLock( p, 100 );
            if( !pl )
                continue;
            ll->push_back( (model_instance_ref *)pl->getRef() );
        }
    }

    //sync model instance with changes
    void model::sync( dpthread_lock *thd, model_writelock *ml )
    {
        shared_obj_guard o;
        renderer_model_readlock *rl;

        this->syncInstances( thd, ml );

        if( !this->r )
            return;
        rl = (renderer_model_readlock *)o.tryReadLock( this->r, 400 );
        if( !rl )
            return;
        rl->sync();
    }

    //set renderer model
    void model::setRenderer( renderer_model *r )
    {
        shared_obj_guard o;
        renderer_model_writelock *rl;

        if( this->r )
            delete this->r;
        this->r = 0;

        rl = (renderer_model_writelock *)o.tryWriteLock( r, 1000 );
        if( !rl )
            return;
        this->r = (renderer_model_ref *)rl->getRef();
    }

    //add material
    model_material *model::makeMaterial( dpid id )
    {
        model_material *c;
        c = new model_material( id );
        this->addComponent( c );
        return c;
    }

    //find material
    model_material *model::findMaterial( dpid id )
    {
        return (model_material *)this->findComponent( model_component_type_material, id );
    }

    //get materials
    void model::getMaterials( std::list<model_material *> *l )
    {
        this->getComponents( model_component_type_material, (std::list<model_component *> *)l );
    }

    //add animation
    model_animation *model::makeAnimation( dpid id )
    {
        model_animation *c;
        c = new model_animation( id );
        this->addComponent( c );
        return c;
    }

    //find animation
    model_animation *model::findAnimation( dpid id )
    {
        return (model_animation *)this->findComponent( model_component_type_animation, id );
    }

    //get animations
    void model::getAnimations( std::list<model_animation *> *l )
    {
        this->getComponents( model_component_type_animation, (std::list<model_component *> *)l );
    }

    //add joint
    model_joint *model::makeJoint( dpid id )
    {
        model_joint *c;
        c = new model_joint( id );
        this->addComponent( c );
        return c;
    }

    //find joint
    model_joint *model::findJoint( dpid id )
    {
        return (model_joint *)this->findComponent( model_component_type_joint, id );
    }

    //get joints
    void model::getJoints( std::list<model_joint *> *l )
    {
        this->getComponents( model_component_type_joint, (std::list<model_component *> *)l );
    }

    //add vertex joint
    model_vertex_joint *model::makeVertexJoint( dpid id, dpid vertex_id, dpid joint_id, float w )
    {
        model_vertex_joint *c;
        c = new model_vertex_joint( id, vertex_id, joint_id );
        c->setWeight( w );
        this->addComponent( c, vertex_id, joint_id );
        return c;
    }

    //find vertex joint
    model_vertex_joint *model::findVertexJoint( dpid id )
    {
        return (model_vertex_joint *)this->findComponent( model_component_type_vertex_joint, id );
    }

    //get vertex joints
    void model::getVertexJoints( std::list<model_vertex_joint *> *l )
    {
        this->getComponents( model_component_type_vertex_joint, (std::list<model_component *> *)l );
    }

    //get vertex joints
    void model::getVertexJoints( std::list<model_vertex_joint *> *l, dpid p1 )
    {
        this->getComponentsByParent( model_component_type_vertex_joint, p1, (std::list<model_component *> *)l );
    }
    
    //get vertex joints
    void model::getVertexJoints( std::list<model_vertex_joint *> *l, dpid p1, dpid p2 )
    {
        this->getComponentsByParents( model_component_type_vertex_joint, p1, p2, (std::list<model_component *> *)l );
    }

    //add frame
    model_frame *model::makeFrame( dpid id )
    {
        model_frame *c;
        c = new model_frame( id );
        this->addComponent( c );
        return c;
    }
    
    //find frame
    model_frame *model::findFrame( dpid id )
    {
        return (model_frame *)this->findComponent( model_component_type_frame, id );
    }
    
    //get frame
    void model::getFrames( std::list<model_frame *> *l )
    {
        this->getComponents( model_component_type_frame, (std::list<model_component *> *)l );
    }
    
    //add animation frame
    model_animation_frame *model::makeAnimationFrame( dpid id, dpid animation_id, dpid frame_id, unsigned int time_ms )
    {
        model_animation_frame *c;
        c = new model_animation_frame( id, animation_id, frame_id );
        c->setTime( time_ms );
        this->addComponent( c, animation_id, frame_id );
        return c;
    }
    
    //find animation frame
    model_animation_frame *model::findAnimationFrame( dpid id )
    {
        return (model_animation_frame *)this->findComponent( model_component_type_animation_frame, id );
    }
    
    //get animation frame
    void model::getAnimationFrames( std::list<model_animation_frame *> *l )
    {
        this->getComponents( model_component_type_animation_frame, (std::list<model_component *> *)l );
    }
    
    //get animation frame by frame or animation id
    void model::getAnimationFrames( std::list<model_animation_frame *> *l, dpid parent_id )
    {
        this->getComponentsByParent( model_component_type_animation_frame, parent_id, (std::list<model_component *> *)l );
    }
    
    //add FrameJoint
    model_frame_joint *model::makeFrameJoint( dpid id, dpid frame_id, dpid joint_id )
    {
        model_frame_joint *c;
        c = new model_frame_joint( id, frame_id, joint_id );
        this->addComponent( c, frame_id, joint_id );
        return c;
    }
    
    //find FrameJoint
    model_frame_joint *model::findFrameJoint( dpid id )
    {
        return (model_frame_joint *)this->findComponent( model_component_type_frame_joint, id );
    }
    
    //get FrameJoints
    void model::getFrameJoints( std::list<model_frame_joint *> *l )
    {
        this->getComponents( model_component_type_frame_joint, (std::list<model_component *> *)l );
    }
    
    //get FrameJoints by frame or animation id
    void model::getFrameJoints( std::list<model_frame_joint *> *l, dpid parent_id )
    {
        this->getComponentsByParent( model_component_type_frame_joint, parent_id, (std::list<model_component *> *)l );
    }
    
};
