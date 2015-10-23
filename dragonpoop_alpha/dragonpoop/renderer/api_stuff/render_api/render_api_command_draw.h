
#ifndef dragonpoop_render_api_command_draw_h
#define dragonpoop_render_api_command_draw_h

#include "render_api_command.h"

namespace dragonpoop
{
    
    class render_api_command_draw : public render_api_command
    {
        
    private:
        
    protected:
        
    public:
        
        //ctor
        render_api_command_draw( render_api_commandlist *l );
        //dtor
        virtual ~render_api_command_draw( void );
        //compile command
        virtual void compile( render_api_context_writelock *ctx );
        
    };
    
};

#endif