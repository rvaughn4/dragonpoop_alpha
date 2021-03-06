
#ifndef dragonpoop_model_triangle_vertex_h
#define dragonpoop_model_triangle_vertex_h

#include "../model_component/model_components.h"
#include "../../dpvertex/dpxyz_f.h"
#include "../../dpvertex/dpst_f.h"
#include "../model_matrix/model_vector.h"

namespace dragonpoop
{

#pragma pack( 0 )
    struct model_triangle_vertex_header_hdr
    {
        uint8_t version;
        uint8_t size;
    };
#pragma pack()
    
#pragma pack( 0 )
    struct model_triangle_vertex_header_v1
    {
        model_triangle_vertex_header_hdr h;
        dpid v_id, t_id;
        dpxyz_f norm;
        dpst_f texcoords0, texcoords1;
    };
#pragma pack()
    
    class model_triangle_vertex : public model_component
    {

    private:

        dpid v_id, t_id;
        dpxyz_f norm;
        dpst_f texcoords0, texcoords1;

    protected:

        //write data to disk/memory
        virtual bool writeData( dpbuffer *b );
        //read data from disk/memory
        virtual bool readData( dpbuffer *b );

    public:

        //ctor
        model_triangle_vertex( dpid id, dpid triangle_id, dpid vertex_id );
        //dtor
        virtual ~model_triangle_vertex( void );
        //get triangle id
        dpid getTriangleId( void );
        //get vertex id
        dpid getVertexId( void );
        //set normals
        void setNormal( dpxyz_f *x );
        //get normals
        void getNormal( dpxyz_f *x );
        //set normals
        void setNormal( model_vector *v );
        //get normals
        void getNormal( model_vector *v );
        //set texcoord0
        void setTexCoord0( dpst_f *x );
        //get texcoord0
        void getTexCoord0( dpst_f *x );
        //set texcoord1
        void setTexCoord1( dpst_f *x );
        //get texcoord1
        void getTexCoord1( dpst_f *x );
        //returns true if has parent
        virtual bool hasParent( dpid id );

    };
    
};

#endif