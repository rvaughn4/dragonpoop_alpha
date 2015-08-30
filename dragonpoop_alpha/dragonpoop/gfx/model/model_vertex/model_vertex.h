
#ifndef dragonpoop_model_vertex_h
#define dragonpoop_model_vertex_h

#include "../model_component/model_components.h"
#include "../../dpvertex/dpxyz_f.h"
#include "../model_matrix/model_vector.h"

namespace dragonpoop
{

    class model_vertex : public model_component
    {

    private:

        dpxyz_f x;

    protected:

    public:

        //ctor
        model_vertex( dpid id );
        //dtor
        virtual ~model_vertex( void );
        //set position
        void setPosition( dpxyz_f *x );
        //get position
        void getPosition( dpxyz_f *x );
        //set position
        void setPosition( model_vector *v );
        //get position
        void getPosition( model_vector *v );
        
    };
    
};

#endif