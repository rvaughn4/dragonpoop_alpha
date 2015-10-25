
#include "render_api_writelock.h"
#include "render_api.h"

namespace dragonpoop
{
    
    //ctor
    render_api_writelock::render_api_writelock( render_api *t, dpmutex_writelock *l ) : shared_obj_writelock( t, l )
    {
        this->t = t;
    }
    
    //dtor
    render_api_writelock::~render_api_writelock( void )
    {
        
    }
    
    //run api
    void render_api_writelock::run( void )
    {
        this->t->run();
    }
    
    //returns true if window is open
    bool render_api_writelock::isOpen( void )
    {
        return this->t->isOpen();
    }
    
    //returns window width
    float render_api_writelock::getWidth( void )
    {
        return this->t->getWidth();
    }
    
    //returns window height
    float render_api_writelock::getHeight( void )
    {
        return this->t->getHeight();
    }
    
    //make context
    render_api_context_ref *render_api_writelock::getContext( void )
    {
        return this->t->getContext( this );
    }
    
    //delete contexts
    void render_api_writelock::deleteContexts( void )
    {
        this->t->deleteContexts();
    }
    
    //make shader
    render_api_shader_ref *render_api_writelock::makeShader( render_api_context_writelock *cl, const char *cname )
    {
        return this->t->makeShader( this, cl, cname );
    }
    
    //make texture
    render_api_texture_ref *render_api_writelock::makeTexture( render_api_context_writelock *cl, dpbitmap *bm )
    {
        return this->t->makeTexture( this, cl, bm );
    }
    
    //make vertex buffer
    render_api_vertexbuffer_ref *render_api_writelock::makeVertexBuffer( render_api_context_writelock *cl, dpvertex_buffer *vb )
    {
        return this->t->makeVertexBuffer( this, cl, vb );
    }
    
    //make index buffer
    render_api_indexbuffer_ref *render_api_writelock::makeIndexBuffer( render_api_context_writelock *cl, dpindex_buffer *vb )
    {
        return this->t->makeIndexBuffer( this, cl, vb );
    }
    
    //delete shaders
    void render_api_writelock::deleteShaders( void )
    {
        this->t->deleteShaders();
    }
    
    //delete textures
    void render_api_writelock::deleteTextures( void )
    {
        this->t->deleteTextures();
    }
    
    //delete vertex buffers
    void render_api_writelock::deleteVertexBuffers( void )
    {
        this->t->deleteVertexBuffers();
    }
    
    //delete index buffers
    void render_api_writelock::deleteIndexBuffers( void )
    {
        this->t->deleteIndexBuffers();
    }
    
    //returns true if mouse input is waiting
    bool render_api_writelock::hasMouseInput( void )
    {
        return this->t->hasMouseInput();
    }
    
    //fetches mouse input
    bool render_api_writelock::getMouseInput( window_mouse_input *m )
    {
        return this->t->getMouseInput( m );
    }
    
    //adds mouse input
    void render_api_writelock::addMouseInput( window_mouse_input *m )
    {
        this->t->addMouseInput( m );
    }
    
    //returns true if kb input is waiting
    bool render_api_writelock::hasKBInput( void )
    {
        return this->t->hasKBInput();
    }
    
    //fetches kb input
    bool render_api_writelock::getKBInput( window_kb_input *m )
    {
        return this->t->getKBInput( m );
    }
    
    //adds kb input
    void render_api_writelock::addKBInput( window_kb_input *m )
    {
        this->t->addKBInput( m );
    }
    
};
