
#ifndef dragonpoop_render_api_context_h
#define dragonpoop_render_api_context_h

#include "render_api.h"

namespace dragonpoop
{
    
    class render_api_ref;
    class render_api_writelock;
    
    class render_api_context : public shared_obj
    {
        
    private:
        
        render_api_ref *r;
        
    protected:
        
        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        
    public:
        
        //ctor
        render_api_context( render_api_writelock *r, dpmutex_master *mm );
        //dtor
        virtual ~render_api_context( void );
        
        friend class render_api_context_ref;
        friend class render_api_context_readlock;
        friend class render_api_context_writelock;
    };
    
};

#endif