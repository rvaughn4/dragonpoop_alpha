
#include "opengl1o5_x11_vertexbuffer.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include "opengl1o5_x11_vertexbuffer_readlock.h"

namespace dragonpoop
{

    //ctor
    opengl1o5_x11_vertexbuffer::opengl1o5_x11_vertexbuffer( render_api_context_writelock *c, dpmutex_master *mm, dpvertex_buffer *vb, bool bUseDl, opengl1o5_x11_functions *gl ) : render_api_vertexbuffer( c, mm, vb )
    {
        this->gl = gl;
        this->glvb = 0;

        if( this->gl->bUseVB && !bUseDl )
        {
            this->gl->glGenBuffers( 1, &this->glvb );
            if( this->glvb )
            {
                this->gl->glBindBuffer( GL_ARRAY_BUFFER, this->glvb );
                this->gl->glBufferData( GL_ARRAY_BUFFER, vb->getSize() * sizeof( dpvertex ), vb->getBuffer(), GL_STATIC_DRAW );

                if( this->gl->glGetError() != 0 )
                {
                    this->gl->glDeleteBuffers( 1, &this->glvb );
                    this->glvb = 0;
                }
                else
                    this->gl->glFlush();
            }
        }
        if( !this->glvb )
            this->vb.copy( vb );
    }

    //dtor
    opengl1o5_x11_vertexbuffer::~opengl1o5_x11_vertexbuffer( void )
    {
        shared_obj_guard o;

        o.tryWriteLock( this, 5000, "opengl1o5_x11_vertexbuffer::~opengl1o5_x11_vertexbuffer" );
        o.unlock();
        this->unlink();

        o.tryWriteLock( this, 5000, "opengl1o5_x11_vertexbuffer::~opengl1o5_x11_vertexbuffer" );
        if( this->glvb )
            this->gl->glDeleteBuffers( 1, &this->glvb );
        this->vb.clear();
        o.unlock();
    }

    //generate read lock
    shared_obj_readlock *opengl1o5_x11_vertexbuffer::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new opengl1o5_x11_vertexbuffer_readlock( (opengl1o5_x11_vertexbuffer *)p, l );
    }

    //return vb
    dpvertex_buffer *opengl1o5_x11_vertexbuffer::getBuffer( void )
    {
        return &this->vb;
    }

    //return vbo
    unsigned int opengl1o5_x11_vertexbuffer::getVBO( void )
    {
        return this->glvb;
    }

};
