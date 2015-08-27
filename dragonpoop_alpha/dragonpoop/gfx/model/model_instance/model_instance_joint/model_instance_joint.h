
#ifndef dragonpoop_model_instance_joint_h
#define dragonpoop_model_instance_joint_h

#include "../../model_joint/model_joint.h"
#include "../../../dpmatrix/dpmatrix.h"

namespace dragonpoop
{
    
    class model_instance_writelock;
    
    class model_instance_joint : public model_component
    {
        
    private:
        
        dpxyzw pos, rot, apos, arot;
        dpmatrix orig, chained;
        bool isChained;
        dpid parent_id;
        
    protected:
        
        //redo matrix
        void redoMatrix( model_instance_writelock *m );
        
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
        //set animation position
        void setAnimationPosition( dpxyzw *x );
        //set animation rotation
        void setAnimationRotation( dpxyzw *x );
        //transform using matrix
        void transform( model_instance_writelock *m, dpxyzw *x );
        //reset matrix
        void reset( void );
        
    };
    
};

#endif