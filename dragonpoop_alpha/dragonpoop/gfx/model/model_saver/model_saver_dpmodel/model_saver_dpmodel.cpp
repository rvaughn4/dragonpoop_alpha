
#include "model_saver_dpmodel.h"
#include "model_saver_dpmodel_state_write.h"
#include "../../../../core/shared_obj/shared_obj_guard.h"
#include "../../model_ref.h"
#include "../../model_writelock.h"

namespace dragonpoop
{
    
    //ctor
    model_saver_dpmodel::model_saver_dpmodel( core *c, model_ref *m, std::string *pname, std::string *fname ) : model_saver( c, m, pname, fname )
    {
        shared_obj_guard o;
        model_writelock *l;
        
        this->m = 0;
        l = (model_writelock *)o.tryWriteLock( m, 300 );
        if( !l )
            return;
        
        this->m = (model_ref *)l->getRef();
    }
    
    //dtor
    model_saver_dpmodel::~model_saver_dpmodel( void )
    {
        delete this->m;
    }
    
    //generate second state
    model_saver_state *model_saver_dpmodel::genState( dpbuffer *b )
    {
        return new model_saver_dpmodel_state_write( b, this->m );
    }
    
};
