
#ifndef dragonpoop_renderer_model_instance_writelock_h
#define dragonpoop_renderer_model_instance_writelock_h

#include "../../../core/shared_obj/shared_obj_writelock.h"
#include "../../../core/dpid/dpid.h"
#include <list>

namespace dragonpoop
{
    class renderer_model_instance;
    class model_component;
    class model_instance_vertex;
    class renderer_model_instance_group;
    class dpthread_lock;
    class model_instance_joint_cache;
    
    class renderer_model_instance_writelock : public shared_obj_writelock
    {
        
    private:
        
        renderer_model_instance *t;
        
    protected:
        
        //ctor
        renderer_model_instance_writelock( renderer_model_instance *t, dpmutex_writelock *l );
        //dtor
        virtual ~renderer_model_instance_writelock( void );
        
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
        //get groups
        void getGroups( std::list<renderer_model_instance_group *> *l );
        //sync
        void sync( void );
        //animate
        void animate( void );
        //run model from task
        void run( dpthread_lock *thd );
        //returns joint cache
        model_instance_joint_cache *getJointCache( void );

        friend class renderer_model_instance;
    };
    
};

#endif