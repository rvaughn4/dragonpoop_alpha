
#ifndef dragonpoop_model_instance_joint_cache_h
#define dragonpoop_model_instance_joint_cache_h

#include <atomic>
#include "../../../dpvertex/dpxyz_f.h"
#include "../../../dpmatrix/dpmatrix.h"

namespace dragonpoop
{
    
    class model_instance_joint;
    struct dpvertex;
    
    struct model_instance_joint_cache_element
    {
        int16_t id, pid;
        dpmatrix start, end;
    };
    
    class model_instance_joint_cache
    {
        
    private:
        
        std::atomic<model_instance_joint_cache_element *> buffer;
        std::atomic<unsigned int> size, cnt;
        
        //auto resize
        void autoResize( unsigned int ncnt );
        //transform vertex
        void transform( dpxyz_f *in, dpxyz_f *out_start, dpxyz_f *out_end, int16_t i );
        //transform vertex by bone
        void transformBone( dpxyz_f *out_start, dpxyz_f *out_end, model_instance_joint_cache_element *e );
        //transform vertex by undo bone
        void transformUndoBone( dpxyz_f *out_start, dpxyz_f *out_end, model_instance_joint_cache_element *e );
        //transform vertex by animation
        void transformAnimation( dpxyz_f *out_start, dpxyz_f *out_end, model_instance_joint_cache_element *e );
        
    protected:
        
    public:
        
        //ctor
        model_instance_joint_cache( void );
        //dtor
        virtual ~model_instance_joint_cache( void );
        //clear
        void clear( void );
        //add joint
        void addJoint( model_instance_joint *j );
        //transform vertex
        void transform( dpvertex *v, float ratio );
        //get pointer to location
        model_instance_joint_cache_element *getElement( int16_t i );
        
    };
    
};

#endif