
#ifndef dragonpoop_dpvertex_h
#define dragonpoop_dpvertex_h

#include "../../core/dpid/dpid.h"
#include "dpxyz_f.h"
#include "dpst_f.h"
#include <stdint.h>

namespace dragonpoop
{
    
#define dpvertex_bones_size 4

#pragma pack( 1 )
    struct dpvertex
    {
        dpxyz_f pos, normal;
        dpst_f texcoords[ 2 ];
        struct
        {
            int16_t index;
            float w;
        } bones[ dpvertex_bones_size ];
    };
#pragma pack(  )

};

#endif