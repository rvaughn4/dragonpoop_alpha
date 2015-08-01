#ifndef dragonpoop_ms3d_model_triangle_h
#define dragonpoop_ms3d_model_triangle_h

#include <stdint.h>
#include "../../../core/dpid/dpid.h"

namespace dragonpoop
{
    
#pragma pack( 1 )
    struct ms3d_model_triangle_section
    {
        uint16_t cnt;
    };
#pragma pack()
    
#pragma pack( 1 )
    struct ms3d_model_triangle
    {
        uint16_t flags;
        uint16_t verticies[3];
        struct
        {
            float x, y, z;
        } normals[ 3 ];
        float s[3]; ///WTF!!!!
        float t[3];
        uint8_t smoothingGroup;
        uint8_t groupIndex;
    };
#pragma pack()
    
#pragma pack( 1 )
    struct ms3d_model_triangle_m
    {
        ms3d_model_triangle f;
        dpid id;
        dpid group_id;
    };
#pragma pack()
    
}

#endif