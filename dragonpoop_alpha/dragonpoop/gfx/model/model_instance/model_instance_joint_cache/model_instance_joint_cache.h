
#ifndef dragonpoop_model_instance_joint_cache_h
#define dragonpoop_model_instance_joint_cache_h

#include "../../model_matrix/model_matrix.h"
#include <atomic>

namespace dragonpoop
{
    
    class model_instance_joint;
    struct dpvertex;
    
    struct model_instance_joint_cache_element
    {
        int16_t id;
        struct
        {
            model_matrix_f animation, bones;
        } start, end;
    };
    
    class model_instance_joint_cache
    {
        
    private:
        
        std::atomic<model_instance_joint_cache_element *> buffer;
        std::atomic<unsigned int> size, cnt;
        
        //auto resize
        void autoResize( unsigned int ncnt );
        //get pointer to location
        model_instance_joint_cache_element *getElement( int16_t i );
        
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
        
    };
    
};

#endif