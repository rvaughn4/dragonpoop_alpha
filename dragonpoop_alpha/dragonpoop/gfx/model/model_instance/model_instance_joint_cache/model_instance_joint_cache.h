
#ifndef dragonpoop_model_instance_joint_cache_h
#define dragonpoop_model_instance_joint_cache_h

#include <atomic>
#include "../../../dpvertex/dpxyz_f.h"
#include "../../../dpmatrix/dpmatrixes.h"

namespace dragonpoop
{
    
    class model_instance_joint;
    struct dpvertex;
    
    struct model_instance_joint_cache_element
    {
        int16_t id, pid;
        dpmatrix bone_local, bone_global, anim_local, anim_global, inv_bone_global;
        dpmatrix up, down;
        dpxyz_f bone_pos, bone_rot, pos_start;
        dpxyz_f pos_end, rot_start, rot_end;
        bool wasUpdated;
    };
    
    class model_instance_joint_cache
    {
        
    private:
        
        std::atomic<model_instance_joint_cache_element *> buffer;
        std::atomic<unsigned int> size, cnt;
        
        //auto resize
        void autoResize( unsigned int ncnt );
        
    protected:
        
        //compute matrix for element, computing all parents first
        void computeMatrix( model_instance_joint_cache_element *e, float r );
        //do matrix up
        void doUpMatrix( model_instance_joint_cache_element *e, dpmatrix *m, float rs, float re );
        //do matrix down
        void doDownMatrix( model_instance_joint_cache_element *e, dpmatrix *m );

        //do angle lerp
        static void angleLerp( dpxyz_f *a, dpxyz_f *b, dpxyz_f *o, float rs, float re );
        //do position lerp
        static void posLerp( dpxyz_f *a, dpxyz_f *b, dpxyz_f *o, float rs, float re );
        
        
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
        void transform( dpvertex *v );
        //get pointer to location
        model_instance_joint_cache_element *getElement( int16_t i );
        //return count
        uint16_t getCount( void );
        //compute matrixes
        void updateMaticies( float r );
        
    };
    
};

#endif