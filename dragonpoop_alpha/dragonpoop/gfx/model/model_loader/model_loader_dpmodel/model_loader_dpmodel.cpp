
#include "model_loader_dpmodel.h"
#include "model_loader_dpmodel_state_read.h"
#include "../../../../core/shared_obj/shared_obj_guard.h"
#include "../../model_ref.h"
#include "../../model_writelock.h"

namespace dragonpoop
{
    
    //ctor
    model_loader_dpmodel::model_loader_dpmodel( core *c, model_ref *m, std::string *pname, std::string *fname ) : model_loader( c, m, pname, fname )
    {
        shared_obj_guard o;
        model_writelock *l;
        
        this->m = 0;
        l = (model_writelock *)o.tryWriteLock( m, 300, "model_loader_dpmodel::model_loader_dpmodel" );
        if( !l )
            return;
        
        this->m = (model_ref *)l->getRef();
    }
    
    //dtor
    model_loader_dpmodel::~model_loader_dpmodel( void )
    {
        delete this->m;
    }
    
    //generate second state
    model_loader_state *model_loader_dpmodel::genState( dpbuffer *b )
    {
        return new model_loader_dpmodel_state_read( b, this->m );
    }
    
};
