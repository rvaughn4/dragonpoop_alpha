
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
        dpmatrix start, end, bone_start, bone_end;
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
        //return count
        uint16_t getCount( void );
        
    };
    
};

#endif