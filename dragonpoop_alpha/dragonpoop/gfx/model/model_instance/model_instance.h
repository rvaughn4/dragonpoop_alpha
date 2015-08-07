
#ifndef dragonpoop_model_instance_h
#define dragonpoop_model_instance_h

#include "../../../core/shared_obj/shared_obj.h"
#include "../../../core/dpid/dpid.h"
#include <string>
#include "../model_component/model_components.h"

namespace dragonpoop
{
    
    class dptaskpool_writelock;
    class dptask;
    class model_task;
    class dpthread_lock;
    class core;
    class model_instance_writelock;
    class model_instance_vertex;
    class model_instance_group;
    class model_instance_triangle_vertex;
    class model_writelock;
    class model_ref;
    class model_vertex;
    class model_group;
    class model_triangle_vertex;
    
    class model_instance : public shared_obj
    {
        
    private:
        
        core *c;
        dpid id;
        model_ref *m;
        struct
        {
            std::list<model_component *> lst;
        } comps;
        
        //delete all components
        void deleteComponents( void );
        
    protected:
        
        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //run model from task
        void run( dpthread_lock *thd, model_instance_writelock *g );
        //get id
        dpid getId( void );
        //add component to list and trees
        void addComponent( model_component *c );
        //add component, 1 parent
        void addComponent( model_component *c, dpid p1 );
        //add component, 2 parents
        void addComponent( model_component *c, dpid p1, dpid p2 );
        //find component by type and id
        model_component *findComponent( uint16_t mtype, dpid id );
        //find components by type
        void getComponents( uint16_t mtype, std::list<model_component *> *l );
        //find components by type and 1 parent
        void getComponentsByParent( uint16_t mtype, dpid p1, std::list<model_component *> *l );
        //find components by type and 2 parents
        void getComponentsByParents( uint16_t mtype, dpid p1, dpid p2, std::list<model_component *> *l );
        //remove component
        void removeComponent( model_component *c );
        //add vertex
        model_instance_vertex *makeVertex( model_vertex *v );
        //find vertex
        model_instance_vertex *findVertex( dpid id );
        //get vertexes
        void getVertexes( std::list<model_instance_vertex *> *l );
        //add group
        model_instance_group *makeGroup( model_group *g );
        //find group
        model_instance_group *findGroup( dpid id );
        //get groups
        void getGroups( std::list<model_instance_group *> *l );
        //add triangle vertex
        model_instance_triangle_vertex *makeTriangleVertex( model_triangle_vertex *v );
        //find triangle vertex
        model_instance_triangle_vertex *findTriangleVertex( dpid id );
        //find triangle vertex
        model_instance_triangle_vertex *findTriangleVertex( dpid triangle_id, dpid vertex_id );
        //get triangle vertexes
        void getTriangleVertexes( std::list<model_instance_triangle_vertex *> *l );
        //get triangle vertexes by triangle or vertex id
        void getTriangleVertexes( std::list<model_instance_triangle_vertex *> *l, dpid pid );
        //make verts
        void makeVertexes( model_writelock *ml );
        //make triangle verts
        void makeTriangleVertexes( model_writelock *ml );
        //make groups
        void makeGroups( model_writelock *ml );
        //sync
        void sync( model_writelock *ml );
        
    public:
        
        //ctor
        model_instance( model_writelock *ml );
        //dtor
        virtual ~model_instance( void );
        //return core
        core *getCore( void );
        
        friend class model_instance_readlock;
        friend class model_instance_writelock;
        friend class gfx;
    };
    
};

#endif