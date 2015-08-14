
#ifndef dragonpoop_model_joint_h
#define dragonpoop_model_joint_h

#include "../model_component/model_components.h"
#include "../../dpvertex/dpxyzw.h"

namespace dragonpoop
{
    
    class model_joint : public model_component
    {
        
    private:
        
        dpxyzw pos, rot;
        
    protected:
        
    public:
        
        //ctor
        model_joint( dpid id );
        //dtor
        virtual ~model_joint( void );
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