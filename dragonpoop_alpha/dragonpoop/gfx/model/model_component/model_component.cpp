
#include "model_component.h"

namespace dragonpoop
{

    //ctor
    model_component::model_component( dpid id, uint16_t mtype )
    {
        this->id = id;
        this->mtype = mtype;
    }

    //dtor
    model_component::~model_component( void )
    {

    }

    //set name
    void model_component::setName( std::string *sname )
    {
        this->sname.assign( *sname );
    }

    //get name
    void model_component::getName( std::string *sname )
    {
        sname->assign( this->sname );
    }

    //set comment
    void model_component::setComment( std::string *s )
    {
        this->scmmt.assign( *s );
    }

    //get comment
    void model_component::getComment( std::string *s )
    {
        s->assign( this->scmmt );
    }

    //append to comment
    void model_component::appendComment( std::string *s )
    {
        this->scmmt.append( *s );
    }

    //compare name
    bool model_component::compareName( std::string *sname )
    {
        return this->sname.compare( *sname ) == 0;
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

};
