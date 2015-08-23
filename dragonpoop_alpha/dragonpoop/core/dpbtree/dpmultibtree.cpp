
#include "dpmultibtree.h"
#include <stdlib.h>
#include <string.h>

namespace dragonpoop
{
    
    //ctor
    dpmultibtree::dpmultibtree( void )
    {
        this->leftbranch = this->rightbranch = 0;
        this->key.size = 0;
        this->key.buffer = 0;
        this->hasKey = 0;
    }
    
    //dtor
    dpmultibtree::~dpmultibtree( void )
    {
        this->clear();
    }
    
    //find leaf
    void *dpmultibtree::findLeaf( char *key, unsigned int key_size )
    {
        int r;
        
        if( !key || !key_size )
            return 0;
        
        r = this->compareKey( key, key_size );
        switch( r )
        {
            case dpmultibtree_left:
            {
                if( this->leftbranch )
                    return this->leftbranch->findLeaf( key, key_size );
            }
            case dpmultibtree_right:
            {
                if( this->rightbranch )
                    return this->rightbranch->findLeaf( key, key_size );
            }
            case dpmultibtree_leaf:
            {
                if( this->leaves.empty() )
                    return 0;
                return this->leaves.front();
            }
        }
        
        return 0;
    }
    
    //add leaf
    void dpmultibtree::addLeaf( char *key, unsigned int key_size, void *o )
    {
        int r;
        
        if( !key || !key_size )
            return;
        if( !this->hasKey )
        {
            this->setKey( key, key_size );
            this->hasKey = 1;
        }
        
        r = this->compareKey( key, key_size );
        switch( r )
        {
            case dpmultibtree_left:
            {
                if( !this->leftbranch )
                    this->leftbranch = (dpmultibtree *)this->genBranch();
                if( this->leftbranch )
                    return this->leftbranch->addLeaf( key, key_size, o );
            }
            case dpmultibtree_right:
            {
                if( !this->rightbranch )
                    this->rightbranch = (dpmultibtree *)this->genBranch();
                if( this->rightbranch )
                    return this->rightbranch->addLeaf( key, key_size, o );
            }
            case dpmultibtree_leaf:
            {
                if( o )
                    this->leaves.push_back( o );
            }
        }
    }
    
    //clear
    void dpmultibtree::clear( void )
    {
        this->dptree::clear();
        if( this->key.buffer )
            free( this->key.buffer );
        this->key.buffer = 0;
        this->key.size = 0;
    }
    
    //remove leaf
    void dpmultibtree::removeLeaf( void *o )
    {
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

        if( this->leftbranch )
            this->leftbranch->removeLeaf( o );
        if( this->rightbranch )
            this->rightbranch->removeLeaf( o );
    }
    
    //clear leaves
    void dpmultibtree::clearLeaves( void )
    {
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
        
        if( this->leftbranch )
            this->leftbranch->clearLeaves();
        if( this->rightbranch )
            this->rightbranch->clearLeaves();

    }
    
    //clear branches
    void dpmultibtree::clearBranches( void )
    {
        if( this->leftbranch )
            delete this->leftbranch;
        if( this->rightbranch )
            delete this->rightbranch;
        this->leftbranch = this->rightbranch = 0;
    }
    
    //ovverride to handle deleteion of leaf
    void dpmultibtree::onRemoveLeaf( void *o )
    {
        
    }
    
    //ovverride to generate branches
    dptree *dpmultibtree::genBranch( void )
    {
        return new dpmultibtree();
    }
    
    //set key
    void dpmultibtree::setKey( char *k, unsigned int sz )
    {
        if( this->key.buffer )
            free( this->key.buffer );
        this->key.buffer = (char *)malloc( sz );
        this->key.size = sz;
        if( this->key.buffer )
            memcpy( this->key.buffer, k, sz );
    }
    
    //compare key
    int dpmultibtree::compareKey( char *k, unsigned int sz )
    {
        unsigned int i, l, le;
        union
        {
            uint8_t *p1;
            uint16_t *p2;
            uint32_t *p4;
            uint64_t *p8;
        } a, b;
        
        if( !this->key.buffer || !this->key.size )
            return dpmultibtree_left;
        
        le = sz;
        if( this->key.size < le )
            le = this->key.size;
        
        i = 0;
        while( i < le )
        {
            l = le - i;
            a.p1 = (uint8_t *)&( k[ i ] );
            b.p1 = (uint8_t *)&( this->key.buffer[ i ] );
            
            if( l >= 8 )
            {
                if( *a.p8 > *b.p8 )
                    return dpmultibtree_right;
                if( *a.p8 < *b.p8 )
                    return dpmultibtree_left;
                i += 8;
            }
            else
            {
                if( l >= 4 )
                {
                    if( *a.p4 > *b.p4 )
                        return dpmultibtree_right;
                    if( *a.p4 < *b.p4 )
                        return dpmultibtree_left;
                    i += 4;
                }
                else
                {
                    if( l >= 2 )
                    {
                        if( *a.p2 > *b.p2 )
                            return dpmultibtree_right;
                        if( *a.p2 < *b.p2 )
                            return dpmultibtree_left;
                        i += 2;
                    }
                    else
                    {
                        if( *a.p1 > *b.p1 )
                            return dpmultibtree_right;
                        if( *a.p1 < *b.p1 )
                            return dpmultibtree_left;
                        i += 1;
                    }
                }
            }
        }
        if( i > sz )
            return dpmultibtree_left;
        if( i > this->key.size )
            return dpmultibtree_right;
        
        return dpmultibtree_leaf;
    }
    
    //get leaves
    void dpmultibtree::getLeaves( std::list< void *> *ll )
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
    
    //find leaves
    void dpmultibtree::findLeaves( char *key, unsigned int key_size, std::list<void *> *l )
    {
        int r;
        
        if( !key || !key_size )
            return;
        
        r = this->compareKey( key, key_size );
        switch( r )
        {
            case dpmultibtree_left:
            {
                if( this->leftbranch )
                    return this->leftbranch->findLeaves( key, key_size, l );
            }
            case dpmultibtree_right:
            {
                if( this->rightbranch )
                    return this->rightbranch->findLeaves( key, key_size, l );
            }
            case dpmultibtree_leaf:
            {
                return this->getLeaves( l );
            }
        }
        
    }
    
};
