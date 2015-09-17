
#ifndef dragonpoop_model_writelock_h
#define dragonpoop_model_writelock_h

#include "../../core/shared_obj/shared_obj_writelock.h"
#include "../../core/dpid/dpid.h"
#include <string>
#include <list>

namespace dragonpoop
{
    class model;
    class core;
    class dpthread_lock;
    class model_component;
    class model_vertex;
    class model_triangle;
    class model_group;
    class model_triangle_vertex;
    class model_group_triangle;
    class model_instance_ref;
    class renderer_model;
    class model_material;
    class model_animation;
    class model_joint;
    class model_vertex_joint;
    class model_frame;
    class model_animation_frame;
    class model_frame_joint;
    class dpbuffer;
    struct dpxyz_f;

    class model_writelock : public shared_obj_writelock
    {

    private:

        model *t;

    protected:

        //ctor
        model_writelock( model *t, dpmutex_writelock *l );
        //dtor
        virtual ~model_writelock( void );

    public:

        //return core
        core *getCore( void );
        //run model from task
        void run( dpthread_lock *thd );
        //set name
        void setName( std::string *sname );
        //get name
        void getName( std::string *sname );
        //compare name
        bool compareName( std::string *sname );
        //set id
        void setId( dpid id );
        //get id
        dpid getId( void );
        //compare id
        bool compareId( dpid id );
        //set comment
        void setComment( std::string *s );
        //get comment
        void getComment( std::string *s );
        //append to comment
        void appendComment( std::string *s );
        //add component to list and trees
        void addComponent( model_component *c );
        //add component, 1 parent
        void addComponent( model_component *c, dpid p1 );
        //add component, 2 parents
        void addComponent( model_component *c, dpid p1, dpid p2 );
        //find component by type and id
        model_component *findComponent( uint16_t mtype, dpid id );
        //find component by type and name
        model_component *findComponent( uint16_t mtype, std::string *s );
        //find components
        void getComponents( std::list<model_component *> *l );
        //find components by type
        void getComponents( uint16_t mtype, std::list<model_component *> *l );
        //find components by type and 1 parent
        void getComponentsByParent( uint16_t mtype, dpid p1, std::list<model_component *> *l );
        //find components by type and 2 parents
        void getComponentsByParents( uint16_t mtype, dpid p1, dpid p2, std::list<model_component *> *l );
        //remove component
        void removeComponent( model_component *c );
        //add vertex
        model_vertex *makeVertex( dpid id );
        //find vertex
        model_vertex *findVertex( dpid id );
        //get vertexes
        void getVertexes( std::list<model_vertex *> *l );
        //add triangle
        model_triangle *makeTriangle( dpid id );
        //find triangle
        model_triangle *findTriangle( dpid id );
        //get triangles
        void getTriangles( std::list<model_triangle *> *l );
        //add group
        model_group *makeGroup( dpid id );
        //find group
        model_group *findGroup( dpid id );
        //get groups
        void getGroups( std::list<model_group *> *l );
        //add triangle vertex
        model_triangle_vertex *makeTriangleVertex( dpid id, dpid triangle_id, dpid vertex_id );
        //find triangle vertex
        model_triangle_vertex *findTriangleVertex( dpid id );
        //find triangle vertex
        model_triangle_vertex *findTriangleVertex( dpid triangle_id, dpid vertex_id );
        //get triangle vertexes
        void getTriangleVertexes( std::list<model_triangle_vertex *> *l );
        //get triangle vertexes by triangle or vertex id
        void getTriangleVertexes( std::list<model_triangle_vertex *> *l, dpid pid );
        //add a group triangle
        model_group_triangle *makeGroupTriangle( dpid id, dpid group_id, dpid triangle_id );
        //find a group triangle
        model_group_triangle *findGroupTriangle( dpid id );
        //find a group triangle
        model_group_triangle *findGroupTriangle( dpid group_id, dpid triangle_id );
        //get all group triangles
        void getGroupTriangles( std::list<model_group_triangle *> *l );
        //get all group triangles belonging to a group or triangle id
        void getGroupTriangles( std::list<model_group_triangle *> *l, dpid pid );
        //create instance
        model_instance_ref *makeInstance( dpid id );
        //get instances
        void getInstances( std::list<model_instance_ref *> *l );
        //sync model instance with changes
        void sync( dpthread_lock *thd );
        //set renderer model
        void setRenderer( renderer_model *r );
        //add material
        model_material *makeMaterial( dpid id );
        //find material
        model_material *findMaterial( dpid id );
        //get materials
        void getMaterials( std::list<model_material *> *l );
        //add animation
        model_animation *makeAnimation( dpid id );
        //find animation
        model_animation *findAnimation( dpid id );
        //get animations
        void getAnimations( std::list<model_animation *> *l );
        //add joint
        model_joint *makeJoint( dpid id );
        //find joint
        model_joint *findJoint( dpid id );
        //get joints
        void getJoints( std::list<model_joint *> *l );
        //add vertex joint
        model_vertex_joint *makeVertexJoint( dpid id, dpid vertex_id, dpid joint_id, float w );
        //find vertex joint
        model_vertex_joint *findVertexJoint( dpid id );
        //get vertex joints
        void getVertexJoints( std::list<model_vertex_joint *> *l );
        //get vertex joints
        void getVertexJoints( std::list<model_vertex_joint *> *l, dpid p1 );
        //get vertex joints
        void getVertexJoints( std::list<model_vertex_joint *> *l, dpid p1, dpid p2 );
        //add frame
        model_frame *makeFrame( dpid id );
        //find frame
        model_frame *findFrame( dpid id );
        //get frame
        void getFrames( std::list<model_frame *> *l );
        //add animation frame
        model_animation_frame *makeAnimationFrame( dpid id, dpid animation_id, dpid frame_id, unsigned int time_ms );
        //find animation frame
        model_animation_frame *findAnimationFrame( dpid id );
        //get animation frame
        void getAnimationFrames( std::list<model_animation_frame *> *l );
        //get animation frame by frame or animation id
        void getAnimationFrames( std::list<model_animation_frame *> *l, dpid parent_id );
        //add FrameJoint
        model_frame_joint *makeFrameJoint( dpid id, dpid frame_id, dpid joint_id );
        //find FrameJoint
        model_frame_joint *findFrameJoint( dpid id );
        //get FrameJoints
        void getFrameJoints( std::list<model_frame_joint *> *l );
        //get FrameJoints by frame or animation id
        void getFrameJoints( std::list<model_frame_joint *> *l, dpid parent_id );
        //get FrameJoints by frame and animation id
        void getFrameJoints( std::list<model_frame_joint *> *l, dpid parent_id_1, dpid partent_id_2 );
        //write model header to file/memory
        bool writeHeader( dpbuffer *b );
        //read model header from file/memory
        bool readHeader( dpbuffer *b, unsigned int *cnt_components );
        //read and create model component from file/memory
        bool readComponent( dpbuffer *b, model_component **c );
        //get model dimensions
        void getSize( dpxyz_f *x );

        friend class model;
    };
    
};

#endif