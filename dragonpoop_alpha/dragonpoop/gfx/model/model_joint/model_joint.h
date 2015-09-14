
#ifndef dragonpoop_model_joint_h
#define dragonpoop_model_joint_h

#include "../model_component/model_components.h"
#include "../../dpvertex/dpxyz_f.h"

namespace dragonpoop
{
    
#pragma pack( 0 )
    struct model_joint_header_hdr
    {
        uint8_t version;
        uint8_t size;
    };
#pragma pack()
    
#pragma pack( 0 )
    struct model_joint_header_v1
    {
        model_joint_header_hdr h;
        dpid parent_id;
        dpxyz_f pos, rot;
    };
#pragma pack()

    class model_joint : public model_component
    {
        
    private:
        
        dpxyz_f pos, rot;
        dpid parent_id;
        
    protected:
        
        //write data to disk/memory
        virtual bool writeData( dpbuffer *b );

    public:
        
        //ctor
        model_joint( dpid id );
        //dtor
        virtual ~model_joint( void );
        //set position
        void setPosition( dpxyz_f *x );
        //get position
        void getPosition( dpxyz_f *x );
        //set rotation
        void setRotation( dpxyz_f *x );
        //get rotation
        void getRotation( dpxyz_f *x );
        //set parent
        void setParent( dpid id );
        //get parent
        dpid getParent( void );
        
    };
    
};

#endif