
#ifndef dragonpoop_model_component_h
#define dragonpoop_model_component_h

#include "../../../core/dpid/dpid.h"
#include <string>

namespace dragonpoop
{

    class model_component
    {

    private:

        uint16_t mtype;
        dpid id;
        std::string sname, scmmt;

    protected:

        //ctor
        model_component( dpid id, uint16_t mtype );

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

    };
    
};

#endif