
#ifndef dragonpoop_model_instance_readlock_h
#define dragonpoop_model_instance_readlock_h

#include "../../../core/shared_obj/shared_obj_readlock.h"
#include "../../../core/dpid/dpid.h"
#include <list>

namespace dragonpoop
{
    class model_instance;
    class core;
    class model_component;
    class model_instance_group;
    class dpvertexindex_buffer;
    class model_instance_animation;
    class model_instance_joint;
    
    class model_instance_readlock : public shared_obj_readlock
    {
        
    private:
        
        model_instance *t;
        
    protected:
        
        //ctor
        model_instance_readlock( model_instance *t, dpmutex_readlock *l );
        //dtor
        virtual ~model_instance_readlock( void );
        
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
        //populate vertex buffer for rendering
        void fillVertexBuffer( dpid group_id, dpvertexindex_buffer *vb );
        //find animation
        model_instance_animation *findAnimation( dpid id );
        //get animations
        void getAnimations( std::list<model_instance_animation *> *l );
        //get joints
        void getJoints( std::list<model_instance_joint *> *l );
        //sync
        void sync( void );
        //get start time
        uint64_t getStartTime( void );
        //get end time
        uint64_t getEndTime( void );

        friend class model_instance;
    };
    
};

#endif