#ifndef dragonpoop_ms3d_model_vertex_h
#define dragonpoop_ms3d_model_vertex_h

#include <stdint.h>
#include "../../../core/dpid/dpid.h"

namespace dragonpoop
{
    
#pragma pack( 1 )
    struct ms3d_model_vertex_section
    {
        uint16_t cnt;
    };
#pragma pack()
    
#pragma pack( 1 )
    struct ms3d_model_vertex
    {
        uint8_t flags;
        struct
        {
            float x, y, z;
        } position;
        int8_t boneId;
        uint8_t referenceCount;
    };
#pragma pack()
    
#pragma pack( 1 )
    struct ms3d_model_vertex_extra_section
    {
        int32_t version;
    };
#pragma pack()
    
#pragma pack( 1 )
    struct ms3d_model_vertex_extra_v1
    {
        int8_t bones[ 3 ];
        uint8_t weights[ 3 ];
    };
#pragma pack()
    
#pragma pack( 1 )
    struct ms3d_model_vertex_extra_v2
    {
        ms3d_model_vertex_extra_v1 v;
        uint32_t extra;
    };
#pragma pack()
    
#pragma pack( 1 )
    struct ms3d_model_vertex_m
    {
        ms3d_model_vertex f;
        struct
        {
            int8_t id;
            uint8_t weight;
        } bones[ 3 ];
        dpid id;
    };
#pragma pack()
    
}

#endif