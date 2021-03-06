
#include "model_loader_ms3d.h"
#include "model_loader_ms3d_state_parse_header.h"

namespace dragonpoop
{
    
    //ctor
    model_loader_ms3d::model_loader_ms3d( core *c, model_ref *m, std::string *path_name, std::string *fname ) : model_loader( c, m, path_name, fname )
    {
        this->verts = 0;
        this->tris = 0;
        this->groups = 0;
        this->materials = 0;
        this->joints = 0;
        this->frames = 0;
        dpid_zero( &this->anim_id );
        
        this->fname.assign( *fname );
        this->pname.assign( *path_name );
    }
    
    //dtor
    model_loader_ms3d::~model_loader_ms3d( void )
    {
    }
    
    //generate second state
    model_loader_state *model_loader_ms3d::genState( dpbuffer *b )
    {
        return new model_loader_ms3d_state_parse_header( b );
    }
    
    std::string trim_right_copy(
                                       const std::string& s,
                                       const std::string& delimiters )
    {
        return s.substr( 0, s.find_last_not_of( delimiters ) + 1 );
    }
    
    std::string trim_left_copy(
                                      const std::string& s,
                                      const std::string& delimiters )
    {
        return s.substr( s.find_first_not_of( delimiters ) );
    }
    
    std::string trim_copy(
                                 const std::string& s,
                                 const std::string& delimiters )
    {
        return trim_left_copy( trim_right_copy( s, delimiters ), delimiters );
    }
    
    
    
};
