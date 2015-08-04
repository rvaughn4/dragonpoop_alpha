
#ifndef dragonpoop_model_instance_readlock_h
#define dragonpoop_model_instance_readlock_h

#include "../../../core/shared_obj/shared_obj_readlock.h"
#include "../../../core/dpid/dpid.h"
#include <list>

namespace dragonpoop
{
    class model_instance;
    class core;
    class model_component;
    class model_instance_vertex;
    class model_instance_group;
    class model_instance_triangle_vertex;
    
    class model_instance_readlock : public shared_obj_readlock
    {
        
    private:
        
        model_instance *t;
        
    protected:
        
        //ctor
        model_instance_readlock( model_instance *t, dpmutex_readlock *l );
        //dtor
        virtual ~model_instance_readlock( void );
        
    public:
        
        //return core
        core *getCore( void );
        //get id
        dpid getId( void );
        //find component by type and id
        model_component *findComponent( uint16_t mtype, dpid id );
        //find components by type
        void getComponents( uint16_t mtype, std::list<model_component *> *l );
        //find components by type and 1 parent
        void getComponentsByParent( uint16_t mtype, dpid p1, std::list<model_component *> *l );
        //find components by type and 2 parents
        void getComponentsByParents( uint16_t mtype, dpid p1, dpid p2, std::list<model_component *> *l );
        //find vertex
        model_instance_vertex *findVertex( dpid id );
        //get vertexes
        void getVertexes( std::list<model_instance_vertex *> *l );
        //find group
        model_instance_group *findGroup( dpid id );
        //get groups
        void getGroups( std::list<model_instance_group *> *l );
        //find triangle vertex
        model_instance_triangle_vertex *findTriangleVertex( dpid id );
        //find triangle vertex
        model_instance_triangle_vertex *findTriangleVertex( dpid triangle_id, dpid vertex_id );
        //get triangle vertexes
        void getTriangleVertexes( std::list<model_instance_triangle_vertex *> *l );
        //get triangle vertexes by triangle or vertex id
        void getTriangleVertexes( std::list<model_instance_triangle_vertex *> *l, dpid pid );
        
        friend class model_instance;
    };
    
};

#endif