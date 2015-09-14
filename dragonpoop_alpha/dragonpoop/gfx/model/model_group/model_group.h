
#ifndef dragonpoop_model_group_h
#define dragonpoop_model_group_h

#include "../model_component/model_components.h"

namespace dragonpoop
{
    
#pragma pack( 0 )
    struct model_group_header_hdr
    {
        uint8_t version;
        uint8_t size;
    };
#pragma pack()
    
#pragma pack( 0 )
    struct model_group_header_v1
    {
        model_group_header_hdr h;
        dpid m_id;
    };
#pragma pack()

    class model_group : public model_component
    {

    private:

        dpid m_id;

    protected:
        
        //write data to disk/memory
        virtual bool writeData( dpbuffer *b );

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