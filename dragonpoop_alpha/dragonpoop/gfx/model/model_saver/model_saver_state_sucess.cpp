
#include "model_saver_state_sucess.h"
#include "model_saver_state_fail.h"
#include "model_saver_writelock.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include "../model_ref.h"
#include "../model_writelock.h"
#include <fstream>
#include "../../../core/dpbuffer/dpbuffer_dynamic.h"

namespace dragonpoop
{
    
    //ctor
    model_saver_state_sucess::model_saver_state_sucess( dpbuffer *b )
    {
        this->b = b;
    }
    
    //dtor
    model_saver_state_sucess::~model_saver_state_sucess( void )
    {
        
    }
    
    //run state, returns next state
    model_saver_state *model_saver_state_sucess::run( dpthread_lock *thd, model_saver_writelock *ml )
    {
        shared_obj_guard g;
        std::fstream f;
        unsigned int sz;
        char *o;
        
        if( !this->b )
            return 0;
        
        f.open( ml->t->fname.c_str(), f.binary | f.trunc | f.out );
        if( !f.is_open() )
            return new model_saver_state_fail( this->b );
        
        o = b->getBuffer();
        sz = b->getSize();
        f.write( o, sz );
        
        f.close();
        
        ml->t->bWasOpen = 1;
        ml->t->bIsRun = 0;
        
        delete this->b;
        this->b = 0;
        
        return 0;
    }
    
};
