
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
        //clear
        void clear( void );
        //add vertex
        void addVertex( dpvertex *v, dpid id );
        //get vertex buffer
        dpvertex *getVertexBuffer( unsigned int *p_size );
        //get index buffer
        dpindex *getIndexBuffer( unsigned int *p_size );

    };
    
};

#endif