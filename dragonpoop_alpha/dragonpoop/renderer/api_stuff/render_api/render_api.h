
#ifndef dragonpoop_render_api_h
#define dragonpoop_render_api_h

#include "../../../core/shared_obj/shared_obj.h"
#include "../../../core/dpmutex/dpmutex_master.h"
#include "../window/window.h"

namespace dragonpoop
{
    
    class render_api : public shared_obj
    {
        
    private:
        
        window *w;
        
    protected:
        
        //returns window
        window *getWindow( void );
        
    public:
        
        
        //ctor
        render_api( window *w, dpmutex_master *mm );
        //dtor
        virtual ~render_api( void );
        //run api
        virtual void run( void );
        //returns true if window is open
        virtual bool isOpen( void );
        //returns window width
        float getWidth( void );
        //returns window height
        float getHeight( void );
        
    };
    
};

#endif