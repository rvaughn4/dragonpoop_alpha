
#include "render_api_commandlist_writelock.h"
#include "render_api_commandlist.h"

namespace dragonpoop
{
    
    //ctor
    render_api_commandlist_writelock::render_api_commandlist_writelock( render_api_commandlist *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    render_api_commandlist_writelock::~render_api_commandlist_writelock( void )
    {
        
    }
    
    //returns true if ready to be executed
    bool render_api_commandlist_writelock::isReadyForExecute( void )
    {
        return this->t->isReadyForExecute();
    }
    
    //compile command list
    void render_api_commandlist_writelock::compile( render_api_context_writelock *ctx )
    {
        this->t->compile( ctx );
    }
    
    //execute command list
    void render_api_commandlist_writelock::execute( render_api_context_writelock *r )
    {
        this->t->execute( r );
    }
    
    //delete commands
    void render_api_commandlist_writelock::deleteCommands( void )
    {
        this->t->deleteCommands();
    }
    
};
