
#include "dpvertex_buffer_indextree.h"

namespace dragonpoop
{

    //ctor
    dpvertex_buffer_indextree::dpvertex_buffer_indextree( void )
    {

    }

    //dtor
    dpvertex_buffer_indextree::~dpvertex_buffer_indextree( void )
    {
        this->clear();
    }

    //find leaf
    bool dpvertex_buffer_indextree::findLeaf( dpvertex *v, uint16_t *p_offset )
    {
        union
        {
            uint64_t r;
            void *rp;
        };
        rp = this->bytetree::findLeaf( (char *)v, sizeof(dpvertex) );

        if( !r )
            return 0;
        r--;
        if( p_offset )
            *p_offset = r;

        return 1;
    }

    //add leaf
    void dpvertex_buffer_indextree::addLeaf( dpvertex *v, uint16_t offset )
    {
        union
        {
            uint64_t r;
            void *rp;
        };

        r = offset + 1;

        this->bytetree::addLeaf( (char *)v, sizeof(dpvertex), rp );
    }

};
