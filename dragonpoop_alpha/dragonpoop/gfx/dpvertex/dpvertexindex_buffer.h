
#ifndef dragonpoop_dpvertexindex_buffer_h
#define dragonpoop_dpvertexindex_buffer_h

#include "dpvertex_buffer.h"
#include "dpindex_buffer.h"

namespace dragonpoop
{

    class dpvertexindex_buffer
    {

    private:

        dpvertex_buffer vb;
        dpindex_buffer ib;

    protected:

    public:

        //ctor
        dpvertexindex_buffer( void );
        //dtor
        virtual ~dpvertexindex_buffer( void );
        //copy
        void copy( dpvertexindex_buffer *c );
        //append buffer to this one
        void append( dpvertexindex_buffer *c );
        //clear
        void clear( void );
        //add vertex
        void addVertex( dpvertex *v );
        //add vertex checking for unqiueness
        void addVertexUnique( dpvertex *v );
        //get vertex buffer
        dpvertex *getVertexBuffer( unsigned int *p_size );
        //get index buffer
        dpindex *getIndexBuffer( unsigned int *p_size );
        //get vertex buffer
        dpvertex_buffer *getVB( void );
        //get index buffer
        dpindex_buffer *getIB( void );

    };
    
};

#endif