
#ifndef dragonpoop_model_joint_h
#define dragonpoop_model_joint_h

#include "../model_component/model_components.h"
#include "../../dpvertex/dpxyz_f.h"

namespace dragonpoop
{
    
    class model_joint : public model_component
    {
        
    private:
        
        dpxyz_f pos, rot;
        dpid parent_id;
        
    protected:
        
    public:
        
        //ctor
        model_joint( dpid id );
        //dtor
        virtual ~model_joint( void );
        //set position
        void setPosition( dpxyz_f *x );
        //get position
        void getPosition( dpxyz_f *x );
        //set rotation
        void setRotation( dpxyz_f *x );
        //get rotation
        void getRotation( dpxyz_f *x );
        //set parent
        void setParent( dpid id );
        //get parent
        dpid getParent( void );
        
    };
    
};

#endif