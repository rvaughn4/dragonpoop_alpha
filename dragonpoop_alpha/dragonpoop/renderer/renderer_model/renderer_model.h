
#ifndef dragonpoop_renderer_model_h
#define dragonpoop_renderer_model_h

#include "../../core/shared_obj/shared_obj.h"
#include "../../core/dpid/dpid.h"
#include <string>
#include <list>
#include "../../gfx/model/model_component/model_components.h"

namespace dragonpoop
{
    
    class dptask;
    class dpthread_lock;
    class core;
    class renderer_model_writelock;
    class renderer_model_group;
    class renderer_model_instance;
    class renderer_model_instance_ref;
    class model_instance_writelock;
    class model_writelock;
    class model_ref;
    
    class renderer_model : public shared_obj
    {
        
    private:
        
        dpid id;
        struct
        {
            std::list<model_component *> lst;
        } comps;
        std::list<renderer_model_instance *> instances;
        std::atomic<bool> bIsSynced;
        model_ref *m;
        uint64_t t_last_i_ran;
        
        //delete all components
        void deleteComponents( void );
        //delete instances
        void deleteInstances( void );
        //sync instances
        void syncInstances( model_writelock *g );
        //run instances
        void runInstances( dpthread_lock *thd );
        
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
        //create instance
        void makeInstance( model_instance_writelock *ml );
        //sync model instance with changes
        void sync( void );
        //run model from task
        void run( dpthread_lock *thd, renderer_model_writelock *g, model_writelock *ml );
        
    public:
        
        //ctor
        renderer_model( model_writelock *ml );
        //dtor
        virtual ~renderer_model( void );
        //get id
        dpid getId( void );
        
        friend class renderer_model_readlock;
        friend class renderer_model_writelock;

    };
    
};

#endif