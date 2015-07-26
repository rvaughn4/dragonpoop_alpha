
#ifndef dragonpoop_model_vertex_h
#define dragonpoop_model_vertex_h

#include "../model_component/model_components.h"
#include "../../dpvertex/dpxyzw.h"

namespace dragonpoop
{

    class model_vertex : public model_component
    {

    private:

        dpxyzw x;

    protected:

    public:

        //ctor
        model_vertex( dpid id );
        //dtor
        virtual ~model_vertex( void );
        //set position
        void setPosition( dpxyzw *x );
        //get position
        void getPosition( dpxyzw *x );
    };
    
};

#endif