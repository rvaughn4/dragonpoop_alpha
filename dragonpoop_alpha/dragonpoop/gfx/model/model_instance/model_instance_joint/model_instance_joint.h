
#ifndef dragonpoop_model_instance_joint_h
#define dragonpoop_model_instance_joint_h

#include "../../model_joint/model_joint.h"
#include "../../../dpmatrix/dpmatrix.h"
#include <atomic>
#include <sstream>

namespace dragonpoop
{
    
    class model_instance_writelock;
    struct dpxyz_f;
    class model_writelock;
    class dpthread_lock;
    class model_instance_animation;
    
    class model_instance_joint : public model_component
    {
        
    private:
        
        struct
        {
            dpxyz_f pos, rot;
            uint64_t t;
        } orig, start, end;
        dpid parent_id;
        int16_t index;
        
        //blend end into start
        void blendStart( uint64_t t );
        //find lowest end time
        uint64_t findLowestEndTime( model_instance_writelock *m );
        //get pos and rot for the given end time from all animations
        void combineAllTransforms( uint64_t t, model_instance_writelock *mi, model_writelock *m, dpxyz_f *pos, dpxyz_f *rot );
        //get pos and rot for the given end time from all frame_joints for the given animation and end time
        void getTransforms( model_instance_animation *a, uint64_t t, model_instance_writelock *mi, model_writelock *m, dpxyz_f *pos, dpxyz_f *rot );
        //get pos and rot for the given frame
        void getTransforms( dpid frame_id, model_writelock *m, dpxyz_f *pos, dpxyz_f *rot );
        
    protected:
        
    public:
        
        //ctor
        model_instance_joint( model_joint *j, int16_t index );
        //dtor
        virtual ~model_instance_joint( void );
        //get position
        void getPosition( dpxyz_f *x );
        //get rotation
        void getRotation( dpxyz_f *x );
        //run animation
        void run( model_instance_writelock *mi, model_writelock *m, dpthread_lock *thd, std::stringstream *ss );
        //get start position, rotation, and time
        uint64_t getStartData( dpxyz_f *pos, dpxyz_f *rot );
        //get end position, rotation, and time
        uint64_t getEndData( dpxyz_f *pos, dpxyz_f *rot );
        //return index
        int16_t getIndex( void );

    };
    
};

#endif