
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
    void dpindex_buffer::addIndex( dpvertex_buffer *vb, dpvertex *v )
    {
        dpindex i;

        i.i = vb->addVertex( v );

        this->addIndex( &i );
    }

    //find index having identical vertex
    bool dpindex_buffer::findIndex( dpvertex_buffer *vb, dpvertex *v, dpindex *i )
    {
        dpvertex *vp, *vi;
        unsigned int iv, sv;
        
        vp = vb->getBuffer();
        sv = vb->getSize();
        
        for( iv = 0; iv < sv; iv++ )
        {
            vi = &vp[ iv ];
            
            if(
                v->start.pos.x != vi->start.pos.x
               ||
               v->start.pos.y != vi->start.pos.y
               ||
               v->start.pos.z != vi->start.pos.z
               ||
               v->start.pos.w != vi->start.pos.w
               )
                continue;
            
            if(
               v->end.pos.x != vi->end.pos.x
               ||
               v->end.pos.y != vi->end.pos.y
               ||
               v->end.pos.z != vi->end.pos.z
               ||
               v->end.pos.w != vi->end.pos.w
               )
                continue;
            
            if(
               v->start.normal.x != vi->start.normal.x
               ||
               v->start.normal.y != vi->start.normal.y
               ||
               v->start.normal.z != vi->start.normal.z
               ||
               v->start.normal.w != vi->start.normal.w
               )
                continue;
            
            if(
               v->end.normal.x != vi->end.normal.x
               ||
               v->end.normal.y != vi->end.normal.y
               ||
               v->end.normal.z != vi->end.normal.z
               ||
               v->end.normal.w != vi->end.normal.w
               )
                continue;
            
            if(
               v->start.texcoords[ 0 ].s != vi->start.texcoords[ 0 ].s
               ||
               v->start.texcoords[ 0 ].t != vi->start.texcoords[ 0 ].t
               ||
               v->start.texcoords[ 1 ].s != vi->start.texcoords[ 1 ].s
               ||
               v->start.texcoords[ 1 ].t != vi->start.texcoords[ 1 ].t
               )
                continue;
            
            if(
               v->end.texcoords[ 0 ].s != vi->end.texcoords[ 0 ].s
               ||
               v->end.texcoords[ 0 ].t != vi->end.texcoords[ 0 ].t
               ||
               v->end.texcoords[ 1 ].s != vi->end.texcoords[ 1 ].s
               ||
               v->end.texcoords[ 1 ].t != vi->end.texcoords[ 1 ].t
               )
                continue;
            
            i->i = iv;
            return 1;
        }

        return 0;
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
