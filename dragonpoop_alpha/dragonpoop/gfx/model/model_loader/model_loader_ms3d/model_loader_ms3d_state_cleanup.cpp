
#include "model_loader_ms3d_state_cleanup.h"
#include "../../../../core/dpbuffer/dpbuffer.h"
#include "../model_loader_state_fail.h"
#include "../model_loader_state_sucess.h"
#include "../model_loader_writelock.h"
#include "model_loader_ms3d.h"
#include "../../model_ref.h"

namespace dragonpoop
{
    
    //ctor
    model_loader_ms3d_state_cleanup::model_loader_ms3d_state_cleanup( dpbuffer *b, model_ref *m, bool bWasSucess )
    {
        this->b = b;
        this->m = m;
        this->bWasSucess = bWasSucess;
    }
    
    //dtor
    model_loader_ms3d_state_cleanup::~model_loader_ms3d_state_cleanup( void )
    {
        
    }
    
    //run state, returns next state
    model_loader_state *model_loader_ms3d_state_cleanup::run( dpthread_lock *thd, model_loader_writelock *ml )
    {
        std::vector<ms3d_model_vertex_m> *v;
        std::vector<ms3d_model_triangle_m> *t;
        std::vector<ms3d_model_group_m> *g;
        std::vector<ms3d_model_material_m> *mt;
        std::vector<ms3d_model_joint_m> *j;
        model_loader_ms3d *mld;
        
        
        mld = (model_loader_ms3d *)ml->getLoader();
        
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
        
        delete this->m;
        
        if( this->bWasSucess )
            return new model_loader_state_sucess( this->b );
        return new model_loader_state_fail( this->b );
    }
    
};
