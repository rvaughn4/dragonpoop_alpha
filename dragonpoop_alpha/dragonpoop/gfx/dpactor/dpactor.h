
#ifndef dragonpoop_dpactor_h
#define dragonpoop_dpactor_h

#include "../../core/shared_obj/shared_obj.h"
#include "../dpposition/dpposition.h"

namespace dragonpoop
{
    
    class dpthread_lock;
    class core;
    class dpactor_ref;
    class dpactor_readlock;
    class dpactor_writelock;
    class model_ref;
    class model_instance_ref;
    class gfx_ref;
    class dpactor_model_state;
    class model_loader_ref;
    
    class dpactor : public shared_obj
    {
        
    private:
        
        core *c;
        gfx_ref *g;
        struct
        {
            struct
            {
                model_ref *m;
                model_instance_ref *mi;
            } low, med, high;
        } models;
        dpposition pos;
        float dis;
        uint64_t t_dis, t_model_state, t_anim_state;
        dpactor_model_state *model_state;
        
    protected:
        
        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //run gui
        void run( dpthread_lock *thd, dpactor_writelock *g );
        //get position
        void getPosition( dpposition *p );
        //set position
        void setPosition( dpposition *p );
        //returns distance from camera
        float getCameraDistance( void );
        //returns distance from camera
        float _getCameraDistance( uint64_t t );
        //load low model
        bool loadLow( model_loader_ref **ldr );
        
    public:
        
        //ctor
        dpactor( core *c );
        //dtor
        virtual ~dpactor( void );
        //return core
        core *getCore( void );
        
        friend class dpactor_readlock;
        friend class dpactor_writelock;
        friend class dpactor_model_state;
        
    };
    
};

#endif