
#ifndef dragonpoop_render_api_command_set_alpha_h
#define dragonpoop_render_api_command_set_alpha_h

#include "render_api_command.h"

namespace dragonpoop
{
    
    class render_api_command_set_alpha : public render_api_command
    {
        
    private:
        
        float a;
        
    protected:
        
    public:
        
        //ctor
        render_api_command_set_alpha( render_api_commandlist *l, float a );
        //dtor
        virtual ~render_api_command_set_alpha( void );
        //compile command
        virtual void compile( render_api_context_writelock *ctx );
        
    };
    
};

#endif