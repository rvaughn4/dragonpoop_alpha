
#include "dpindex_buffer.h"
#include "dpvertex_buffer.h"

#include <stdlib.h>
#include <string.h>

namespace dragonpoop
{

    //ctor
    dpindex_buffer::dpindex_buffer( void )
    {
        this->buffer.cnt = 0;
        this->buffer.max = 0;
        this->buffer.ptr = 0;
    }

    //dtor
    dpindex_buffer::~dpindex_buffer( void )
    {
        this->clear();
    }

    //copy
    void dpindex_buffer::copy( dpindex_buffer *c )
    {
        unsigned int i, sz;
        dpindex *b, *p;

        this->clear();
        sz = c->getSize();
        b = c->getBuffer();

        for( i = 0; i < sz; i++ )
        {
            p = &b[ i ];
            this->addIndex( p );
        }
    }

    //get size
    unsigned int dpindex_buffer::getSize( void )
    {
        if( !this->buffer.ptr )
            return 0;
        return this->buffer.cnt;
    }

    //get buffer
    dpindex *dpindex_buffer::getBuffer( void )
    {
        return this->buffer.ptr;
    }

    //add index
    void dpindex_buffer::addIndex( dpindex *i )
    {
        unsigned int nc, nm;
        dpindex *nb;

        nc = this->buffer.cnt + 1;
        if( !this->buffer.ptr || nc >= this->buffer.max )
        {
            nm = nc * 2 + 3;
            nb = (dpindex *)malloc( sizeof(dpindex) * nm );
            if( !nb )
                return;
            if( this->buffer.ptr )
            {
                memcpy( nb, this->buffer.ptr, sizeof(dpindex) * this->buffer.cnt );
                free( this->buffer.ptr );
            }
            this->buffer.ptr = nb;
            this->buffer.max = nm;
        }

        this->buffer.ptr[ nc - 1 ] = *i;
        this->buffer.cnt = nc;
    }

    //add index by vertex id
    void dpindex_buffer::addIndex( dpvertex_buffer *vb, dpvertex *v, dpid id )
    {
        dpindex i;

        i.id = id;
        i.i = vb->addVertex( v );

        this->addIndex( &i );
    }

    //clear
    void dpindex_buffer::clear( void )
    {
        if( this->buffer.ptr )
            free( this->buffer.ptr );
        this->buffer.cnt = 0;
        this->buffer.max = 0;
        this->buffer.ptr = 0;
    }

};
