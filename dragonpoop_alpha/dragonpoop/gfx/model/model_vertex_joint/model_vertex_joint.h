
#ifndef dragonpoop_model_vertex_joint_h
#define dragonpoop_model_vertex_joint_h

#include "../model_component/model_components.h"

namespace dragonpoop
{
    
#pragma pack( 0 )
    struct model_vertex_joint_header_hdr
    {
        uint8_t version;
        uint8_t size;
    };
#pragma pack()
    
#pragma pack( 0 )
    struct model_vertex_joint_header_v1
    {
        model_vertex_joint_header_hdr h;
        dpid vertex_id, joint_id;
        float w;
    };
#pragma pack()

    class model_vertex_joint : public model_component
    {
        
    private:
        
        dpid vertex_id, joint_id;
        float w;
        
    protected:
        
        //write data to disk/memory
        virtual bool writeData( dpbuffer *b );
        //read data from disk/memory
        virtual bool readData( dpbuffer *b );

    public:
        
        //ctor
        model_vertex_joint( dpid id, dpid vertex_id, dpid joint_id );
        //dtor
        virtual ~model_vertex_joint( void );
        //get vertex id
        dpid getVertexId( void );
        //get joint id
        dpid getJointId( void );
        //get weight
        float getWeight( void );
        //set weight
        void setWeight( float w );
        
    };
    
};

#endif