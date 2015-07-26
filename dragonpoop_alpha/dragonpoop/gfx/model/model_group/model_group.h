
#ifndef dragonpoop_model_group_h
#define dragonpoop_model_group_h

#include "../model_component/model_components.h"

namespace dragonpoop
{

    class model_group : public model_component
    {

    private:

        dpid m_id;

    protected:

    public:

        //ctor
        model_group( dpid id );
        //dtor
        virtual ~model_group( void );
        //set material id
        void setMaterialId( dpid id );
        //get material id
        dpid getMaterialId( void );

    };
    
};

#endif