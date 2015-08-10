#ifndef dragonpoop_ms3d_model_material_h
#define dragonpoop_ms3d_model_material_h

#include <stdint.h>
#include "../../../../core/dpid/dpid.h"
#include <string>

namespace dragonpoop
{
    
#pragma pack( 1 )
    struct ms3d_model_material_section
    {
        uint16_t cnt;
    };
#pragma pack()
    
#pragma pack( 1 )
    struct ms3d_model_material
    {
        int8_t name[ 32 ];
        struct
        {
            struct
            {
                float r, g, b, a;
            } ambient, diffuse, specular, emissive;
        } colors;
        float shininess;
        float transparency;
        uint8_t mode;
        int8_t tex_filename[ 128 ];
        int8_t alpha_filename[ 128 ];
    };
#pragma pack()
    
#pragma pack( 1 )
    struct ms3d_model_material_m
    {
        ms3d_model_material f;
        dpid id;
        std::string cmt;
    };
#pragma pack()
    
}

#endif