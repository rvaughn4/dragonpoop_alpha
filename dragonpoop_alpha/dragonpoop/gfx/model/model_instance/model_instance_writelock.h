
#ifndef dragonpoop_model_instance_writelock_h
#define dragonpoop_model_instance_writelock_h

#include "../../../core/shared_obj/shared_obj_writelock.h"
#include "../../../core/dpid/dpid.h"
#include <list>

namespace dragonpoop
{
    class model_instance;
    class core;
    class model_component;
    class model_instance_group;
    class dpthread_lock;
    class model_writelock;
    class model_readlock;
    class renderer_model_instance;
    class dpvertexindex_buffer;
    class model_instance_animation;
    class model_instance_joint;
    struct model_gui_pos;
    
    class model_instance_writelock : public shared_obj_writelock
    {
        
    private:
        
        model_instance *t;
        
    protected:
        
        //ctor
        model_instance_writelock( model_instance *t, dpmutex_writelock *l );
        //dtor
        virtual ~model_instance_writelock( void );
        
    public:
        
        //return core
        core *getCore( void );
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
        model_instance_group *findGroup( dpid id );
        //get groups
        void getGroups( std::list<model_instance_group *> *l );
        //run model from task
        void run( dpthread_lock *thd, model_writelock *m );
        //sync
        void sync( void );
        //set renderer model
        void setRenderer( renderer_model_instance *r );
        //populate vertex buffer for rendering
        void fillVertexBuffer( dpid group_id, dpvertexindex_buffer *vb );
        //find animation
        model_instance_animation *findAnimation( dpid id );
        //find animation
        model_instance_animation *findAnimation( const char *cname );
        //play animation (returns animation instance id)
        dpid playAnimation( dpid id, model_readlock *ml, const char *cname, bool bDoRepeat, float speed );
        //play animation (returns animation instance id)
        dpid playAnimation( dpid id, model_writelock *ml, const char *cname, bool bDoRepeat, float speed );
        //stop animation
        void stopAnimation( const char *cname );
        //stop animation
        void stopAnimation( dpid id );
        //returns true if animation is playing
        bool isAnimationPlaying( const char *cname );
        //returns true if animation is playing
        bool isAnimationPlaying( dpid id );
        //change animation repeat
        void setAnimationRepeat( const char *cname, bool bDoRepeat );
        //change animation repeat
        void setAnimationRepeat( dpid id, bool bDoRepeat );
        //change animation speed
        void setAnimationSpeed( const char *cname, float s );
        //change animation speed
        void setAnimationSpeed( dpid id, float s );
        //stop all animations
        void stopAllAnimations( void );
        //get animations
        void getAnimations( std::list<model_instance_animation *> *l );
        //get joints
        void getJoints( std::list<model_instance_joint *> *l );
        //get start time
        uint64_t getStartTime( void );
        //get end time
        uint64_t getEndTime( void );
        //set position
        void setGuiPosition( float x, float y );
        //set width and height
        void setGuiSize( float w, float h );
        //get dimensions
        model_gui_pos *getGuiDimensions( void );
        //set gui mode
        void setGuiMode( bool b );
        //returns true if gui
        bool isGui( void );

        friend class model_instance;
    };
    
};

#endif