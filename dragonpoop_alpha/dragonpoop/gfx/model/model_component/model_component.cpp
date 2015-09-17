
#include "model_component.h"
#include "../../../core/dpbuffer/dpbuffer.h"
#include "../../../core/dpbuffer/dpbuffer_dynamic.h"

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
    
    //write data to disk/memory
    bool model_component::writeData( dpbuffer *b )
    {
        return 1;
    }
    
    //read data from disk/memory
    bool model_component::readData( dpbuffer *b )
    {
        return 1;
    }
    
    //write component to file/memory
    bool model_component::write( dpbuffer *b )
    {
        model_component_header_v1 h;
        dpbuffer_dynamic d;
        std::string sname, scmt;
        unsigned int wc;
        
        if( !this->writeData( &d ) )
            return 0;
        
        this->getName( &sname );
        this->getComment( &scmt );
        
        wc = b->getWriteCursor();
        h.h.version = 1;
        h.h.hdr_size = sizeof( h );
        h.id = this->id;
        h.ctype = this->mtype;
        h.name_size = (unsigned int)sname.size();
        h.cmt_size = (unsigned int)scmt.size();
        h.h.total_size = h.h.hdr_size + h.name_size + h.cmt_size + d.getSize();
        
        if( !b->writeBytes( (uint8_t *)&h, sizeof( h ) ) )
            return 0;
        if( h.name_size && !b->writeBytes( (uint8_t *)sname.c_str(), h.name_size ) )
            return 0;
        if( h.cmt_size && !b->writeBytes( (uint8_t *)scmt.c_str(), h.cmt_size ) )
            return 0;
        if( d.getSize() && !b->writeBytes( (uint8_t *)d.getBuffer(), d.getSize() ) )
            return 0;
        
        b->setWriteCursor( wc + h.h.total_size );
        return 1;
    }

    //read component type from file/memory
    bool model_component::readType( dpbuffer *b, uint16_t *mtype )
    {
        unsigned int rc;
        model_component_header_v1 h;

        rc = b->getReadCursor();
        
        if( !b->readBytes( (uint8_t *)&h, sizeof( h ) ) )
            return 0;
        if( h.h.hdr_size < sizeof( h ) || h.h.version < 1 )
            return 0;
        
        b->setReadCursor( rc );
        *mtype = h.ctype;
        return 1;
    }
    
    //read component from file/memory
    bool model_component::read( dpbuffer *b )
    {
        model_component_header_v1 h;
        unsigned int i, rc;
        dpbuffer_dynamic nb;
        uint8_t v;
        std::string s;
        bool r;
        
        rc = i = b->getReadCursor();
        if( !b->readBytes( (uint8_t *)&h, sizeof( h ) ) )
            return 0;
        i += h.h.hdr_size;
        b->setReadCursor( i );
        this->id = h.id;
        
        nb.clear();
        for( i = 0; i < h.name_size; i++ )
        {
            if( !b->readByte( &v ) )
                return 0;
            nb.writeByte( &v );
        }
        s.assign( nb.getBuffer(), nb.getWriteCursor() );
        this->setName( &s );
        
        nb.clear();
        for( i = 0; i < h.cmt_size; i++ )
        {
            if( !b->readByte( &v ) )
                return 0;
            nb.writeByte( &v );
        }
        s.assign( nb.getBuffer(), nb.getWriteCursor() );
        this->setComment( &s );
        
        r = this->readData( b );
        
        b->setReadCursor( rc + h.h.total_size );
        return r;
    }
    
};
