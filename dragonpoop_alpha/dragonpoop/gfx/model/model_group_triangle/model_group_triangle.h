
#ifndef dragonpoop_model_group_triangle_h
#define dragonpoop_model_group_triangle_h

#include "../model_component/model_components.h"

namespace dragonpoop
{

#pragma pack( 0 )
    struct model_group_triangle_header_hdr
    {
        uint8_t version;
        uint8_t size;
    };
#pragma pack()
    
#pragma pack( 0 )
    struct model_group_triangle_header_v1
    {
        model_group_triangle_header_hdr h;
        dpid g_id, t_id;
    };
#pragma pack()
    
    class model_group_triangle : public model_component
    {

    private:

        dpid g_id, t_id;

    protected:
        
        //write data to disk/memory
        virtual bool writeData( dpbuffer *b );

    public:

        //ctor
        model_group_triangle( dpid id, dpid group_id, dpid triangle_id );
        //dtor
        virtual ~model_group_triangle( void );
        //get triangle id
        dpid getTriangleId( void );
        //get group id
        dpid getGroupId( void );

    };
    
};

#endif