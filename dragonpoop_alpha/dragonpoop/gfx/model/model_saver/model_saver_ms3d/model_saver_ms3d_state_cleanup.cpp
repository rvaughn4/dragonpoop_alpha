
#include "model_saver_ms3d_state_cleanup.h"
#include "../../../../core/dpbuffer/dpbuffer.h"
#include "../model_saver_state_fail.h"
#include "../model_saver_state_sucess.h"
#include "../model_saver_writelock.h"
#include "model_saver_ms3d.h"
#include "../../model_ref.h"

namespace dragonpoop
{
    
    //ctor
    model_saver_ms3d_state_cleanup::model_saver_ms3d_state_cleanup( dpbuffer *b, model_ref *m, bool bWasSucess )
    {
        this->b = b;
        this->m = m;
        this->bWasSucess = bWasSucess;
    }
    
    //dtor
    model_saver_ms3d_state_cleanup::~model_saver_ms3d_state_cleanup( void )
    {
        
    }
    
    //run state, returns next state
    model_saver_state *model_saver_ms3d_state_cleanup::run( dpthread_lock *thd, model_saver_writelock *ml )
    {
        std::vector<ms3d_model_vertex_m> *v;
        std::vector<ms3d_model_triangle_m> *t;
        std::vector<ms3d_model_group_m> *g;
        std::vector<ms3d_model_material_m> *mt;
        std::vector<ms3d_model_joint_m> *j;
        std::vector<ms3d_model_frame> *f;
        model_saver_ms3d *mld;
        
        
        mld = (model_saver_ms3d *)ml->getSaver();
        
        v = mld->verts;
        delete v;
        
        t = mld->tris;
        delete t;
        
        g = mld->groups;
        delete g;
        
        mt = mld->materials;
        delete mt;
        
        j = mld->joints;
        delete j;
        
        f = mld->frames;
        delete f;
        
        delete this->m;
        
        if( this->bWasSucess )
            return new model_saver_state_sucess( this->b );
        return new model_saver_state_fail( this->b );
    }
    
};
