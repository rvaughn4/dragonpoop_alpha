
#ifndef dragonpoop_dpid_h
#define dragonpoop_dpid_h

#include <stdint.h>

namespace dragonpoop
{

#pragma pack( 1 )
    struct dpid
    {
        uint32_t ticks;
        uint32_t epoch;
        uint32_t counter;
        uint32_t addr;
        uint32_t randno;
    };
#pragma pack()

    //test dpid for zero
    bool dpid_isZero( dpid *i );
    //zero out dpid
    void dpid_zero( dpid *i );
    //compare dpids
    bool dpid_compare( dpid *a, dpid *b );
    //copy dpids
    void dpid_copy( dpid *src, dpid *dst );

}

#endif