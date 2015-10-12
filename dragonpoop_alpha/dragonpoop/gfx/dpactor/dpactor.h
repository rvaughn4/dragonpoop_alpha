
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
    class dpactor_animate_state;
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
        dpactor_animate_state *anim_state;
        
        //load model
        bool _load( model_ref **m, model_instance_ref **mi, model_loader_ref **ldr );
        //make model instance
        bool _make( model_ref **m, model_instance_ref **mi, dpthread_lock *thd );
        //destroy model instance
        void _kill( model_ref **m, model_instance_ref **mi );
        //stop all animations
        void _stopAnimations( model_instance_ref *mi );
        //play animation
        void _playAnimation( model_ref *m, model_instance_ref *mi, const char *cname, float spd, bool doRepeat );
        //returns true if animation is playing
        bool _isPlaying( model_instance_ref *mi, const char *cname );
        //convert animation name
        bool convertName( std::string *sin, std::string *sout );

    protected:
        
        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //run actor
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
        //make low model instance
        bool makeLow( dpthread_lock *thd );
        //destroy low
        void killLow( void );
        //load med model
        bool loadMed( model_loader_ref **ldr );
        //make med model instance
        bool makeMed( dpthread_lock *thd );
        //destroy med
        void killMed( void );
        //load high model
        bool loadHigh( model_loader_ref **ldr );
        //make high model instance
        bool makeHigh( dpthread_lock *thd );
        //destroy high
        void killHigh( void );
        //stop all animations
        void stopAnimations( void );
        //play animation
        void playAnimation( const char *cname, float spd, bool doRepeat );
        //returns true if animation is playing
        bool isPlaying( const char *cname );
        
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
        friend class dpactor_animate_state;
        
    };
    
};

#endif