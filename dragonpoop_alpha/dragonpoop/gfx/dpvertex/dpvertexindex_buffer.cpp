
#include "dpvertexindex_buffer.h"

namespace dragonpoop
{

    //ctor
    dpvertexindex_buffer::dpvertexindex_buffer( void )
    {

    }

    //dtor
    dpvertexindex_buffer::~dpvertexindex_buffer( void )
    {

    }

    //copy
    void dpvertexindex_buffer::copy( dpvertexindex_buffer *c )
    {
        this->vb.copy( &c->vb );
        this->ib.copy( &c->ib );
    }

    //clear
    void dpvertexindex_buffer::clear( void )
    {
        this->vb.clear();
        this->ib.clear();
    }

    //add vertex
    void dpvertexindex_buffer::addVertex( dpvertex *v, dpid id )
    {
        this->ib.addIndex( &this->vb, v, id );
    }

    //get vertex buffer
    dpvertex *dpvertexindex_buffer::getVertexBuffer( unsigned int *p_size )
    {
        if( p_size )
            *p_size = this->vb.getSize();
        return this->vb.getBuffer();
    }

    //get index buffer
    dpindex *dpvertexindex_buffer::getIndexBuffer( unsigned int *p_size )
    {
        if( p_size )
            *p_size = this->ib.getSize();
        return this->ib.getBuffer();
    }

};
