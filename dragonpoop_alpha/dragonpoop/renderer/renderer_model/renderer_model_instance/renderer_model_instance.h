
#ifndef dragonpoop_renderer_model_instance_h
#define dragonpoop_renderer_model_instance_h

#include "../../../core/shared_obj/shared_obj.h"
#include "../../../core/dpid/dpid.h"
#include <string>
#include "../../../gfx/model/model_component/model_components.h"

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
    
    class renderer_model_instance : public shared_obj
    {
        
    private:
        
        dpid id;
        struct
        {
            std::list<model_component *> lst;
        } comps;
        std::atomic<bool> bIsSynced;
        model_instance_ref *m;
        
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
        renderer_model_instance_group *makeGroup( model_instance_writelock *ml, model_instance_group *g );
        //find group
        renderer_model_instance_group *findGroup( dpid id );
        //get groups
        void getGroups( std::list<renderer_model_instance_group *> *l );
        //make groups
        void makeGroups( model_instance_writelock *ml );
        //sync
        void sync( void );
        //run model from task
        void run( dpthread_lock *thd, renderer_model_instance_writelock *g );
        //handle sync
        virtual void onSync( dpthread_lock *thd, renderer_model_instance_writelock *g, model_instance_writelock *ml );
        //genertae group
        virtual renderer_model_instance_group *genGroup( model_instance_writelock *ml, model_instance_group *g );
        //render model
        void render( dpthread_lock *thd, renderer_writelock *r, renderer_model_readlock *m, renderer_model_instance_readlock *mi );
        
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