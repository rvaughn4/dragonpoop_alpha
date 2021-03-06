
#include "model_readlock.h"
#include "model.h"

namespace dragonpoop
{

    //ctor
    model_readlock::model_readlock( model *t, dpmutex_readlock *l ) : shared_obj_readlock( t, l )
    {
        this->t = t;
    }

    //dtor
    model_readlock::~model_readlock( void )
    {

    }

    //return core
    core *model_readlock::getCore( void )
    {
        return this->t->getCore();
    }

    //get name
    void model_readlock::getName( std::string *sname )
    {
        this->t->getName( sname );
    }

    //compare name
    bool model_readlock::compareName( std::string *sname )
    {
        return this->t->compareName( sname );
    }

    //get id
    dpid model_readlock::getId( void )
    {
        return this->t->getId();
    }

    //compare id
    bool model_readlock::compareId( dpid id )
    {
        return this->t->compareId( id );
    }

    //get comment
    void model_readlock::getComment( std::string *s )
    {
        this->t->getComment( s );
    }
    
    //find component by type and id
    model_component *model_readlock::findComponent( uint16_t mtype, dpid id )
    {
        return this->t->findComponent( mtype, id );
    }
    
    //find component by type and name
    model_component *model_readlock::findComponent( uint16_t mtype, std::string *s )
    {
        return this->t->findComponent( mtype, s );
    }
    
    //find components
    void model_readlock::getComponents( std::list<model_component *> *l )
    {
        this->t->getComponents( l );
    }
    
    //find components by type
    void model_readlock::getComponents( uint16_t mtype, std::list<model_component *> *l )
    {
        this->t->getComponents( mtype, l );
    }
    
    //find components by type and 1 parent
    void model_readlock::getComponentsByParent( uint16_t mtype, dpid p1, std::list<model_component *> *l )
    {
        this->t->getComponentsByParent( mtype, p1, l );
    }
    
    //find components by type and 2 parents
    void model_readlock::getComponentsByParents( uint16_t mtype, dpid p1, dpid p2, std::list<model_component *> *l )
    {
        this->t->getComponentsByParents( mtype, p1, p2, l );
    }

    //find vertex
    model_vertex *model_readlock::findVertex( dpid id )
    {
        return this->t->findVertex( id );
    }
    
    //get vertexes
    void model_readlock::getVertexes( std::list<model_vertex *> *l )
    {
        this->t->getVertexes( l );
    }
    
    //find triangle
    model_triangle *model_readlock::findTriangle( dpid id )
    {
        return this->findTriangle( id );
    }
    
    //get triangles
    void model_readlock::getTriangles( std::list<model_triangle *> *l )
    {
        this->t->getTriangles( l );
    }
    
    //find group
    model_group *model_readlock::findGroup( dpid id )
    {
        return this->t->findGroup( id );
    }
    
    //get groups
    void model_readlock::getGroups( std::list<model_group *> *l )
    {
        this->t->getGroups( l );
    }
    
    //find triangle vertex
    model_triangle_vertex *model_readlock::findTriangleVertex( dpid id )
    {
        return this->t->findTriangleVertex( id );
    }
    
    //find triangle vertex
    model_triangle_vertex *model_readlock::findTriangleVertex( dpid triangle_id, dpid vertex_id )
    {
        return this->t->findTriangleVertex( triangle_id, vertex_id );
    }
    
    //get triangle vertexes
    void model_readlock::getTriangleVertexes( std::list<model_triangle_vertex *> *l )
    {
        this->t->getTriangleVertexes( l );
    }
    
    //get triangle vertexes by triangle or vertex id
    void model_readlock::getTriangleVertexes( std::list<model_triangle_vertex *> *l, dpid pid )
    {
        this->t->getTriangleVertexes( l, pid );
    }
    
    //find a group triangle
    model_group_triangle *model_readlock::findGroupTriangle( dpid id )
    {
        return this->t->findGroupTriangle( id );
    }
    
    //find a group triangle
    model_group_triangle *model_readlock::findGroupTriangle( dpid group_id, dpid triangle_id )
    {
        return this->t->findGroupTriangle( group_id, triangle_id );
    }
    
    //get all group triangles
    void model_readlock::getGroupTriangles( std::list<model_group_triangle *> *l )
    {
        this->t->getGroupTriangles( l );
    }
    
    //get all group triangles belonging to a group or triangle id
    void model_readlock::getGroupTriangles( std::list<model_group_triangle *> *l, dpid pid )
    {
        this->t->getGroupTriangles( l, pid );
    }
    
    //find material
    model_material *model_readlock::findMaterial( dpid id )
    {
        return this->t->findMaterial( id );
    }
    
    //get materials
    void model_readlock::getMaterials( std::list<model_material *> *l )
    {
        this->t->getMaterials( l );
    }
    
    //find animation
    model_animation *model_readlock::findAnimation( dpid id )
    {
        return this->t->findAnimation( id );
    }
    
    //find animation
    model_animation *model_readlock::findAnimation( const char *cname )
    {
        return this->t->findAnimation( cname );
    }

    //get animations
    void model_readlock::getAnimations( std::list<model_animation *> *l )
    {
        this->t->getAnimations( l );
    }
    
    //find joint
    model_joint *model_readlock::findJoint( dpid id )
    {
        return this->t->findJoint( id );
    }
    
    //get joints
    void model_readlock::getJoints( std::list<model_joint *> *l )
    {
        this->t->getJoints( l );
    }
    
    //find vertex joint
    model_vertex_joint *model_readlock::findVertexJoint( dpid id )
    {
        return this->t->findVertexJoint( id );
    }
    
    //get vertex joints
    void model_readlock::getVertexJoints( std::list<model_vertex_joint *> *l )
    {
        this->t->getVertexJoints( l );
    }
    
    //get vertex joints
    void model_readlock::getVertexJoints( std::list<model_vertex_joint *> *l, dpid p1 )
    {
        this->t->getVertexJoints( l, p1 );
    }
    
    //get vertex joints
    void model_readlock::getVertexJoints( std::list<model_vertex_joint *> *l, dpid p1, dpid p2 )
    {
        this->t->getVertexJoints( l, p1, p2 );
    }
    
    //find frame
    model_frame *model_readlock::findFrame( dpid id )
    {
        return this->t->findFrame( id );
    }
    
    //get frame
    void model_readlock::getFrames( std::list<model_frame *> *l )
    {
        this->t->getFrames( l );
    }
 
    //find animation frame
    model_animation_frame *model_readlock::findAnimationFrame( dpid id )
    {
        return this->t->findAnimationFrame( id );
    }
    
    //get animation frame
    void model_readlock::getAnimationFrames( std::list<model_animation_frame *> *l )
    {
        this->t->getAnimationFrames( l );
    }
    
    //get animation frame by frame or animation id
    void model_readlock::getAnimationFrames( std::list<model_animation_frame *> *l, dpid parent_id )
    {
        this->t->getAnimationFrames( l, parent_id );
    }
    
    //find FrameJoint
    model_frame_joint *model_readlock::findFrameJoint( dpid id )
    {
        return this->t->findFrameJoint( id );
    }
    
    //get FrameJoints
    void model_readlock::getFrameJoints( std::list<model_frame_joint *> *l )
    {
        this->t->getFrameJoints( l );
    }
    
    //get FrameJoints by frame or animation id
    void model_readlock::getFrameJoints( std::list<model_frame_joint *> *l, dpid parent_id )
    {
        this->t->getFrameJoints( l, parent_id );
    }
    
    //get FrameJoints by frame and animation id
    void model_readlock::getFrameJoints( std::list<model_frame_joint *> *l, dpid parent_id_1, dpid parent_id_2 )
    {
        this->t->getFrameJoints( l, parent_id_1, parent_id_2 );
    }
    
    //write model header to file/memory
    bool model_readlock::writeHeader( dpbuffer *b )
    {
        return this->t->writeHeader( b );
    }
    
    //get model dimensions
    void model_readlock::getSize( dpxyz_f *x )
    {
        return this->t->getSize( x );
    }

    //get model dimensions
    void model_readlock::getCenter( dpxyz_f *x )
    {
        return this->t->getCenter( x );
    }
    
    //find instance
    model_instance_ref *model_readlock::findInstance( dpid id )
    {
        return this->t->findInstance( id );
    }
    
    //return instance count
    unsigned int model_readlock::getInstanceCount( void )
    {
        return this->t->getInstanceCount();
    }
    
    //get ref count
    int model_readlock::getRefCount( void )
    {
        return this->t->getRefCount();
    }
    
};
