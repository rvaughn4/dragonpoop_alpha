
#ifndef dragonpoop_renderer_model_instance_h
#define dragonpoop_renderer_model_instance_h

#include "../../../core/shared_obj/shared_obj.h"
#include "../../../core/dpid/dpid.h"
#include "../../../gfx/model/model_component/model_components.h"
#include "../../../core/dpbtree/dpid_btree.h"
#include "../../../core/dpbtree/dpid_multibtree.h"
#include "../../../core/dpbtree/dpmultibtree.h"
#include "../../../core/bytetree/dpid_bytetree.h"
#include "../../../gfx/model/model_instance/model_instance_joint_cache/model_instance_joint_cache.h"
#include "../../../gfx/model/model_instance/model_instance.h"
#include "../../../gfx/dpposition/dpposition.h"

#include <string>

namespace dragonpoop
{

    class renderer_model_instance_writelock;
    class renderer_model_instance_vertex;
    class renderer_model_instance_group;
    class renderer_model_writelock;
    class renderer_model_ref;
    class model_instance_group;
    class model_instance_writelock;
    class model_instance_ref;
    class dpthread_lock;
    class renderer_model_readlock;
    class renderer_writelock;
    class renderer_model_instance_readlock;
    class renderer_model_instance_joint;
    struct dpvertex;
    struct model_gui_pos;
    class dpmatrix;
    class render_api_context_writelock;
    class render_api_commandlist_writelock;
    class renderer_model_material;
    class dpheight_cache_readlock;

    class renderer_model_instance : public shared_obj
    {

    private:

        dpid id;
        struct
        {
            std::list<model_component *> lst;
            dpid_btree byid;
            dpid_multibtree byowner;
            dpmultibtree bytype;
        } comps;
        std::atomic<bool> bIsSynced, bIsAnimated, bIsGui, bIsPosSynced;
        model_instance_ref *m;
        model_instance_joint_cache jnts;
        model_gui_pos gui_pos;
        dpposition pos;
        bool bIsAlive;
        float fade_val;
        dpxyz_f smooth_rot;

        //delete all components
        void deleteComponents( void );

    protected:

        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //add component to list and trees
        void addComponent( model_component *c );
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
        //add group
        renderer_model_instance_group *makeGroup( model_instance_writelock *ml, model_instance_group *g, dpthread_lock *thd );
        //find group
        renderer_model_instance_group *findGroup( dpid id );
        //add joint
        renderer_model_instance_joint *makeJoint( model_instance_writelock *ml, model_instance_joint *g, dpthread_lock *thd );
        //find joint
        renderer_model_instance_joint *findJoint( dpid id );
        //get groups
        void getGroups( std::list<renderer_model_instance_group *> *l );
        //make groups
        void makeGroups( model_instance_writelock *ml, dpthread_lock *thd );
        //sync groups
        void syncGroups( model_instance_writelock *ml, dpthread_lock *thd );
        //animate groups
        void animateGroups( model_instance_writelock *ml, dpthread_lock *thd );
        //sync
        void sync( void );
        //animate
        void animate( void );
        //run model from task
        void run( dpthread_lock *thd, renderer_model_instance_writelock *g );
        //handle sync
        virtual void onSync( dpthread_lock *thd, renderer_model_instance_writelock *g, model_instance_writelock *ml );
        //genertae group
        virtual renderer_model_instance_group *genGroup( model_instance_writelock *ml, model_instance_group *g, dpthread_lock *thd );
        //genertae joint
        virtual renderer_model_instance_joint *genJoint( model_instance_writelock *ml, model_instance_joint *g, dpthread_lock *thd );
        //render model
        void render( dpthread_lock *thd, dpposition *campos, renderer_model_readlock *m, renderer_model_instance_readlock *mi, dpheight_cache_readlock *heights, dpmatrix *m_world, render_api_context_writelock *ctx, render_api_commandlist_writelock *clist );
        //render group
        void renderGroup( dpthread_lock *thd, renderer_model_instance_readlock *mi, renderer_model_material *m, renderer_model_instance_group *g, render_api_context_writelock *ctx, render_api_commandlist_writelock *clist );
        //returns joint cache
        model_instance_joint_cache *getJointCache( void );
        //get joints
        void getJoints( std::list<renderer_model_instance_joint *> *l );
        //make joints
        void makeJoints( model_instance_writelock *ml, dpthread_lock *thd );
        //sync joints
        void syncJoints( model_instance_writelock *ml, dpthread_lock *thd );
        //transform vertex using joints
        void transform( dpvertex *v );
        //recompute animation joint matrixes
        void redoMatrixes( renderer_model_instance_readlock *m, uint64_t t );
        //recompute animation joint matrixes
        void redoMatrixes( renderer_model_instance_writelock *m, uint64_t t );
        //get model view matrix
        void getModelViewMatrix( dpthread_lock *thd, dpposition *campos, renderer_model_readlock *m, dpheight_cache_readlock *heights, dpmatrix *in_world_matrix, dpmatrix *out_model_matrix );
        //get dimensions
        model_gui_pos *getGuiDimensions( void );
        //returns true if gui
        bool isGui( void );
        //get position
        void getPosition( dpposition *p );
        //get position
        dpposition *_getPosition( void );
        //sync position
        void syncPosition( void );
        //returns true if alive
        bool isAlive( void );
        //kills instance
        void kill( void );
        //retuns opacity
        float getAlpha( void );

    public:

        //ctor
        renderer_model_instance( model_instance_writelock *ml );
        //dtor
        virtual ~renderer_model_instance( void );
        //get id
        dpid getId( void );

        friend class renderer_model_instance_readlock;
        friend class renderer_model_instance_writelock;

    };

};

#endif
