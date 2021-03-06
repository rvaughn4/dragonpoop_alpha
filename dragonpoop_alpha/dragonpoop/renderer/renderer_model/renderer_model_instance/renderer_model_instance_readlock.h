
#ifndef dragonpoop_renderer_model_instance_readlock_h
#define dragonpoop_renderer_model_instance_readlock_h

#include "../../../core/shared_obj/shared_obj_readlock.h"
#include "../../../core/dpid/dpid.h"
#include <list>

namespace dragonpoop
{
    class renderer_model_instance;
    class model_component;
    class renderer_model_instance_group;
    class renderer_writelock;
    class renderer_model_readlock;
    class renderer_model_instance_readlock;
    class dpthread_lock;
    class model_instance_joint_cache;
    class renderer_model_instance_joint;
    struct dpvertex;
    class dpmatrix;
    struct model_gui_pos;
    class dpthread_lock;
    class dpposition;
    class render_api_context_writelock;
    class render_api_commandlist_writelock;
    class dpheight_cache_readlock;

    class renderer_model_instance_readlock : public shared_obj_readlock
    {

    private:

        renderer_model_instance *t;

    protected:

        //ctor
        renderer_model_instance_readlock( renderer_model_instance *t, dpmutex_readlock *l );
        //dtor
        virtual ~renderer_model_instance_readlock( void );

    public:

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
        //find group
        renderer_model_instance_group *findGroup( dpid id );
        //find joint
        renderer_model_instance_joint *findJoint( dpid id );
        //get groups
        void getGroups( std::list<renderer_model_instance_group *> *l );
        //get joints
        void getJoints( std::list<renderer_model_instance_joint *> *l );
        //sync
        void sync( void );
        //animate
        void animate( void );
        //render model
        void render( dpthread_lock *thd, dpposition *campos, renderer_model_readlock *m, dpheight_cache_readlock *heights, dpmatrix *m_world, render_api_context_writelock *ctx, render_api_commandlist_writelock *clist );
        //returns joint cache
        model_instance_joint_cache *getJointCache( void );
        //transform vertex using joints
        void transform( dpvertex *v );
        //recompute animation joint matrixes
        void redoMatrixes( uint64_t t );
        //get dimensions
        model_gui_pos *getGuiDimensions( void );
        //returns true if gui
        bool isGui( void );
        //returns true if alive
        bool isAlive( void );
        //retuns opacity
        float getAlpha( void );

        friend class renderer_model_instance;
    };

};

#endif
