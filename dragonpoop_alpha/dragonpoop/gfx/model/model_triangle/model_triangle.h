
#ifndef dragonpoop_model_triangle_h
#define dragonpoop_model_triangle_h

#include "../model_component/model_components.h"

namespace dragonpoop
{

    class model_triangle : public model_component
    {

    private:

        dpid v0, v1, v2;

    protected:

    public:

        //ctor
        model_triangle( dpid id );
        //dtor
        virtual ~model_triangle( void );
        //set vertex 0 id
        void setVertex0( dpid id );
        //set vertex 1 id
        void setVertex1( dpid id );
        //set vertex 2 id
        void setVertex2( dpid id );
        //get vertex 0 id
        dpid getVertex0( void );
        //get vertex 1 id
        dpid getVertex1( void );
        //get vertex 2 id
        dpid getVertex2( void );

    };
    
};

#endif