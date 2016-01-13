
#ifndef dragonpoop_render_api_writelock_h
#define dragonpoop_render_api_writelock_h

#include "../../../core/shared_obj/shared_obj_writelock.h"
#include "../window/window.h"

namespace dragonpoop
{
    class render_api;
    class render_api_context_ref;
    class render_api_context_writelock;

    class render_api_writelock : public shared_obj_writelock
    {

    private:

        render_api *t;

    protected:

        //ctor
        render_api_writelock( render_api *t, dpmutex_writelock *l );
        //dtor
        virtual ~render_api_writelock( void );

    public:

        //run api
        void run( void );
        //returns true if window is open
        bool isOpen( void );
        //returns window width
        float getWidth( void );
        //returns window height
        float getHeight( void );
        //make context
        render_api_context_ref *getContext( void );
        //delete contexts
        void deleteContexts( void );

        friend class render_api;
    };

};

#endif
