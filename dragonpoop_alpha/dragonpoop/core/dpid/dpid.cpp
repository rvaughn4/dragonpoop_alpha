
#include "dpid.h"

namespace dragonpoop
{

    //test dpid for zero
    bool dpid_isZero( dpid *i )
    {
        return !i->addr && !i->counter && !i->epoch && !i->rdm;
    }

    //zero out dpid
    void dpid_zero( dpid *i )
    {
        i->addr = i->counter = i->epoch = i->rdm = 0;
    }

    //compare dpids
    bool dpid_compare( dpid *a, dpid *b )
    {
        if( a->rdm != b->rdm )
            return 0;
        if( a->addr != b->addr )
            return 0;
        if( a->counter != b->counter )
            return 0;
        if( a->epoch != b->epoch )
            return 0;
        return 1;
    }

    //copy dpids
    void dpid_copy( dpid *src, dpid *dst )
    {
        dst->addr = src->addr;
        dst->counter = src->counter;
        dst->epoch = src->epoch;
        dst->rdm = src->rdm;
    }

    //compare dpid
    int dpid_signed_compare( dpid *a, dpid *b )
    {
        if( a->rdm > b->rdm )
            return 1;
        if( a->rdm < b->rdm )
            return -1;
        
        if( a->counter > b->counter )
            return 1;
        if( a->counter < b->counter )
            return -1;

        if( a->epoch > b->epoch )
            return 1;
        if( a->epoch < b->epoch )
            return -1;

        if( a->addr > b->addr )
            return 1;
        if( a->addr < b->addr )
            return -1;
        
        return 0;
    }
    
    //returns all zeroes
    dpid dpid_null( void )
    {
        dpid r;
        dpid_zero( &r );
        return r;
    }
    
}

