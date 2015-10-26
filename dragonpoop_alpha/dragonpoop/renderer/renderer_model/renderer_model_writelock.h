
#ifndef dragonpoop_renderer_model_writelock_h
#define dragonpoop_renderer_model_writelock_h

#include "../../core/shared_obj/shared_obj_writelock.h"
#include "../../core/dpid/dpid.h"
#include <string>
#include <list>

namespace dragonpoop
{
    class renderer_model;
    class model_component;
    class renderer_model_group;
    class model_instance_writelock;
    class renderer_model_instance_ref;
    class dpthread_lock;
    class model_writelock;
    class renderer_model_material;
    struct dpxyz_f;
    class render_api_context_writelock;
    
    class renderer_model_writelock : public shared_obj_writelock
    {
        
    private:
        
        renderer_model *t;
        
    protected:
        
        //ctor
        renderer_model_writelock( renderer_model *t, dpmutex_writelock *l );
        //dtor
        virtual ~renderer_model_writelock( void );
        
    public:
        
        //get id
        dpid getId( void );
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
        //find material
        renderer_model_material *findMaterial( dpid id );
        //get materials
        void getMaterials( std::list<renderer_model_material *> *l );
        //get size
        void getSize( dpxyz_f *x );
        //get center
        void getCenter( dpxyz_f *x );
        //returns true if alive
        bool isAlive( void );
        //kill model
        void kill( void );
        //run model from task
        void run( dpthread_lock *thd, render_api_context_writelock *ctx );

        friend class renderer_model;
    };
    
};

#endif