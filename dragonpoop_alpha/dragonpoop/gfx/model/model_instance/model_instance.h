
#ifndef dragonpoop_model_instance_h
#define dragonpoop_model_instance_h

#include "../../../core/shared_obj/shared_obj.h"
#include "../../../core/dpid/dpid.h"
#include <string>
#include "../model_component/model_components.h"
#include "../../../core/dpbtree/dpid_btree.h"
#include "../../../core/dpbtree/dpid_multibtree.h"
#include "../../../core/dpbtree/dpmultibtree.h"

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
    class model_instance_triangle;
    class model_instance_triangle_vertex;
    class model_writelock;
    class model_ref;
    class model_vertex;
    class model_group;
    class model_triangle_vertex;
    class model_group_vertex;
    class renderer_model_instance;
    class renderer_model_instance_ref;
    class model_group_triangle;
    class dpvertexindex_buffer;
    class model_instance_animation;
    class model_animation;
    class model_instance_joint;
    class model_joint;
    struct dpxyzw;
    
    class model_instance : public shared_obj
    {
        
    private:
        
        core *c;
        dpid id;
        model_ref *m;
        struct
        {
            std::list<model_component *> lst;
            dpid_btree byid;
            dpmultibtree bytype;
            dpid_multibtree byowner;
        } comps;
        renderer_model_instance_ref *r;
        uint64_t t_start, t_end, t_last_animate, t_frame_time;
        
        //delete all components
        void deleteComponents( void );
        //populate vertex buffer for rendering
        void computeMeshes( void );
        
        //redo mesh
        void redoMesh( void );
        //redo group mesh
        void redoMesh( model_instance_group *g );
        //redo triangle mesh
        void redoMesh( dpvertexindex_buffer *vb, model_instance_triangle *t );
        //redo vertex mesh
        void redoMesh( dpvertexindex_buffer *vb, model_instance_triangle_vertex *tv );
        
        //redo animation
        void redoAnim( model_writelock *m );
        //redo animation, compute current frame
        void redoAnim( model_writelock *m, model_instance_animation *a );
        //redo animation, compound joint transforms
        void redoAnim( model_writelock *m, model_instance_joint *j );
        //redo animation, compound joint transforms for a given animation
        void redoAnim( model_writelock *m, model_instance_joint *j, model_instance_animation *a, dpxyzw *trans, dpxyzw *rot );

    protected:
        
        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //run model from task
        void run( dpthread_lock *thd, model_instance_writelock *g, model_writelock *m );
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
        model_instance_triangle_vertex *makeTriangleVertex( model_triangle_vertex *tv );
        //find triangle vertex
        model_instance_triangle_vertex *findTriangleVertex( dpid id );
        //find triangle vertex
        model_instance_triangle_vertex *findTriangleVertex( dpid triangle_id, dpid vertex_id );
        //get triangle vertexes
        void getTriangleVertexes( std::list<model_instance_triangle_vertex *> *l );
        //get triangle vertexes by triangle or vertex id
        void getTriangleVertexes( std::list<model_instance_triangle_vertex *> *l, dpid pid );
        //add triangle
        model_instance_triangle *makeTriangle( model_group_triangle *gt );
        //find triangle
        model_instance_triangle *findTriangle( dpid id );
        //find triangle
        model_instance_triangle *findTriangle( dpid group_id, dpid triangle_id );
        //get triangles
        void getTriangles( std::list<model_instance_triangle *> *l );
        //get triangles by triangle or group id
        void getTriangles( std::list<model_instance_triangle *> *l, dpid pid );
        //make verts
        void makeVertexes( model_writelock *ml );
        //make triangles
        void makeTriangles( model_writelock *ml );
        //make triangle verts
        void makeTriangleVertexes( model_writelock *ml );
        //make groups
        void makeGroups( model_writelock *ml );
        //add animation
        model_instance_animation *makeAnimation( model_animation *g );
        //find animation
        model_instance_animation *findAnimation( dpid id );
        //get animations
        void getAnimations( std::list<model_instance_animation *> *l );
        //make animations
        void makeAnimations( model_writelock *ml );
        //add joint
        model_instance_joint *makeJoint( model_joint *g );
        //find joint
        model_instance_joint *findJoint( dpid id );
        //get joints
        void getJoints( std::list<model_instance_joint *> *l );
        //make joints
        void makeJoints( model_writelock *ml );
        //sync
        void sync( model_writelock *ml, uint64_t tms );
        //do animation
        void animate( model_writelock *ml, uint64_t tms );
        //set renderer model
        void setRenderer( renderer_model_instance *r );
        //populate vertex buffer for rendering
        void fillVertexBuffer( dpid group_id, dpvertexindex_buffer *vb );
        
    public:
        
        //ctor
        model_instance( dpid id, model_writelock *ml );
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