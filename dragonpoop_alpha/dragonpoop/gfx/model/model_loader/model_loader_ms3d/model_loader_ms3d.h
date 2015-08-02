
#ifndef dragonpoop_model_loader_ms3d_h
#define dragonpoop_model_loader_ms3d_h

#include "../model_loader.h"
#include "model_loader_ms3d_vertex.h"
#include "model_loader_ms3d_triangle.h"
#include "model_loader_ms3d_group.h"
#include <vector>

namespace dragonpoop
{
    
    class model_loader_ms3d : public model_loader
    {
        
    private:
        
        std::atomic<std::vector<ms3d_model_vertex_m> *> verts;
        std::atomic<std::vector<ms3d_model_triangle_m> *> tris;
        std::atomic<std::vector<ms3d_model_group_m> *> groups;
        
    protected:
        
        //generate second state
        virtual model_loader_state *genState( dpbuffer *b );
        
    public:
        
        //ctor
        model_loader_ms3d( core *c, dptaskpool_writelock *tp, model_ref *m, std::string *fname );
        //dtor
        virtual ~model_loader_ms3d( void );
        
        
        friend class model_loader_ms3d_state_parse_verts;
        friend class model_loader_ms3d_state_make_verts;
        friend class model_loader_ms3d_state_parse_triangles;
        friend class model_loader_ms3d_state_make_triangles;
        friend class model_loader_ms3d_state_parse_groups;
        
        friend class model_loader_ms3d_state_cleanup;
    };
    
};

#endif