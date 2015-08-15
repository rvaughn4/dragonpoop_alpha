#ifndef dragonpoop_ms3d_model_comment_h
#define dragonpoop_ms3d_model_comment_h

#include <stdint.h>

namespace dragonpoop
{
    
#pragma pack( 1 )
    struct ms3d_model_extras_section
    {
        int32_t version;
    };
#pragma pack()
    
#pragma pack( 1 )
    struct ms3d_model_comment_section
    {
        int32_t cnt;
    };
#pragma pack()
    
#pragma pack( 1 )
    struct ms3d_model_comment_prefix
    {
        int32_t size;
    };
#pragma pack()
    
    
#pragma pack( 1 )
    struct ms3d_model_comment
    {
        ms3d_model_comment_prefix h;
        char data;
    };
#pragma pack()
    
}

#endif