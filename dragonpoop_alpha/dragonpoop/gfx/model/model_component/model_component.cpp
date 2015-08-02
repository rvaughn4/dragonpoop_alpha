
#include "model_component.h"

namespace dragonpoop
{

    //ctor
    model_component::model_component( dpid id, uint16_t mtype )
    {
        this->id = id;
        this->mtype = mtype;
        this->sname = 0;
        this->scmmt = 0;
    }

    //dtor
    model_component::~model_component( void )
    {
        std::string *s;
        s = this->sname;
        delete s;
        s = this->scmmt;
        delete s;
    }

    //set name
    void model_component::setName( std::string *sname )
    {
        std::string *s;
        s = this->sname;
        if( !s )
            this->sname = s = new std::string();
        s->assign( *sname );
    }

    //get name
    void model_component::getName( std::string *sname )
    {
        std::string *s;
        s = this->sname;
        if( s )
            sname->assign( *s );
    }

    //set comment
    void model_component::setComment( std::string *ss )
    {
        std::string *s;
        s = this->scmmt;
        if( !s )
            this->scmmt = s = new std::string();
        s->assign( *ss );
    }

    //get comment
    void model_component::getComment( std::string *ss )
    {
        std::string *s;
        s = this->scmmt;
        if( s )
            ss->assign( *s );
    }

    //append to comment
    void model_component::appendComment( std::string *ss )
    {
        std::string *s;
        s = this->scmmt;
        if( !s )
            this->scmmt = s = new std::string();
        s->append( *ss );
    }

    //compare name
    bool model_component::compareName( std::string *sname )
    {
        std::string *s;
        s = this->sname;
        if( s )
            return s->compare( *sname ) == 0;
        return 0;
    }

    //set id
    void model_component::setId( dpid id )
    {
        this->id = id;
    }

    //get id
    dpid model_component::getId( void )
    {
        return this->id;
    }

    //compare id
    bool model_component::compareId( dpid id )
    {
        return dpid_compare( &this->id, &id );
    }

    //get type
    uint16_t model_component::getType( void )
    {
        return this->mtype;
    }

    //returns true if has parent
    bool model_component::hasParent( dpid id )
    {
        return 0;
    }
    
};
