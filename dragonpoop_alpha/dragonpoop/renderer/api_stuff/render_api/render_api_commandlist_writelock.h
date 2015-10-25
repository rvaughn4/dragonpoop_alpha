
#ifndef dragonpoop_render_api_commandlist_writelock_h
#define dragonpoop_render_api_commandlist_writelock_h

#include "../../../core/shared_obj/shared_obj_writelock.h"

namespace dragonpoop
{
    class render_api_commandlist;
    class render_api_context_ref;
    class render_api_context_writelock;
    
    class render_api_commandlist_writelock : public shared_obj_writelock
    {
        
    private:
        
        render_api_commandlist *t;
        
    protected:
        
        //ctor
        render_api_commandlist_writelock( render_api_commandlist *t, dpmutex_writelock *l );
        //dtor
        virtual ~render_api_commandlist_writelock( void );
        
    public:
        
        //returns true if ready to be executed
        bool isReadyForExecute( void );
        //compile command list
        void compile( render_api_context_writelock *ctx );
        //execute command list
        void execute( render_api_context_writelock *r );
        //delete commands
        void deleteCommands( void );

        
        friend class render_api_commandlist;
    };
    
};

#endif