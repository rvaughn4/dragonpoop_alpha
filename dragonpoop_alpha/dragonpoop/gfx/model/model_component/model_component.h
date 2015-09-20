
#ifndef dragonpoop_model_component_h
#define dragonpoop_model_component_h

#include "../../../core/dpid/dpid.h"
#include <string>
#include <atomic>

namespace dragonpoop
{
    
    class dpbuffer;
    
#pragma pack( 0 )
    struct model_component_header_hdr
    {
        uint8_t version;
        uint8_t hdr_size;
        uint32_t total_size;
    };
#pragma pack()

#pragma pack( 0 )
    struct model_component_header_v1
    {
        model_component_header_hdr h;
        uint16_t ctype;
        dpid id;
        uint16_t name_size;
        uint16_t cmt_size;
    };
#pragma pack()
    
    class model_component
    {

    private:

        uint16_t mtype;
        dpid id;
        std::atomic<std::string *> sname, scmmt;
        std::atomic<bool> bIsalive;

    protected:

        //ctor
        model_component( dpid id, uint16_t mtype );
        //write data to disk/memory
        virtual bool writeData( dpbuffer *b );
        //read data from disk/memory
        virtual bool readData( dpbuffer *b );

    public:

        //dtor
        virtual ~model_component( void );
        //set name
        void setName( std::string *sname );
        //get name
        void getName( std::string *sname );
        //set comment
        void setComment( std::string *s );
        //get comment
        void getComment( std::string *s );
        //append to comment
        void appendComment( std::string *s );
        //compare name
        bool compareName( std::string *sname );
        //set id
        void setId( dpid id );
        //get id
        dpid getId( void );
        //compare id
        bool compareId( dpid id );
        //get type
        uint16_t getType( void );
        //returns true if has parent
        virtual bool hasParent( dpid id );
        //write component to file/memory
        virtual bool write( dpbuffer *b );
        //read component type from file/memory
        static bool readType( dpbuffer *b, uint16_t *mtype );
        //read component from file/memory
        virtual bool read( dpbuffer *b );
        //kill component
        void kill( void );
        //returns true if alive
        bool isAlive( void );

    };

};

#endif
