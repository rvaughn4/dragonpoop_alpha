
#ifndef dragonpoop_model_instance_vertex_h
#define dragonpoop_model_instance_vertex_h

#include "../../model_component/model_components.h"
#include "../../../dpvertex/dpxyzw.h"
#include "../../model_vertex/model_vertex.h"

namespace dragonpoop
{
    
    class model_instance_vertex : public model_component
    {
        
    private:
        
        dpxyzw pos;
        
        struct
        {
            dpxyzw pos;
            uint64_t t;
        } start, end;
        
    protected:
        
    public:
        
        //ctor
        model_instance_vertex( model_vertex *v );
        //dtor
        virtual ~model_instance_vertex( void );
        //set position
        void setPosition( dpxyzw *x );
        //get position
        void getPosition( dpxyzw *x );
    };
    
};

#endif