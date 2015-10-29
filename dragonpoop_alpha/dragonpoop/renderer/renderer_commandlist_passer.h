
#ifndef dragonpoop_renderer_commandlist_passer_h
#define dragonpoop_renderer_commandlist_passer_h

#include "../core/shared_obj/shared_obj.h"

namespace dragonpoop
{
    
    class render_api_commandlist_ref;
    
    class renderer_commandlist_passer : public shared_obj
    {
        
    private:
        
        std::atomic<render_api_commandlist_ref *> *rmodel, *rgui;
        
        
    protected:
        
        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //set model commandlist
        void setModel( render_api_commandlist_ref *r );
        //get model commandlist
        render_api_commandlist_ref *getModel( void );
        
    public:
        
        //ctor
        renderer_commandlist_passer( dpmutex_master *mm );
        //dtor
        virtual ~renderer_commandlist_passer( void );
        

    };
    
};

#endif