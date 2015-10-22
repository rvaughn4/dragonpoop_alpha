
#include "render_api_context.h"
#include "render_api_ref.h"
#include "render_api_writelock.h"
#include "render_api_context_ref.h"
#include "render_api_context_readlock.h"
#include "render_api_context_writelock.h"
#include "render_api_commandlist.h"
#include "render_api_commandlist_ref.h"
#include "render_api_commandlist_writelock.h"
#include "../../../core/shared_obj/shared_obj_guard.h"

namespace dragonpoop
{

    //ctor
    render_api_context::render_api_context( render_api_writelock *r, dpmutex_master *mm ) : shared_obj( mm )
    {
        this->r = (render_api_ref *)r->getRef();
        this->mm = mm;
    }
    
    //dtor
    render_api_context::~render_api_context( void )
    {
        shared_obj_guard o;

        o.tryWriteLock( this, 5000, "render_api_context::~render_api_context" );
        o.unlock();
        this->unlink();
        
        this->deleteCmdLists();
        delete this->r;
    }
    
    //generate read lock
    shared_obj_readlock *render_api_context::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new render_api_context_readlock( (render_api_context *)p, l );
    }
    
    //generate write lock
    shared_obj_writelock *render_api_context::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new render_api_context_writelock( (render_api_context *)p, l );
    }
    
    //generate ref
    shared_obj_ref *render_api_context::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new render_api_context_ref( (render_api_context *)p, k );
    }

    //generate commandlist
    render_api_commandlist *render_api_context::genCmdList( render_api_context_writelock *cl, dpmutex_master *mm )
    {
        return 0;//new render_api_commandlist( cl, mm );
    }
    
    //make commandlist
    render_api_commandlist_ref *render_api_context::makeCmdList( render_api_context_writelock *cl )
    {
     /*   shared_obj_guard o;
        render_api_commandlist *c;
        render_api_commandlist_writelock *l;
        
        
        c = this->genCmdList( cl, this->mm );
       */
        return 0;
    }
    
    //delete commandlists
    void render_api_context::deleteCmdLists( void )
    {
        
    }
    
};
