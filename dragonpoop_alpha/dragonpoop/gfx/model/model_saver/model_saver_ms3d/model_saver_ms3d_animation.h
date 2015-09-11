#ifndef dragonpoop_ms3d_model_animation_h
#define dragonpoop_ms3d_model_animation_h

#include <stdint.h>
#include "../../../../core/dpid/dpid.h"

namespace dragonpoop
{
    
#pragma pack( 1 )
    struct ms3d_model_animation_section
    {
        float fps;
        float current_time;
        int32_t cnt_frames;
    };
#pragma pack()
    
    struct ms3d_model_frame
    {
        float t;
        float ot;
        dpid id, afid;
    };

    struct ms3d_model_animation_m
    {
        int32_t cnt_frames;
        unsigned int start, length, skip;
        float highest_time;
        dpid id;
    };
    
}

#endif
