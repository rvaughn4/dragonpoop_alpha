
#ifndef dragonpoop_model_frame_joint_h
#define dragonpoop_model_frame_joint_h

#include "../model_component/model_components.h"
#include "../../dpvertex/dpxyz_f.h"

namespace dragonpoop
{
    
    class model_frame_joint : public model_component
    {
        
    private:
        
        dpxyz_f trans, rot;
        dpid frame_id, joint_id;
        
    protected:
        
    public:
        
        //ctor
        model_frame_joint( dpid id, dpid frame_id, dpid joint_id );
        //dtor
        virtual ~model_frame_joint( void );
        //return frame id
        dpid getFrameId( void );
        //return joint id
        dpid getJointId( void );
        //set rotation
        void setRotation( dpxyz_f *x );
        //get rotation
        void getRotation( dpxyz_f *x );
        //set translation
        void setTranslation( dpxyz_f *x );
        //get translation
        void getTranslation( dpxyz_f *x );
        //returns true if has parent
        virtual bool hasParent( dpid id );
        
    };
    
};

#endif