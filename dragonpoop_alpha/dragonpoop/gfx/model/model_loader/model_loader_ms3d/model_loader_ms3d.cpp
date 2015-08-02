
#include "model_loader_ms3d.h"
#include "model_loader_ms3d_state_parse_header.h"

namespace dragonpoop
{
    
    //ctor
    model_loader_ms3d::model_loader_ms3d( core *c, dptaskpool_writelock *tp, model_ref *m, std::string *fname ) : model_loader( c, tp, m, fname )
    {
        this->verts = 0;
        this->tris = 0;
        this->groups = 0;
    }
    
    //dtor
    model_loader_ms3d::~model_loader_ms3d( void )
    {
        std::vector<ms3d_model_vertex_m> *v;
        std::vector<ms3d_model_triangle_m> *t;
        std::vector<ms3d_model_group_m> *g;
        
        v = this->verts;
        delete v;
        
        t = this->tris;
        delete t;

        g = this->groups;
        delete g;
    }
    
    //generate second state
    model_loader_state *model_loader_ms3d::genState( dpbuffer *b )
    {
        return new model_loader_ms3d_state_parse_header( b );
    }
    
};
