
#ifndef dragonpoop_model_instance_joint_h
#define dragonpoop_model_instance_joint_h

#include "../../model_joint/model_joint.h"
#include "../../../dpmatrix/dpmatrix.h"
#include <atomic>

namespace dragonpoop
{
    
    class model_instance_writelock;
    struct model_instance_joint_cache_element;
    struct dpxyz_f;
    
    class model_instance_joint : public model_component
    {
        
    private:
        
        dpxyz_f pos, rot, apos, arot, apos_old, arot_old;
        dpid parent_id;
        int16_t index, p_index;
        
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
        void setStartPosition( dpxyz_f *x );
        //set animation rotation
        void setStartRotation( dpxyz_f *x );
        //set animation position
        void setStopPosition( dpxyz_f *x );
        //set animation rotation
        void setStopRotation( dpxyz_f *x );
        //set joint index
        void setIndex( int16_t i );
        //get joint index
        int16_t getIndex( void );
        //redo matrix
        void redoMatrix( model_instance_writelock *m );
        //populate joint cache entity
        void fillCacheEntity( model_instance_joint_cache_element *e );

    };
    
};

#endif