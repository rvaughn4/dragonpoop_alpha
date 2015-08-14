
#ifndef dragonpoop_model_readlock_h
#define dragonpoop_model_readlock_h

#include "../../core/shared_obj/shared_obj_readlock.h"
#include "../../core/dpid/dpid.h"
#include <string>
#include <list>

namespace dragonpoop
{
    class model;
    class core;
    class model_component;
    class model_vertex;
    class model_triangle;
    class model_group;
    class model_triangle_vertex;
    class model_group_triangle;
    class model_material;
    class model_animation;
    class model_joint;

    class model_readlock : public shared_obj_readlock
    {

    private:

        model *t;

    protected:

        //ctor
        model_readlock( model *t, dpmutex_readlock *l );
        //dtor
        virtual ~model_readlock( void );

    public:

        //return core
        core *getCore( void );
        //get name
        void getName( std::string *sname );
        //compare name
        bool compareName( std::string *sname );
        //get id
        dpid getId( void );
        //compare id
        bool compareId( dpid id );
        //get comment
        void getComment( std::string *s );
        //find component by type and id
        model_component *findComponent( uint16_t mtype, dpid id );
        //find components by type
        void getComponents( uint16_t mtype, std::list<model_component *> *l );
        //find components by type and 1 parent
        void getComponentsByParent( uint16_t mtype, dpid p1, std::list<model_component *> *l );
        //find components by type and 2 parents
        void getComponentsByParents( uint16_t mtype, dpid p1, dpid p2, std::list<model_component *> *l );
        //find vertex
        model_vertex *findVertex( dpid id );
        //get vertexes
        void getVertexes( std::list<model_vertex *> *l );
        //find triangle
        model_triangle *findTriangle( dpid id );
        //get triangles
        void getTriangles( std::list<model_triangle *> *l );
        //find group
        model_group *findGroup( dpid id );
        //get groups
        void getGroups( std::list<model_group *> *l );
        //find triangle vertex
        model_triangle_vertex *findTriangleVertex( dpid id );
        //find triangle vertex
        model_triangle_vertex *findTriangleVertex( dpid triangle_id, dpid vertex_id );
        //get triangle vertexes
        void getTriangleVertexes( std::list<model_triangle_vertex *> *l );
        //get triangle vertexes by triangle or vertex id
        void getTriangleVertexes( std::list<model_triangle_vertex *> *l, dpid pid );
        //find a group triangle
        model_group_triangle *findGroupTriangle( dpid id );
        //find a group triangle
        model_group_triangle *findGroupTriangle( dpid group_id, dpid triangle_id );
        //get all group triangles
        void getGroupTriangles( std::list<model_group_triangle *> *l );
        //get all group triangles belonging to a group or triangle id
        void getGroupTriangles( std::list<model_group_triangle *> *l, dpid pid );
        //find material
        model_material *findMaterial( dpid id );
        //get materials
        void getMaterials( std::list<model_material *> *l );
        //find animation
        model_animation *findAnimation( dpid id );
        //get animations
        void getAnimations( std::list<model_animation *> *l );
        //find joint
        model_joint *findJoint( dpid id );
        //get joints
        void getJoints( std::list<model_joint *> *l );

        friend class model;
    };
    
};

#endif