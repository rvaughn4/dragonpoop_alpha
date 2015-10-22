
#ifndef dragonpoop_render_api_indexbuffer_h
#define dragonpoop_render_api_indexbuffer_h

#include "render_api.h"

namespace dragonpoop
{
    
    class render_api_ref;
    class render_api_writelock;
    
    class render_api_indexbuffer : public shared_obj
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
        render_api_indexbuffer( render_api_writelock *r, dpmutex_master *mm );
        //dtor
        virtual ~render_api_indexbuffer( void );
        
        friend class render_api_indexbuffer_ref;
        friend class render_api_indexbuffer_readlock;
        friend class render_api_indexbuffer_writelock;
    };
    
};

#endif