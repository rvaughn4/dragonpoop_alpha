#ifndef dragonpoop_ms3d_model_joint_h
#define dragonpoop_ms3d_model_joint_h

#include <stdint.h>
#include "../../../../core/dpid/dpid.h"
#include <vector>
#include <string>

namespace dragonpoop
{
    
#pragma pack( 1 )
    struct ms3d_model_joint_section
    {
        uint16_t cnt;
    };
#pragma pack()
    
#pragma pack( 1 )
    struct ms3d_model_joint
    {
        uint8_t flags;
        int8_t name[ 32 ];
        int8_t parent_name[ 32 ];
        struct
        {
            float x, y, z;
        } rot, pos;
        uint16_t cnt_rot_frames;
        uint16_t cnt_pos_frames;
    };
#pragma pack()
    
#pragma pack( 1 )
    struct ms3d_model_joint_keyframe
    {
        float time;
        float x, y, z;
    };
#pragma pack()
    
#pragma pack( 1 )
    struct ms3d_model_joint_extra_section
    {
        int32_t version;
    };
#pragma pack()
    
#pragma pack( 1 )
    struct ms3d_model_joint_extra
    {
        struct
        {
            float r, g, b;
        } color;
    };
#pragma pack()
    
#pragma pack( 1 )
    struct ms3d_model_joint_m
    {
        ms3d_model_joint f;
        dpid id, pid;
        std::vector<ms3d_model_joint_keyframe> rotate_frames, translate_frames;
        ms3d_model_joint_extra e;
        std::string cmt;
    };
#pragma pack()
    
}

#endif
