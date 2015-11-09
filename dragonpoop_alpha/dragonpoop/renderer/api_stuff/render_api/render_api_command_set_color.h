
#ifndef dragonpoop_render_api_command_set_color_h
#define dragonpoop_render_api_command_set_color_h

#include "render_api_command.h"

namespace dragonpoop
{
    
    class render_api_command_set_color : public render_api_command
    {
        
    private:
        
        float r, g, b;
        
    protected:
        
    public:
        
        //ctor
        render_api_command_set_color( render_api_commandlist *l, float r, float g, float b );
        //dtor
        virtual ~render_api_command_set_color( void );
        //compile command
        virtual void compile( render_api_context_writelock *ctx );
        
    };
    
};

#endif