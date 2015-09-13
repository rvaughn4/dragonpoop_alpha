
#include "model_saver.h"
#include "model_saver_readlock.h"
#include "model_saver_writelock.h"
#include "model_saver_ref.h"
#include "../model_ref.h"
#include "../model_writelock.h"
#include "../../../core/core.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include "../../../core/dptask/dptask.h"
#include "../../../core/dptaskpool/dptaskpool_writelock.h"
#include "model_saver_state.h"
#include "model_saver_state_fail.h"
#include "model_saver_state_begin.h"
#include "model_saver_ms3d/model_saver_ms3d.h"

#include <stdlib.h>

namespace dragonpoop
{
    
    //ctor
    model_saver::model_saver( core *c,  model_ref *m, std::string *pname, std::string *fname ) : shared_obj( c->getMutexMaster() )
    {
        shared_obj_guard o;
        model_writelock *ml;
        
        this->bIsRun = 1;
        this->bWasOpen = 1;
        
        this->buffer = 0;
        this->fname.assign( *fname );
        this->pname.assign( *pname );
        this->cs = new model_saver_state_begin();
        
        ml = (model_writelock *)o.writeLock( m );
        this->m = (model_ref *)ml->getRef();
    }
    
    //dtor
    model_saver::~model_saver( void )
    {
        if( this->cs )
            delete this->cs;
        if( this->buffer )
            free( this->buffer );
        
        delete this->m;
    }
    
    //load model from file
    model_saver *model_saver::saveFile( core *c, model_ref *m, const char *pname, const char *fname )
    {
        std::string sname, sext, spath;
        model_saver *l;
        unsigned int l_dot, l_end;
        
        sname.assign( fname );
        spath.assign( pname );
        
        l_end = (unsigned int)sname.size();
        l_dot = (unsigned int)sname.rfind( "." );
        if( l_dot >= l_end - 1 )
            return 0;
        
        sext = sname.substr( l_dot + 1 );
        
        l = 0;
        
        if( sext.compare( "ms3d" ) == 0 )
            l = new model_saver_ms3d( c, m, &spath, &sname );
        
        return l;
    }
    
    //generate read lock
    shared_obj_readlock *model_saver::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new model_saver_readlock( (model_saver *)p, l );
    }
    
    //generate write lock
    shared_obj_writelock *model_saver::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new model_saver_writelock( (model_saver *)p, l );
    }
    
    //generate ref
    shared_obj_ref *model_saver::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new model_saver_ref( (model_saver *)p, k );
    }
    
    //run model saver from task
    void model_saver::run( dpthread_lock *thd, model_saver_writelock *g )
    {
        model_saver_state *ns;
        
        if( !this->cs )
            return;
        ns = this->cs->run( thd, g );
        if( ns )
        {
            delete this->cs;
            this->cs = ns;
        }
    }
    
    //generate second state
    model_saver_state *model_saver::genState( dpbuffer *b )
    {
        return new model_saver_state_fail( b );
    }
    
    //returns true if sucessful
    bool model_saver::wasSucessful( void )
    {
        return this->bWasOpen;
    }
    
    //returns true if running
    bool model_saver::isRunning( void )
    {
        return this->bIsRun;
    }
    
    //returns model
    model_ref *model_saver::getModel( void )
    {
        model_writelock *l;
        shared_obj_guard o;
        
        l = (model_writelock *)o.writeLock( this->m );
        if( !l )
            return 0;
        return (model_ref *)l->getRef();
    }
    
};
