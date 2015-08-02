
#include "model_loader_state_openfile.h"
#include "model_loader_writelock.h"
#include <fstream>
#include "model_loader_state_fail.h"
#include "../../../core/dpbuffer/dpbuffer_static.h"

namespace dragonpoop
{
    
    //ctor
    model_loader_state_openfile::model_loader_state_openfile( void )
    {
        
    }
    
    //dtor
    model_loader_state_openfile::~model_loader_state_openfile( void )
    {
        
    }
    
    //run state, returns next state
    model_loader_state *model_loader_state_openfile::run( dpthread_lock *thd, model_loader_writelock *ml )
    {
        std::fstream f;
        int sz;
        dpbuffer *b;
        
        f.open( ml->t->fname.c_str(), f.binary | f.in );
        
        if( !f.is_open() )
            return new model_loader_state_fail( 0 );
        
        f.seekg( 0, f.end );
        sz = (int)f.tellg();
        f.seekg( 0, f.beg );
        
        if( sz <= 0 )
            return new model_loader_state_fail( 0 );
        
        ml->t->size = sz;
        ml->t->buffer = (char *)malloc( sz );
        if( !ml->t->buffer )
            return new model_loader_state_fail( 0 );
        
        f.read( ml->t->buffer, sz );
        
        b = new dpbuffer_static( ml->t->buffer, ml->t->size );
        if( !b )
            return new model_loader_state_fail( 0 );
        
        return ml->t->genState( b );
    }

};
