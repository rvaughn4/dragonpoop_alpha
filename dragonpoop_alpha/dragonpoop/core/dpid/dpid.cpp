
#include "dpid.h"

namespace dragonpoop
{

    //test dpid for zero
    bool dpid_isZero( dpid *i )
    {
        return !i->addr && !i->counter && !i->epoch && !i->ticks && !i->randno;
    }

    //zero out dpid
    void dpid_zero( dpid *i )
    {
        i->addr = i->counter = i->epoch = i->ticks = i->randno = 0;
    }

    //compare dpids
    bool dpid_compare( dpid *a, dpid *b )
    {
        if( a->addr != b->addr )
            return 0;
        if( a->counter != b->counter )
            return 0;
        if( a->epoch != b->epoch )
            return 0;
        if( a->ticks != b->ticks )
            return 0;
        if( a->randno != b->randno )
            return 0;
        return 1;
    }

    //copy dpids
    void dpid_copy( dpid *src, dpid *dst )
    {
        dst->addr = src->addr;
        dst->counter = src->counter;
        dst->epoch = src->epoch;
        dst->randno = src->randno;
        dst->ticks = src->ticks;
    }

}

