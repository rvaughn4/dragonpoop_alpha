
#include "dpbtree.h"
#include <stdlib.h>
#include <string.h>

namespace dragonpoop
{
    
    //ctor
    dpbtree::dpbtree( void )
    {
        this->leftbranch = this->rightbranch = 0;
        this->o = 0;
        this->key.size = 0;
        this->key.buffer = 0;
    }
    
    //dtor
    dpbtree::~dpbtree( void )
    {
        this->clear();
    }
    
    //find leaf
    void *dpbtree::findLeaf( char *key, unsigned int key_size )
    {
        int r;
        
        if( !key || !key_size )
            return 0;
        
        r = this->compareKey( key, key_size );
        switch( r )
        {
            case dpbtree_left:
            {
                if( this->leftbranch )
                    return this->leftbranch->findLeaf( key, key_size );
            }
            case dpbtree_right:
            {
                if( this->rightbranch )
                    return this->rightbranch->findLeaf( key, key_size );
            }
            case dpbtree_leaf:
            {
                return this->o;
            }
        }
     
        return 0;
    }
    
    //add leaf
    void dpbtree::addLeaf( char *key, unsigned int key_size, void *o )
    {
        int r;
        
        if( !key || !key_size )
            return;
        if( !this->key.buffer || !this->key.size )
            this->setKey( key, key_size );
     
        r = this->compareKey( key, key_size );
        switch( r )
        {
            case dpbtree_left:
            {
                if( !this->leftbranch )
                    this->leftbranch = this->genBranch();
                if( this->leftbranch )
                    return this->leftbranch->addLeaf( key, key_size, o );
            }
            case dpbtree_right:
            {
                if( !this->rightbranch )
                    this->rightbranch = this->genBranch();
                if( this->rightbranch )
                    return this->rightbranch->addLeaf( key, key_size, o );
            }
            case dpbtree_leaf:
            {
                if( this->o )
                    this->onRemoveLeaf( this->o );
                this->o = o;
            }
        }
    }
    
    //clear
    void dpbtree::clear( void )
    {
        this->dptree::clear();
        if( this->key.buffer )
            free( this->key.buffer );
        this->key.buffer = 0;
        this->key.size = 0;
    }
    
    //remove leaf
    void dpbtree::removeLeaf( void *o )
    {
        if( this->o == o )
        {
            this->onRemoveLeaf( o );
            this->o = 0;
        }
        if( this->leftbranch )
            this->leftbranch->removeLeaf( o );
        if( this->rightbranch )
            this->rightbranch->removeLeaf( o );
    }
    
    //clear leaves
    void dpbtree::clearLeaves( void )
    {
        if( this->leftbranch )
            this->leftbranch->clearLeaves();
        if( this->rightbranch )
            this->rightbranch->clearLeaves();
        if( this->o )
            this->onRemoveLeaf( this->o );
        this->o = 0;
    }
    
    //clear branches
    void dpbtree::clearBranches( void )
    {
        if( this->leftbranch )
            delete this->leftbranch;
        if( this->rightbranch )
            delete this->rightbranch;
        this->leftbranch = this->rightbranch = 0;
    }
    
    //ovverride to handle deleteion of leaf
    void dpbtree::onRemoveLeaf( void *o )
    {
        
    }
    
    //ovverride to generate branches
    dpbtree *dpbtree::genBranch( void )
    {
        return new dpbtree();
    }

    //set key
    void dpbtree::setKey( char *k, unsigned int sz )
    {
        if( this->key.buffer )
            free( this->key.buffer );
        this->key.buffer = (char *)malloc( sz );
        this->key.size = sz;
        if( this->key.buffer )
            memcpy( this->key.buffer, k, sz );
    }
    
    //compare key
    int dpbtree::compareKey( char *k, unsigned int sz )
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
            return dpbtree_left;
        
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
                    return dpbtree_right;
                if( *a.p8 < *b.p8 )
                    return dpbtree_left;
                i += 8;
            }
            else
            {
                if( l >= 4 )
                {
                    if( *a.p4 > *b.p4 )
                        return dpbtree_right;
                    if( *a.p4 < *b.p4 )
                        return dpbtree_left;
                    i += 4;
                }
                else
                {
                    if( l >= 2 )
                    {
                        if( *a.p2 > *b.p2 )
                            return dpbtree_right;
                        if( *a.p2 < *b.p2 )
                            return dpbtree_left;
                        i += 2;
                    }
                    else
                    {
                        if( *a.p1 > *b.p1 )
                            return dpbtree_right;
                        if( *a.p1 < *b.p1 )
                            return dpbtree_left;
                        i += 1;
                    }
                }
            }
        }
        if( i > sz )
            return dpbtree_left;
        if( i > this->key.size )
            return dpbtree_right;
        
        return dpbtree_leaf;
    }
    
};
