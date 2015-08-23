
#include "model_writelock.h"
#include "model.h"

namespace dragonpoop
{

    //ctor
    model_writelock::model_writelock( model *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }

    //dtor
    model_writelock::~model_writelock( void )
    {

    }

    //return core
    core *model_writelock::getCore( void )
    {
        return this->t->getCore();
    }

    //run model from task
    void model_writelock::run( dpthread_lock *thd )
    {
        this->t->run( thd, this );
    }

    //set name
    void model_writelock::setName( std::string *sname )
    {
        this->t->setName( sname );
    }

    //get name
    void model_writelock::getName( std::string *sname )
    {
        this->t->getName( sname );
    }

    //compare name
    bool model_writelock::compareName( std::string *sname )
    {
        return this->t->compareName( sname );
    }

    //set id
    void model_writelock::setId( dpid id )
    {
        this->t->setId( id );
    }

    //get id
    dpid model_writelock::getId( void )
    {
        return this->t->getId();
    }

    //compare id
    bool model_writelock::compareId( dpid id )
    {
        return this->t->compareId( id );
    }

    //set comment
    void model_writelock::setComment( std::string *s )
    {
        this->t->setComment( s );
    }

    //get comment
    void model_writelock::getComment( std::string *s )
    {
        this->t->getComment( s );
    }

    //append to comment
    void model_writelock::appendComment( std::string *s )
    {
        this->t->appendComment( s );
    }

    
    //add component to list and trees
    void model_writelock::addComponent( model_component *c )
    {
        this->t->addComponent( c );
    }
    
    //add component, 1 parent
    void model_writelock::addComponent( model_component *c, dpid p1 )
    {
        this->t->addComponent( c, p1 );
    }
    
    //add component, 2 parents
    void model_writelock::addComponent( model_component *c, dpid p1, dpid p2 )
    {
        this->t->addComponent( c, p1, p2 );
    }
    
    //find component by type and id
    model_component *model_writelock::findComponent( uint16_t mtype, dpid id )
    {
        return this->t->findComponent( mtype, id );
    }
    
    //find components by type
    void model_writelock::getComponents( uint16_t mtype, std::list<model_component *> *l )
    {
        this->t->getComponents( mtype, l );
    }
    
    //find components by type and 1 parent
    void model_writelock::getComponentsByParent( uint16_t mtype, dpid p1, std::list<model_component *> *l )
    {
        this->t->getComponentsByParent( mtype, p1, l );
    }
    
    //find components by type and 2 parents
    void model_writelock::getComponentsByParents( uint16_t mtype, dpid p1, dpid p2, std::list<model_component *> *l )
    {
        this->t->getComponentsByParents( mtype, p1, p2, l );
    }
    
    //remove component
    void model_writelock::removeComponent( model_component *c )
    {
        this->t->removeComponent( c );
    }
    
    //add vertex
    model_vertex *model_writelock::makeVertex( dpid id )
    {
        return this->t->makeVertex( id );
    }
    
    //find vertex
    model_vertex *model_writelock::findVertex( dpid id )
    {
        return this->t->findVertex( id );
    }
    
    //get vertexes
    void model_writelock::getVertexes( std::list<model_vertex *> *l )
    {
        this->t->getVertexes( l );
    }
    
    //add triangle
    model_triangle *model_writelock::makeTriangle( dpid id )
    {
        return this->t->makeTriangle( id );
    }
    
    //find triangle
    model_triangle *model_writelock::findTriangle( dpid id )
    {
        return this->findTriangle( id );
    }
    
    //get triangles
    void model_writelock::getTriangles( std::list<model_triangle *> *l )
    {
        this->t->getTriangles( l );
    }
    
    //add group
    model_group *model_writelock::makeGroup( dpid id )
    {
        return this->t->makeGroup( id );
    }
    
    //find group
    model_group *model_writelock::findGroup( dpid id )
    {
        return this->t->findGroup( id );
    }
    
    //get groups
    void model_writelock::getGroups( std::list<model_group *> *l )
    {
        this->t->getGroups( l );
    }
    
    //add triangle vertex
    model_triangle_vertex *model_writelock::makeTriangleVertex( dpid id, dpid triangle_id, dpid vertex_id )
    {
        return this->t->makeTriangleVertex( id, triangle_id, vertex_id );
    }
    
    //find triangle vertex
    model_triangle_vertex *model_writelock::findTriangleVertex( dpid id )
    {
        return this->t->findTriangleVertex( id );
    }
    
    //find triangle vertex
    model_triangle_vertex *model_writelock::findTriangleVertex( dpid triangle_id, dpid vertex_id )
    {
        return this->t->findTriangleVertex( triangle_id, vertex_id );
    }
    
    //get triangle vertexes
    void model_writelock::getTriangleVertexes( std::list<model_triangle_vertex *> *l )
    {
        this->t->getTriangleVertexes( l );
    }
    
    //get triangle vertexes by triangle or vertex id
    void model_writelock::getTriangleVertexes( std::list<model_triangle_vertex *> *l, dpid pid )
    {
        this->t->getTriangleVertexes( l, pid );
    }
    
    //add a group triangle
    model_group_triangle *model_writelock::makeGroupTriangle( dpid id, dpid group_id, dpid triangle_id )
    {
        return this->t->makeGroupTriangle( id, group_id, triangle_id );
    }
    
    //find a group triangle
    model_group_triangle *model_writelock::findGroupTriangle( dpid id )
    {
        return this->t->findGroupTriangle( id );
    }
    
    //find a group triangle
    model_group_triangle *model_writelock::findGroupTriangle( dpid group_id, dpid triangle_id )
    {
        return this->t->findGroupTriangle( group_id, triangle_id );
    }
    
    //get all group triangles
    void model_writelock::getGroupTriangles( std::list<model_group_triangle *> *l )
    {
        this->t->getGroupTriangles( l );
    }
    
    //get all group triangles belonging to a group or triangle id
    void model_writelock::getGroupTriangles( std::list<model_group_triangle *> *l, dpid pid )
    {
        this->t->getGroupTriangles( l, pid );
    }
    
    //create instance
    model_instance_ref *model_writelock::makeInstance( dpid id )
    {
        return this->t->makeInstance( id, this );
    }
    
    //get instances
    void model_writelock::getInstances( std::list<model_instance_ref *> *l )
    {
        this->t->getInstances( l );
    }

    //sync model instance with changes
    void model_writelock::sync( void )
    {
        this->t->sync( this );
    }
    
    //set renderer model
    void model_writelock::setRenderer( renderer_model *r )
    {
        this->t->setRenderer( r );
    }

    //add material
    model_material *model_writelock::makeMaterial( dpid id )
    {
        return this->t->makeMaterial( id );
    }
    
    //find material
    model_material *model_writelock::findMaterial( dpid id )
    {
        return this->t->findMaterial( id );
    }
    
    //get materials
    void model_writelock::getMaterials( std::list<model_material *> *l )
    {
        this->t->getMaterials( l );
    }
    
    //add animation
    model_animation *model_writelock::makeAnimation( dpid id )
    {
        return this->t->makeAnimation( id );
    }
    
    //find animation
    model_animation *model_writelock::findAnimation( dpid id )
    {
        return this->t->findAnimation( id );
    }
    
    //get animations
    void model_writelock::getAnimations( std::list<model_animation *> *l )
    {
        this->t->getAnimations( l );
    }
    
    //add joint
    model_joint *model_writelock::makeJoint( dpid id )
    {
        return this->t->makeJoint( id );
    }
    
    //find joint
    model_joint *model_writelock::findJoint( dpid id )
    {
        return this->t->findJoint( id );
    }
    
    //get joints
    void model_writelock::getJoints( std::list<model_joint *> *l )
    {
        this->t->getJoints( l );
    }
    
    //add vertex joint
    model_vertex_joint *model_writelock::makeVertexJoint( dpid id, dpid vertex_id, dpid joint_id, float w )
    {
        return this->t->makeVertexJoint( id, vertex_id, joint_id, w );
    }
    
    //find vertex joint
    model_vertex_joint *model_writelock::findVertexJoint( dpid id )
    {
        return this->t->findVertexJoint( id );
    }
    
    //get vertex joints
    void model_writelock::getVertexJoints( std::list<model_vertex_joint *> *l )
    {
        this->t->getVertexJoints( l );
    }
    
    //add frame
    model_frame *model_writelock::makeFrame( dpid id )
    {
        return this->t->makeFrame( id );
    }
    
    //find frame
    model_frame *model_writelock::findFrame( dpid id )
    {
        return this->t->findFrame( id );
    }
    
    //get frame
    void model_writelock::getFrames( std::list<model_frame *> *l )
    {
        this->t->getFrames( l );
    }
    
    //add animation frame
    model_animation_frame *model_writelock::makeAnimationFrame( dpid id, dpid animation_id, dpid frame_id, unsigned int time_ms )
    {
        return this->t->makeAnimationFrame( id, animation_id, frame_id, time_ms );
    }
    
    //find animation frame
    model_animation_frame *model_writelock::findAnimationFrame( dpid id )
    {
        return this->t->findAnimationFrame( id );
    }
    
    //get animation frame
    void model_writelock::getAnimationFrames( std::list<model_animation_frame *> *l )
    {
        this->t->getAnimationFrames( l );
    }
    
    //get animation frame by frame or animation id
    void model_writelock::getAnimationFrames( std::list<model_animation_frame *> *l, dpid parent_id )
    {
        this->t->getAnimationFrames( l, parent_id );
    }
    
};
