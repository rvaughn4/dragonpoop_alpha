
#ifndef dragonpoop_model_loader_ms3d_state_make_frames_h
#define dragonpoop_model_loader_ms3d_state_make_frames_h

#include "../model_loader_state.h"
#include <string>
#include <vector>
#include "../../../dpmatrix/dpmatrixes.h"

namespace dragonpoop
{
    
    class dpbuffer;
    class model_ref;
    class dpbitmap;
    struct ms3d_model_frame;
    class model_writelock;
    struct ms3d_model_joint_keyframe;
    struct dpxyz_f;
    
    class model_loader_ms3d_state_make_frames : public model_loader_state
    {
        
    private:
        
        dpbuffer *b;
        model_ref *m;
        std::vector<float> frame_times;
        
    protected:
        
        //find unqiue frame times
        void findUnique( model_loader_writelock *ml );
        //find unique frame times in a joint
        void findUnique( model_loader_writelock *ml, std::vector<ms3d_model_joint_keyframe> *l );
        //make frames
        void makeFrames( dpthread_lock *thd, model_loader_writelock *ml );
        //make animation frames
        void makeAnimationFrames( dpthread_lock *thd, model_loader_writelock *ml, dpid anim_id, unsigned int start_frame, unsigned int end_frame );
        //make frame joints
        void makeFrameJoints( dpthread_lock *thd, model_loader_writelock *ml );
        //find xyz keyframe before and after time and interpolate
        void getKeyframe( float t, std::vector<ms3d_model_joint_keyframe> *l, dpxyz_f *x );
        //find quaternion before and after time and interpolate
        void getKeyframeQuat( float t, std::vector<ms3d_model_joint_keyframe> *l, dpxyz_f *x );
        //find xyz keyframe before time
        void getKeyframeBefore( float t, std::vector<ms3d_model_joint_keyframe> *l, ms3d_model_joint_keyframe *x );
        //find xyz keyframe after time
        void getKeyframeAfter( float t, std::vector<ms3d_model_joint_keyframe> *l, ms3d_model_joint_keyframe *x );
        //read text file and create animations
        void readAnimations( dpthread_lock *thd, model_loader_writelock *ml );
        
    public:
        
        //ctor
        model_loader_ms3d_state_make_frames( dpbuffer *b, model_ref *m );
        //dtor
        virtual ~model_loader_ms3d_state_make_frames( void );
        //run state, returns next state
        virtual model_loader_state *run( dpthread_lock *thd, model_loader_writelock *ml );
        
    };
    
};

#endif