
#ifndef dragonpoop_model_loader_ms3d_h
#define dragonpoop_model_loader_ms3d_h

#include "../model_loader.h"
#include "model_loader_ms3d_vertex.h"
#include "model_loader_ms3d_triangle.h"
#include "model_loader_ms3d_group.h"
#include "model_loader_ms3d_material.h"
#include "model_loader_ms3d_animation.h"
#include "model_loader_ms3d_joint.h"
#include <vector>
#include <string>

namespace dragonpoop
{
    
    class model_loader_ms3d : public model_loader
    {
        
    private:
        
        std::atomic<std::vector<ms3d_model_vertex_m> *> verts;
        std::atomic<std::vector<ms3d_model_triangle_m> *> tris;
        std::atomic<std::vector<ms3d_model_group_m> *> groups;
        std::atomic<std::vector<ms3d_model_material_m> *> materials;
        std::atomic<std::vector<ms3d_model_joint_m> *> joints;
        std::atomic<std::vector<ms3d_model_frame> *> frames;
        ms3d_model_animation_section anim;
        dpid anim_id;
        std::string cmt, fname, pname;
        
    protected:
        
        //generate second state
        virtual model_loader_state *genState( dpbuffer *b );
        
    public:
        
        //ctor
        model_loader_ms3d( core *c, model_ref *m, std::string *path_name, std::string *fname );
        //dtor
        virtual ~model_loader_ms3d( void );
        
        
        friend class model_loader_ms3d_state_parse_verts;
        friend class model_loader_ms3d_state_parse_vertex_extras;
        friend class model_loader_ms3d_state_make_verts;
        friend class model_loader_ms3d_state_make_vertex_joints;
        friend class model_loader_ms3d_state_parse_triangles;
        friend class model_loader_ms3d_state_make_triangles;
        friend class model_loader_ms3d_state_parse_groups;
        friend class model_loader_ms3d_state_make_groups;
        friend class model_loader_ms3d_state_parse_materials;
        friend class model_loader_ms3d_state_make_materials;
        friend class model_loader_ms3d_state_parse_animation;
        friend class model_loader_ms3d_state_make_animation;
        friend class model_loader_ms3d_state_parse_joints;
        friend class model_loader_ms3d_state_make_joints;
        friend class model_loader_ms3d_state_parse_comments;
        friend class model_loader_ms3d_state_make_frames;
        
        friend class model_loader_ms3d_state_cleanup;
    };
    
    std::string trim_right_copy(
                                       const std::string& s,
                                       const std::string& delimiters = " \f\n\r\t\v" );
    std::string trim_left_copy(
                                      const std::string& s,
                                      const std::string& delimiters = " \f\n\r\t\v" );
    
    std::string trim_copy(
                                 const std::string& s,
                                 const std::string& delimiters = " \f\n\r\t\v" );
    
};

#endif