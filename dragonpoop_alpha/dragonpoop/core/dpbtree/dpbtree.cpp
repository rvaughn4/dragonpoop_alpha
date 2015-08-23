
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
        this->hasKey = 0;
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
        dpbtree *b;
        
        if( !key || !key_size )
            return 0;
        
        r = this->compareKey( key, key_size );
        switch( r )
        {
            case dpbtree_left:
            {
                b = this->leftbranch;
                if( b )
                    return b->findLeaf( key, key_size );
            }
            case dpbtree_right:
            {
                b = this->rightbranch;
                if( b )
                    return b->findLeaf( key, key_size );
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
        dpbtree *b;
        
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
            case dpbtree_left:
            {
                b = this->leftbranch;
                if( !b )
                    this->leftbranch = b = (dpbtree *)this->genBranch();
                if( b )
                    return b->addLeaf( key, key_size, o );
            }
            case dpbtree_right:
            {
                b = this->rightbranch;
                if( !b )
                    this->rightbranch = b = (dpbtree *)this->genBranch();
                if( b )
                    return b->addLeaf( key, key_size, o );
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
        this->hasKey = 0;
        if( this->key.buffer )
            free( this->key.buffer );
        this->key.buffer = 0;
        this->key.size = 0;
    }
    
    //remove leaf
    void dpbtree::removeLeaf( void *o )
    {
        dpbtree *b;
        
        if( this->o == o )
        {
            this->onRemoveLeaf( o );
            this->o = 0;
        }

        b = this->leftbranch;
        if( b )
            b->removeLeaf( o );
        b = this->rightbranch;
        if( b )
            b->removeLeaf( o );
    }
    
    //clear leaves
    void dpbtree::clearLeaves( void )
    {
        dpbtree *b;
        
        b = this->leftbranch;
        if( b )
            b->clearLeaves();
        b = this->rightbranch;
        if( b )
            b->clearLeaves();

        if( this->o )
            this->onRemoveLeaf( this->o );
        this->o = 0;
    }
    
    //clear branches
    void dpbtree::clearBranches( void )
    {
        dpbtree *b;
        
        b = this->leftbranch;
        if( b )
            delete b;
        b = this->rightbranch;
        if( b )
            delete b;
        this->leftbranch = this->rightbranch = 0;
    }
    
    //ovverride to handle deleteion of leaf
    void dpbtree::onRemoveLeaf( void *o )
    {
        
    }
    
    //ovverride to generate branches
    dptree *dpbtree::genBranch( void )
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
    
    //get leaves
    void dpbtree::getLeaves( std::list< void *> *l )
    {
        if( this->o )
            l->push_back( this->o );
    }
    
    //find leaves
    void dpbtree::findLeaves( char *key, unsigned int key_size, std::list<void *> *l )
    {
        int r;
        dpbtree *b;
        
        if( !key || !key_size )
            return;
        
        r = this->compareKey( key, key_size );
        switch( r )
        {
            case dpbtree_left:
            {
                b = this->leftbranch;
                if( b )
                    return b->findLeaves( key, key_size, l );
            }
            case dpbtree_right:
            {
                b = this->rightbranch;
                if( b )
                    return b->findLeaves( key, key_size, l );
            }
            case dpbtree_leaf:
            {
                return this->getLeaves( l );
            }
        }
        
    }
    
};
