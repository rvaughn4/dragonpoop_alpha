
#include "multibytetree.h"

namespace dragonpoop
{

    //ctor
    multibytetree::multibytetree( void )
    {
        memset( &this->branches, 0, sizeof( this->branches ) );
    }

    //dtor
    multibytetree::~multibytetree( void )
    {
        this->clear();
    }

    //find leaf
    void *multibytetree::findLeaf( char *key, unsigned int key_size )
    {
        std::list<void *> l;
        this->findLeaves( key, key_size, &l );
        return l.front();
    }

    //find leaves
    void multibytetree::findLeaves( char *key, unsigned int key_size, std::list<void *> *l )
    {
        uint8_t k, *b;
        multibytetree *t;

        if( !key || !key_size )
        {
            this->getLeaves( l );
            return;
        }

        b = (uint8_t *)key;
        k = b[ 0 ];
        t = this->branches[ k ];
        if( !t )
            return;

        key_size--;
        if( key_size )
            t->findLeaves( &key[ 1 ], key_size, l );
        else
            t->findLeaves( 0, 0, l );
    }

    //add leaf
    void multibytetree::addLeaf( char *key, unsigned int key_size, void *o )
    {
        uint8_t k, *b;
        multibytetree *t;

        if( !key || !key_size )
        {
            this->leaves.push_back( o );
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
    void multibytetree::clear( void )
    {
        this->clearBranches();
        this->clearLeaves();
    }

    //remove leaf
    void multibytetree::removeLeaf( void *o )
    {
        unsigned int i;
        multibytetree *t;
        std::list< void *> *l;
        std::list< void *>::iterator li;
        void *p;

        l = &this->leaves;
        for( li = l->begin(); li != l->end(); ++li )
        {
            p = *li;
            if( p != o )
                continue;
            this->onRemoveLeaf( p );
            *li = 0;
        }
        l->remove( 0 );

        for( i = 0; i < 256; i++ )
        {
            t = this->branches[ i ];
            if( t )
                t->removeLeaf( o );
        }
    }

    //clear leaves
    void multibytetree::clearLeaves( void )
    {
        unsigned int i;
        multibytetree *t;
        std::list< void *> *l;
        std::list< void *>::iterator li;
        void *p;

        l = &this->leaves;
        for( li = l->begin(); li != l->end(); ++li )
        {
            p = *li;
            this->onRemoveLeaf( p );
        }
        l->clear();

        for( i = 0; i < 256; i++ )
        {
            t = this->branches[ i ];
            if( t )
                t->clearLeaves();
        }
    }

    //clear branches
    void multibytetree::clearBranches( void )
    {
        unsigned int i;
        multibytetree *t;

        for( i = 0; i < 256; i++ )
        {
            t = this->branches[ i ];
            this->branches[ i ] = 0;
            if( t )
                delete t;
        }
    }

    //ovverride to handle deleteion of leaf
    void multibytetree::onRemoveLeaf( void *o )
    {
    }

    //ovverride to generate branches
    multibytetree *multibytetree::genBranch( void )
    {
        return new multibytetree();
    }

    //get leaves
    void multibytetree::getLeaves( std::list< void *> *ll )
    {
        std::list< void *> *l;
        std::list< void *>::iterator li;
        void *p;

        l = &this->leaves;
        for( li = l->begin(); li != l->end(); ++li )
        {
            p = *li;
            ll->push_back( p );
        }
    }

};
