
#ifndef dragonpoop_render_api_h
#define dragonpoop_render_api_h

#include "../../../core/shared_obj/shared_obj.h"
#include "../../../core/dpmutex/dpmutex_master.h"
#include "../window/window.h"
#include <list>

namespace dragonpoop
{

    class render_api_context;
    class render_api_context_ref;
    class render_api_writelock;
    class render_api_context_writelock;

    class render_api : public shared_obj
    {

    private:

        window *w;
        dpmutex_master *mm;
        std::list<render_api_context *> contexts;
        int crun;

        //run contexts
        void runContexts( void );

    protected:

        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //returns window
        window *getWindow( void );
        //run api
        virtual void run( void );
        //returns true if window is open
        virtual bool isOpen( void );
        //returns window width
        float getWidth( void );
        //returns window height
        float getHeight( void );
        //generate context
        virtual render_api_context *genContext( render_api_writelock *al, dpmutex_master *mm );
        //make context
        render_api_context_ref *getContext( render_api_writelock *al );
        //delete contexts
        void deleteContexts( void );

    public:

        //ctor
        render_api( window *w, dpmutex_master *mm );
        //dtor
        virtual ~render_api( void );

        friend class render_api_ref;
        friend class render_api_readlock;
        friend class render_api_writelock;
    };

};

#endif
