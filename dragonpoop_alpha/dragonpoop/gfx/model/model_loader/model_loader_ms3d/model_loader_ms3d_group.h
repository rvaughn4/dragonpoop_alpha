#ifndef dragonpoop_ms3d_model_group_h
#define dragonpoop_ms3d_model_group_h

#include <stdint.h>
#include "../../../core/dpid/dpid.h"
#include <vector>
#include <string>

namespace dragonpoop
{
    
#pragma pack( 1 )
    struct ms3d_model_group_section
    {
        uint16_t cnt;
    };
#pragma pack()
    
#pragma pack( 1 )
    struct ms3d_model_group
    {
        uint8_t flags;
        int8_t name[ 32 ];
        uint16_t cntTriangles;
    };
#pragma pack()
    
#pragma pack( 1 )
    struct ms3d_model_group_triangle
    {
        uint16_t index;
    };
#pragma pack()
    
#pragma pack( 1 )
    struct ms3d_model_group_end
    {
        int8_t index;
    };
#pragma pack()
    
#pragma pack( 1 )
    struct ms3d_model_group_m
    {
        ms3d_model_group f;
        ms3d_model_group_end e;
        dpid id;
        std::vector<ms3d_model_group_triangle> triangles;
        std::string cmt;
    };
#pragma pack()
    
}

#endif