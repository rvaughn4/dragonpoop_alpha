
#ifndef dragonpoop_model_instance_writelock_h
#define dragonpoop_model_instance_writelock_h

#include "../../../core/shared_obj/shared_obj_writelock.h"
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
    class dpthread_lock;
    class model_writelock;
    class renderer_model_instance;
    class dpvertexindex_buffer;
    
    class model_instance_writelock : public shared_obj_writelock
    {
        
    private:
        
        model_instance *t;
        
    protected:
        
        //ctor
        model_instance_writelock( model_instance *t, dpmutex_writelock *l );
        //dtor
        virtual ~model_instance_writelock( void );
        
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
        //run model from task
        void run( dpthread_lock *thd );
        //sync
        void sync( model_writelock *ml );
        //set renderer model
        void setRenderer( renderer_model_instance *r );
        //populate vertex buffer for rendering
        void fillVertexBuffer( dpid group_id, dpvertexindex_buffer *vb );

        friend class model_instance;
    };
    
};

#endif