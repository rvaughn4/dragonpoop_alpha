
#ifndef dragonpoop_renderer_model_readlock_h
#define dragonpoop_renderer_model_readlock_h

#include "../../core/shared_obj/shared_obj_readlock.h"
#include "../../core/dpid/dpid.h"
#include <string>
#include <list>

namespace dragonpoop
{
    class renderer_model;
    class core;
    class model_component;
    class renderer_model_group;
    class renderer_writelock;
    class dpthread_lock;
    class renderer_model_material;
    struct dpxyz_f;
    class dpmatrix;
    
    class renderer_model_readlock : public shared_obj_readlock
    {
        
    private:
        
        renderer_model *t;
        
    protected:
        
        //ctor
        renderer_model_readlock( renderer_model *t, dpmutex_readlock *l );
        //dtor
        virtual ~renderer_model_readlock( void );
        
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
        //sync model instance with changes
        void sync( void );
        //render model
        void render( dpthread_lock *thd, renderer_writelock *r, bool doGui, dpmatrix *m_world );
        //find material
        renderer_model_material *findMaterial( dpid id );
        //get materials
        void getMaterials( std::list<renderer_model_material *> *l );
        //get size
        void getSize( dpxyz_f *x );
        //get center
        void getCenter( dpxyz_f *x );

        friend class renderer_model;
    };
    
};

#endif