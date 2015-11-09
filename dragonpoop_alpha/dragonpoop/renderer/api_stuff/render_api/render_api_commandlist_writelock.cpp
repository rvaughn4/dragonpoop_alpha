
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
    
    //compile command list
    bool render_api_commandlist_writelock::compile( render_api_context_writelock *ctx )
    {
        return this->t->compile( ctx );
    }
    
    //execute command list
    bool render_api_commandlist_writelock::execute( render_api_context_writelock *r, dpmatrix *m_world )
    {
        return this->t->execute( r, m_world );
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

    //set color command
    void render_api_commandlist_writelock::setColor( float r, float g, float b )
    {
        this->t->setColor( r, g, b );
    }
    
    //draw command
    void render_api_commandlist_writelock::draw( void )
    {
        this->t->draw();
    }
    
    //set position
    void render_api_commandlist_writelock::setPosition( dpposition *p )
    {
        this->t->setPosition( p );
    }
    
    //get position
    void render_api_commandlist_writelock::getPosition( dpposition *p )
    {
        this->t->getPosition( p );
    }
    
};
