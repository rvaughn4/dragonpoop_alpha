
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
    
    //set shader command
    void render_api_commandlist_writelock::setShader( render_api_shader_ref *p )
    {
        this->t->setShader( p );
    }
    
    //set texture command
    void render_api_commandlist_writelock::setTexture( render_api_texture_ref *p, int level )
    {
        this->t->setTexture( p, level );
    }
    
    //set vertex buffer command
    void render_api_commandlist_writelock::setVertexBuffer( render_api_vertexbuffer_ref *p )
    {
        this->t->setVertexBuffer( p );
    }
    
    //set index buffer command
    void render_api_commandlist_writelock::setIndexBuffer( render_api_indexbuffer_ref *p )
    {
        this->t->setIndexBuffer( p );
    }
    
    //set matrix command
    void render_api_commandlist_writelock::setMatrix( dpmatrix *m )
    {
        this->t->setMatrix( m );
    }
    
    //set alpha command
    void render_api_commandlist_writelock::setAlpha( float a )
    {
        this->t->setAlpha( a );
    }
    
    //draw command
    void render_api_commandlist_writelock::draw( void )
    {
        this->t->draw();
    }
    
};
