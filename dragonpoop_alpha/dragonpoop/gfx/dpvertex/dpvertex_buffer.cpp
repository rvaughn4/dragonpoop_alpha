
#include "dpvertex_buffer.h"
#include <stdlib.h>
#include <string.h>

namespace dragonpoop
{

    //ctor
    dpvertex_buffer::dpvertex_buffer( void )
    {
        this->buffer.cnt = 0;
        this->buffer.max = 0;
        this->buffer.ptr = 0;
    }

    //dtor
    dpvertex_buffer::~dpvertex_buffer( void )
    {
        this->clear();
    }

    //copy
    void dpvertex_buffer::copy( dpvertex_buffer *c )
    {
        unsigned int i, sz;
        dpvertex *b, *p;

        this->clear();
        sz = c->getSize();
        b = c->getBuffer();

        for( i = 0; i < sz; i++ )
        {
            p = &b[ i ];
            this->addVertex( p );
        }
    }

    //get size
    unsigned int dpvertex_buffer::getSize( void )
    {
        if( !this->buffer.ptr )
            return 0;
        return this->buffer.cnt;
    }

    //get buffer
    dpvertex *dpvertex_buffer::getBuffer( void )
    {
        return this->buffer.ptr;
    }

    //add index
    uint16_t dpvertex_buffer::addVertex( dpvertex *i )
    {
        unsigned int nc, nm;
        dpvertex *nb;

        nc = this->buffer.cnt + 1;
        if( !this->buffer.ptr || nc >= this->buffer.max )
        {
            nm = nc * 2 + 3;
            nb = (dpvertex *)malloc( sizeof(dpvertex) * nm );
            if( !nb )
                return 0;
            if( this->buffer.ptr )
            {
                memcpy( nb, this->buffer.ptr, sizeof(dpvertex) * this->buffer.cnt );
                free( this->buffer.ptr );
            }
            this->buffer.ptr = nb;
            this->buffer.max = nm;
        }

        this->buffer.ptr[ nc - 1 ] = *i;
        this->buffer.cnt = nc;
        return (uint16_t)( nc - 1 );
    }

    //clear
    void dpvertex_buffer::clear( void )
    {
        if( this->buffer.ptr )
            free( this->buffer.ptr );
        this->buffer.cnt = 0;
        this->buffer.max = 0;
        this->buffer.ptr = 0;
    }

};
