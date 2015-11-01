
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
        //returns true if mouse input is waiting
        bool hasMouseInput( void );
        //fetches mouse input
        bool getMouseInput( window_mouse_input *m );
        //adds mouse input
        void addMouseInput( window_mouse_input *m );
        //returns true if kb input is waiting
        bool hasKBInput( void );
        //fetches kb input
        bool getKBInput( window_kb_input *m );
        //adds kb input
        void addKBInput( window_kb_input *m );

        friend class render_api;
    };
    
};

#endif