
#ifndef dragonpoop_renderer_model_instance_joint_h
#define dragonpoop_renderer_model_instance_joint_h

#include "../../../../gfx/model/model_component/model_components.h"
#include "../../../../gfx/dpvertex/dpxyz_f.h"
#include "../../../../gfx/dpmatrix/dpmatrixes.h"

namespace dragonpoop
{
    
    class model_instance_joint;
    class model_instance_writelock;
    class dpthread_lock;
    class renderer_model_instance_writelock;
    class renderer_model_instance_readlock;
    struct dpvertex;
    
    class renderer_model_instance_joint : public model_component
    {
        
    private:
        
        struct
        {
            dpxyz_f pos, rot;
            uint64_t t;
        } orig, start, end;
        dpid parent_id;
        int16_t index;
        
        dpmatrix bone_local, bone_global, bone_global_inv, anim_local, anim_global, anim_global_inv;
        uint64_t t_matrix;
 
    protected:
        
        //recompute matrix
        void __redoMatrix( renderer_model_instance_joint *p, uint64_t t );
        
    public:
        
        //ctor
        renderer_model_instance_joint( model_instance_writelock *ml, model_instance_joint *j, dpthread_lock *thd );
        //dtor
        virtual ~renderer_model_instance_joint( void );
        //sync with joint
        virtual void sync( model_instance_writelock *ml, model_instance_joint *j, dpthread_lock *thd );
        //recompute matrix
        void redoMatrix( renderer_model_instance_writelock *m, uint64_t t );
        //recompute matrix
        void redoMatrix( renderer_model_instance_readlock *m, uint64_t t );
        //transform vertex by joint matrixes
        void transform( dpvertex *v );
        //return index
        int16_t getIndex( void );
        
    };
    
};

#endif