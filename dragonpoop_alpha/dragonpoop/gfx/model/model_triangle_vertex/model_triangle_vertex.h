
#ifndef dragonpoop_model_triangle_vertex_h
#define dragonpoop_model_triangle_vertex_h

#include "../model_component/model_components.h"
#include "../../dpvertex/dpxyzw.h"
#include "../../dpvertex/dpst.h"

namespace dragonpoop
{

    class model_triangle_vertex : public model_component
    {

    private:

        dpid v_id, t_id;
        dpxyzw norm;
        dpst texcoords0, texcoords1;

    protected:

    public:

        //ctor
        model_triangle_vertex( dpid id );
        //dtor
        virtual ~model_triangle_vertex( void );
        //set triangle id
        void setTriangleId( dpid id );
        //set vertex id
        void setVertexId( dpid id );
        //get triangle id
        dpid getTriangleId( void );
        //get vertex id
        dpid getVertexId( void );
        //set normals
        void setNormal( dpxyzw *x );
        //get normals
        void getNormal( dpxyzw *x );
        //set texcoord0
        void setTexCoord0( dpst *x );
        //get texcoord0
        void getTexCoord0( dpst *x );
        //set texcoord1
        void setTexCoord1( dpst *x );
        //get texcoord1
        void getTexCoord1( dpst *x );

    };
    
};

#endif