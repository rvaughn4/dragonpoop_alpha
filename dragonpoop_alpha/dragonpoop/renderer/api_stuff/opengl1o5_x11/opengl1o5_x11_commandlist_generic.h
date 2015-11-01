
#ifndef dragonpoop_opengl1o5_x11_commandlist_generic_h
#define dragonpoop_opengl1o5_x11_commandlist_generic_h

#include "opengl1o5_x11.h"
#include "../render_api/render_api_commandlist_generic.h"

namespace dragonpoop
{
    
    class opengl1o5_x11_commandlist_generic : public render_api_commandlist_generic
    {
        
    private:
        
    protected:
        
    public:
        
        //ctor
        opengl1o5_x11_commandlist_generic( dpmutex_master *mm );
        //dtor
        virtual ~opengl1o5_x11_commandlist_generic( void );
        
        friend class opengl1o5_x11_context;
    };
    
};

#endif