
#include "model_saver_dpmodel_state_write.h"
#include "../model_saver_writelock.h"
#include "../model_saver_state_fail.h"
#include "../model_saver_state_sucess.h"
#include "../../../../core/shared_obj/shared_obj_guard.h"
#include "../../model_ref.h"
#include "../../model_writelock.h"
#include "../../model_component/model_component.h"
#include <list>

namespace dragonpoop
{
    
    //ctor
    model_saver_dpmodel_state_write::model_saver_dpmodel_state_write( dpbuffer *b, model_ref *m )
    {
        this->b = b;
        this->m = m;
    }
    
    //dtor
    model_saver_dpmodel_state_write::~model_saver_dpmodel_state_write( void )
    {
        
    }
    
    //run state, returns next state
    model_saver_state *model_saver_dpmodel_state_write::run( dpthread_lock *thd, model_saver_writelock *ml )
    {
        shared_obj_guard o;
        model_writelock *m;
        std::list<model_component *> l;
        std::list<model_component *>::iterator i;
        model_component *c;
        
        if( !this->m )
            return new model_saver_state_fail( this->b );
        m = (model_writelock *)o.tryWriteLock( this->m, 300 );
        if( !m )
            return new model_saver_state_fail( this->b );
        
        if( !m->writeHeader( this->b ) )
            return new model_saver_state_fail( this->b );
        
        m->getComponents( &l );
        for( i = l.begin(); i != l.end(); ++i )
        {
            c = *i;
            if( !c->write( this->b ) )
                return new model_saver_state_fail( this->b );
        }

        o.unlock();
        return new model_saver_state_sucess( this->b );
    }

};
