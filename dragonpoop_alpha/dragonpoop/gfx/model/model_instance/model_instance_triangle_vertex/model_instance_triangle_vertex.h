
#ifndef dragonpoop_model_instance_triangle_vertex_h
#define dragonpoop_model_instance_triangle_vertex_h

#include "../../model_component/model_components.h"
#include "../../../dpvertex/dpxyzw.h"
#include "../../../dpvertex/dpst.h"
#include "../../model_triangle_vertex/model_triangle_vertex.h"
#include "../../model_vertex/model_vertex.h"

namespace dragonpoop
{
    
    class model_instance_triangle_vertex : public model_component
    {
        
    private:
        
        dpid v_id, t_id;
        dpxyzw norm, pos;
        dpst texcoords0, texcoords1;
        
        struct
        {
            dpxyzw norm;
            uint64_t t;
        } start, end;
        
    protected:
        
    public:
        
        //ctor
        model_instance_triangle_vertex( model_triangle_vertex *tv );
        //dtor
        virtual ~model_instance_triangle_vertex( void );
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
        //returns true if has parent
        virtual bool hasParent( dpid id );
        
    };
    
};

#endif