#ifndef dragonpoop_ms3d_model_header_h
#define dragonpoop_ms3d_model_header_h

#include <stdint.h>

namespace dragonpoop
{
    
#define ms3d_magic_number "MS3D000000"
    
#pragma pack( 1 )
    struct ms3d_model_header
    {
        char ms3d_magic[ 10 ];
        uint32_t version;
    };
#pragma pack()
    
#pragma pack( 1 )
    struct ms3d_model_extra
    {
        float joint_size;
        int32_t transparency_mode;
        float alpha_ref;
    };
#pragma pack()
    
}

#endif