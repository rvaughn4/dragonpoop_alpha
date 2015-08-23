
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
        this->leaves.buffer = 0;
        this->leaves.size = 0;
    }
    
    //dtor
    dpmultibtree::~dpmultibtree( void )
    {
        void **b;
        
        this->clear();
        
        b = this->leaves.buffer;
        if( b )
            delete[] b;
    }
    
    //find leaf
    void *dpmultibtree::findLeaf( char *key, unsigned int key_size )
    {
        int r;
        dpmultibtree *b;
        
        if( !key || !key_size )
            return 0;
        
        r = this->compareKey( key, key_size );
        switch( r )
        {
            case dpmultibtree_left:
            {
                b = this->leftbranch;
                if( b )
                    return b->findLeaf( key, key_size );
            }
            case dpmultibtree_right:
            {
                b = this->rightbranch;
                if( b )
                    return b->findLeaf( key, key_size );
            }
            case dpmultibtree_leaf:
            {
                if( !this->leaves.size || !this->leaves.buffer )
                    return 0;
                return this->leaves.buffer;
            }
        }
        
        return 0;
    }
    
    //add leaf
    void dpmultibtree::addLeaf( char *key, unsigned int key_size, void *o )
    {
        int r;
        dpmultibtree *b;
        
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
                b = this->leftbranch;
                if( !b )
                    this->leftbranch = b = (dpmultibtree *)this->genBranch();
                if( b )
                    return b->addLeaf( key, key_size, o );
            }
            case dpmultibtree_right:
            {
                b = this->rightbranch;
                if( !b )
                    this->rightbranch = b = (dpmultibtree *)this->genBranch();
                if( b )
                    return b->addLeaf( key, key_size, o );
            }
            case dpmultibtree_leaf:
            {
                if( !this->_addUseFreeSpace( o ) )
                    this->_addAllocSpace( o );
            }
        }
    }
    
    //clear
    void dpmultibtree::clear( void )
    {
        char *b;
        
        this->dptree::clear();
        
        b = this->key.buffer;
        if( b )
            delete[] b;
        this->hasKey = 0;
        this->key.buffer = 0;
        this->key.size = 0;
    }
    
    //remove leaf
    void dpmultibtree::removeLeaf( void *o )
    {
        dpmultibtree *b;
        
        if( this->_remove( o ) )
            this->onRemoveLeaf( o );
        
        b = this->leftbranch;
        if( b )
            b->removeLeaf( o );
        b = this->rightbranch;
        if( b )
            b->removeLeaf( o );
    }
    
    //clear leaves
    void dpmultibtree::clearLeaves( void )
    {
        dpmultibtree *b;

        this->_clear();
        
        b = this->leftbranch;
        if( b )
            b->clearLeaves();
        b = this->rightbranch;
        if( b )
            b->clearLeaves();
    }
    
    //clear branches
    void dpmultibtree::clearBranches( void )
    {
        dpmultibtree *b;

        b = this->leftbranch;
        if( b )
            delete b;
        b = this->rightbranch;
        if( b )
            delete b;

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
        this->key.buffer = new char[ sz ];
        this->key.size = sz;
        if( this->key.buffer )
            memcpy( this->key.buffer, k, sz );
    }
    
    //compare key
    int dpmultibtree::compareKey( char *k, unsigned int sz )
    {
        unsigned int i, le;
        struct
        {
            uint8_t *p1;
        } a, b;
        
        if( !this->key.buffer || !this->key.size )
            return dpmultibtree_left;
        
        le = sz;
        if( this->key.size < le )
            le = this->key.size;
        
        i = 0;
        while( i < le )
        {
            a.p1 = (uint8_t *)&( k[ i ] );
            b.p1 = (uint8_t *)&( this->key.buffer[ i ] );
            if( *a.p1 > *b.p1 )
                return dpmultibtree_right;
            if( *a.p1 < *b.p1 )
                return dpmultibtree_left;
            i += 1;
        }
        if( i > sz )
            return dpmultibtree_left;
        if( i > this->key.size )
            return dpmultibtree_right;
        
        return dpmultibtree_leaf;
    }
    
    //get leaves
    void dpmultibtree::getLeaves( std::list< void *> *l )
    {
        this->_copy( l );
    }
    
    //find leaves
    void dpmultibtree::findLeaves( char *key, unsigned int key_size, std::list<void *> *l )
    {
        int r;
        dpmultibtree *b;
        
        if( !key || !key_size )
            return;
        
        r = this->compareKey( key, key_size );
        switch( r )
        {
            case dpmultibtree_left:
            {
                b = this->leftbranch;
                if( b )
                    return b->findLeaves( key, key_size, l );
            }
            case dpmultibtree_right:
            {
                b = this->rightbranch;
                if( b )
                    return b->findLeaves( key, key_size, l );
            }
            case dpmultibtree_leaf:
            {
                return this->getLeaves( l );
            }
        }
        
    }
    
    //add new leaf using free space
    bool dpmultibtree::_addUseFreeSpace( void *v )
    {
        unsigned int i;
        void *p, **l;
        
        if( !this->leaves.size || !this->leaves.buffer )
            return 0;
        
        l = this->leaves.buffer;
        for( i = 0; i < this->leaves.size; i++ )
        {
            p = l[ i ];
            if( p )
                continue;
            l[ i ] = v;
            return 1;
        }
        
        return 0;
    }
    
    //add new leaf by increasing size of buffer
    bool dpmultibtree::_addAllocSpace( void *v )
    {
        void **nb, **ob;
        unsigned int ns, os;
        
        ob = this->leaves.buffer;
        os = this->leaves.size;
        
        ns = os * 2 + 3;
        nb = new void *[ ns ];
        if( !nb )
            return 0;
        memset( nb, 0, ns * sizeof( void *) );
        if( ob )
        {
            memcpy( nb, ob, os );
            delete[] ob;
        }
        this->leaves.buffer = nb;
        this->leaves.size = ns;
        
        return this->_addUseFreeSpace( v );
    }
    
    //remove leaf by freeing slot
    bool dpmultibtree::_remove( void *v )
    {
        unsigned int i;
        void *p, **l;
        
        if( !this->leaves.size || !this->leaves.buffer )
            return 0;
        
        l = this->leaves.buffer;
        for( i = 0; i < this->leaves.size; i++ )
        {
            p = l[ i ];
            if( p == v )
                return 1;
        }
        
        return 0;
    }
    
    //clear leaves buffer
    void dpmultibtree::_clear( void )
    {
        unsigned int i;
        void **l;
        
        if( !this->leaves.size || !this->leaves.buffer )
            return;
        
        l = this->leaves.buffer;
        for( i = 0; i < this->leaves.size; i++ )
        {
            l[ i ] = 0;
        }
    }
    
    //copy leaves
    void dpmultibtree::_copy( std::list< void *> *ll )
    {
        unsigned int i;
        void *p, **l;
        
        if( !this->leaves.size || !this->leaves.buffer )
            return;
        
        l = this->leaves.buffer;
        for( i = 0; i < this->leaves.size; i++ )
        {
            p = l[ i ];
            if( p )
                ll->push_back( p );
        }
    }
    
};
