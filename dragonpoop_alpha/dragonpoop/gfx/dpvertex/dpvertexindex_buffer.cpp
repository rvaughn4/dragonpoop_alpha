
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
    void dpvertexindex_buffer::addVertex( dpvertex *v )
    {
        this->ib.addIndex( &this->vb, v );
    }

    //add vertex
    void dpvertexindex_buffer::addVertexUnique( dpvertex *v )
    {
        dpindex i;
        
        if( this->ib.findIndex( &this->vb, v, &i ) )
            this->ib.addIndex( &i );
        else
            this->ib.addIndex( &this->vb, v );
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
    
    //append buffer to this one
    void dpvertexindex_buffer::append( dpvertexindex_buffer *c )
    {
        dpindex *ix, *ip;
        dpvertex *v, *vp;
        unsigned int ii, is, vi, vs;
        
        ip = c->getIndexBuffer( &is );
        vp = c->getVertexBuffer( &vs );
        
        for( ii = 0 ; ii < is; ii++ )
        {
            ix = &ip[ ii ];
            vi = ix->i;
            if( vi >= vs )
                continue;
            v = &vp[ vi ];
            this->addVertex( v );
        }
    }
    
    //get vertex buffer
    dpvertex_buffer *dpvertexindex_buffer::getVB( void )
    {
        return &this->vb;
    }
    
    //get index buffer
    dpindex_buffer *dpvertexindex_buffer::getIB( void )
    {
        return &this->ib;
    }
    
};
