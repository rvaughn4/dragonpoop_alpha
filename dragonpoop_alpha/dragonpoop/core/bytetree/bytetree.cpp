
#include "bytetree.h"
#include <stdint.h>
#include <string.h>

namespace dragonpoop
{

    //ctor
    bytetree::bytetree( void )
    {
        this->o = 0;
        memset( &this->branches, 0, sizeof( this->branches ) );
    }

    //dtor
    bytetree::~bytetree( void )
    {
        this->clear();
    }

    //find leaf
    void *bytetree::findLeaf( char *key, unsigned int key_size )
    {
        uint8_t k, *b;
        bytetree *t;

        if( !key || !key_size )
            return this->o;

        b = (uint8_t *)key;
        k = b[ 0 ];
        t = this->branches[ k ];
        if( !t )
            return 0;

        key_size--;
        if( key_size )
            return t->findLeaf( &key[ 1 ], key_size );
        return t->findLeaf( 0, 0 );
    }

    //add leaf
    void bytetree::addLeaf( char *key, unsigned int key_size, void *o )
    {
        uint8_t k, *b;
        bytetree *t;

        if( !key || !key_size )
        {
            if( this->o )
                this->onRemoveLeaf( this->o );
            this->o = o;
            return;
        }

        b = (uint8_t *)key;
        k = b[ 0 ];
        t = this->branches[ k ];
        if( !t )
        {
            t = this->genBranch();
            this->branches[ k ] = t;
        }
        if( !t )
            return;

        key_size--;
        if( key_size )
            t->addLeaf( &key[ 1 ], key_size, o );
        else
            t->addLeaf( 0, 0, o );
    }

    //clear
    void bytetree::clear( void )
    {
        this->clearBranches();
        this->clearLeaves();
    }

    //clear leaves
    void bytetree::clearLeaves( void )
    {
        unsigned int i;
        bytetree *t;

        if( this->o )
            this->onRemoveLeaf( this->o );
        this->o = 0;

        for( i = 0; i < 256; i++ )
        {
            t = this->branches[ i ];
            if( t )
                t->clearLeaves();
        }
    }

    //clear branches
    void bytetree::clearBranches( void )
    {
        unsigned int i;
        bytetree *t;

        for( i = 0; i < 256; i++ )
        {
            t = this->branches[ i ];
            this->branches[ i ] = 0;
            if( t )
                delete t;
        }
    }

    //ovverride to handle deleteion of leaf
    void bytetree::onRemoveLeaf( void *o )
    {

    }

    //ovverride to generate branches
    bytetree *bytetree::genBranch( void )
    {
        return new bytetree();
    }

    //remove leaf
    void bytetree::removeLeaf( void *o )
    {
        unsigned int i;
        bytetree *t;

        if( this->o && this->o == o )
        {
            this->onRemoveLeaf( this->o );
            this->o = 0;
        }

        for( i = 0; i < 256; i++ )
        {
            t = this->branches[ i ];
            if( t )
                t->removeLeaf( o );
        }
    }

};
