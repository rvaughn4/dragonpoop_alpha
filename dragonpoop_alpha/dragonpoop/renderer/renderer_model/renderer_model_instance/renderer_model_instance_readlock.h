
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
        //get groups
        void getGroups( std::list<renderer_model_instance_group *> *l );
        //sync
        void sync( void );
        //render model
        void render( dpthread_lock *thd, renderer_writelock *r, renderer_model_readlock *m );
        
        friend class renderer_model_instance;
    };
    
};

#endif