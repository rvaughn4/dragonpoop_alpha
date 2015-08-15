
#ifndef dragonpoop_model_instance_joint_h
#define dragonpoop_model_instance_joint_h

#include "../../model_joint/model_joint.h"

namespace dragonpoop
{
    
    class model_instance_joint : public model_component
    {
        
    private:
        
        dpxyzw pos, rot;
        
    protected:
        
    public:
        
        //ctor
        model_instance_joint( model_joint *j );
        //dtor
        virtual ~model_instance_joint( void );
        //set position
        void setPosition( dpxyzw *x );
        //get position
        void getPosition( dpxyzw *x );
        //set rotation
        void setRotation( dpxyzw *x );
        //get rotation
        void getRotation( dpxyzw *x );
        
    };
    
};

#endif