
#ifndef dragonpoop_model_instance_joint_h
#define dragonpoop_model_instance_joint_h

#include "../../model_joint/model_joint.h"
#include "../../model_matrix/model_matrix.h"
#include "../../../dpvertex/dpxyz_f.h"

namespace dragonpoop
{
    
    class model_instance_writelock;
    
    class model_instance_joint : public model_component
    {
        
    private:
        
        dpxyz_f pos, rot, apos, arot;
        model_matrix local, global, local_bone, global_bone;

        bool isChained;
        dpid parent_id;
        int16_t index;
        
    protected:
       
    public:
        
        //ctor
        model_instance_joint( model_joint *j );
        //dtor
        virtual ~model_instance_joint( void );
        //set position
        void setPosition( dpxyz_f *x );
        //get position
        void getPosition( dpxyz_f *x );
        //set rotation
        void setRotation( dpxyz_f *x );
        //get rotation
        void getRotation( dpxyz_f *x );
        //set animation position
        void setAnimationPosition( dpxyz_f *x );
        //set animation rotation
        void setAnimationRotation( dpxyz_f *x );
        //reset matrix
        void reset( void );
        //set joint index
        void setIndex( int16_t i );
        //get joint index
        int16_t getIndex( void );
        //redo matrix
        void redoMatrix( model_instance_writelock *m );
        
    };
    
};

#endif