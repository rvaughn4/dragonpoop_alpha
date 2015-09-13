
#include "model_loader.h"
#include "model_loader_readlock.h"
#include "model_loader_writelock.h"
#include "model_loader_ref.h"
#include "../model_ref.h"
#include "../model_writelock.h"
#include "../../../core/core.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include "../../../core/dptask/dptask.h"
#include "../../../core/dptaskpool/dptaskpool_writelock.h"
#include "model_loader_state.h"
#include "model_loader_state_fail.h"
#include "model_loader_state_openfile.h"
#include "model_loader_ms3d/model_loader_ms3d.h"

#include <stdlib.h>

namespace dragonpoop
{

    //ctor
    model_loader::model_loader( core *c, model_ref *m, std::string *pname, std::string *fname ) : shared_obj( c->getMutexMaster() )
    {
        shared_obj_guard o;
        model_writelock *ml;

        this->bIsRun = 1;
        this->bWasOpen = 1;

        this->buffer = 0;
        this->fname.assign( *fname );
        this->pname.assign( *pname );
        this->cs = new model_loader_state_openfile();

        ml = (model_writelock *)o.writeLock( m );
        this->m = (model_ref *)ml->getRef();
    }

    //dtor
    model_loader::~model_loader( void )
    {
        if( this->cs )
            delete this->cs;
        if( this->buffer )
            free( this->buffer );

        delete this->m;
    }

    //load model from file
    model_loader *model_loader::loadFile( core *c, model_ref *m, const char *path_name, const char *fname )
    {
        std::string sname, sext, spath;
        model_loader *l;
        unsigned int l_dot, l_end;

        spath.assign( path_name );
        sname.assign( fname );

        l_end = (unsigned int)sname.size();
        l_dot = (unsigned int)sname.rfind( "." );
        if( l_dot >= l_end - 1 )
            return 0;

        sext = sname.substr( l_dot + 1 );

        l = 0;

        if( sext.compare( "ms3d" ) == 0 )
            l = new model_loader_ms3d( c, m, &spath, &sname );

        return l;
    }

    //generate read lock
    shared_obj_readlock *model_loader::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new model_loader_readlock( (model_loader *)p, l );
    }

    //generate write lock
    shared_obj_writelock *model_loader::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new model_loader_writelock( (model_loader *)p, l );
    }

    //generate ref
    shared_obj_ref *model_loader::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new model_loader_ref( (model_loader *)p, k );
    }

    //run model loader from task
    void model_loader::run( dpthread_lock *thd, model_loader_writelock *g )
    {
        model_loader_state *ns;

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
    model_loader_state *model_loader::genState( dpbuffer *b )
    {
        return new model_loader_state_fail( b );
    }

    //returns true if sucessful
    bool model_loader::wasSucessful( void )
    {
        return this->bWasOpen;
    }

    //returns true if running
    bool model_loader::isRunning( void )
    {
        return this->bIsRun;
    }

    //returns model
    model_ref *model_loader::getModel( void )
    {
        model_writelock *l;
        shared_obj_guard o;

        l = (model_writelock *)o.writeLock( this->m );
        if( !l )
            return 0;
        return (model_ref *)l->getRef();
    }

};
