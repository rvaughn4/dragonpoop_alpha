
#include "dpmultibtree.h"
#include <string.h>

namespace dragonpoop
{

    //ctor
    dpmultibtree::dpmultibtree( void )
    {
    }

    //dtor
    dpmultibtree::~dpmultibtree( void )
    {
        this->clear();
    }

    //find leaf
    void *dpmultibtree::findLeaf( char *key, unsigned int key_size )
    {
        std::multimap<dpmultibtree_key, void *>::iterator i, e;
        dpmultibtree_key k( this );

        if( this->m.empty() )
            return 0;

        k.set( key, key_size );
        e = this->m.end();
        i = this->m.find( k );

        if( i == e )
            return 0;
        return i->second;
    }

    //find leaves
    void dpmultibtree::findLeaves( char *key, unsigned int key_size, std::list<void *> *l )
    {
        std::multimap<dpmultibtree_key, void *>::iterator i, e;
        dpmultibtree_key k( this );

        if( this->m.empty() )
            return;

        k.set( key, key_size );
        std::pair<std::multimap<dpmultibtree_key, void *>::iterator, std::multimap<dpmultibtree_key, void *>::iterator> r;
        r = this->m.equal_range( k );
        e = r.second;
        i = r.first;

        if( i == this->m.end() )
            return;
        for( ; i != e; ++i )
            l->push_back( i->second );
    }

    //add leaf
    void dpmultibtree::addLeaf( char *key, unsigned int key_size, void *o )
    {
        dpmultibtree_key k( this );
        k.set( key, key_size );
        this->m.insert( std::pair<dpmultibtree_key, void *>( k, o ) );
    }

    //remove leaf
    void dpmultibtree::removeLeaf( void *o )
    {
        std::multimap<dpmultibtree_key, void *>::iterator i, e;

        e = this->m.end();
        for( i = this->m.begin(); i != e; ++i )
        {
            if( o != i->second )
                continue;
            this->m.erase( i );
            return this->removeLeaf( o );
        }
    }

    //clear leaves
    void dpmultibtree::clearLeaves( void )
    {
        this->m.clear();
    }

    //clear branches
    void dpmultibtree::clearBranches( void )
    {
    }

    //get leaves
    void dpmultibtree::getLeaves( std::list< void *> *l )
    {
        std::multimap<dpmultibtree_key, void *>::iterator i, e;

        e = this->m.end();
        for( i = this->m.begin(); i != e; ++i )
        {
            l->push_back( i->second );
        }
    }

    //compare key, return >0 if a > b, <0 if a<b or 0 if a==b
    int dpmultibtree::compareKey( char *k_a, char *k_b, unsigned int k_a_size, unsigned int k_b_size )
    {
        unsigned int i, l;

        l = k_a_size;
        if( k_b_size > l )
            l = k_b_size;

        for( i = 0; i < l; i++ )
        {
            if( k_a[ i ] < k_b[ i ] )
                return -1;
            if( k_a[ i ] > k_b[ i ] )
                return 1;
        }
        if( i > k_a_size )
            return 1;
        if( i > k_b_size )
            return -1;

        return 0;
    }

    //ovverride to generate branches
    dptree *dpmultibtree::genBranch( void )
    {
        return new dpmultibtree();
    }

    //ctor
    dpmultibtree_key::dpmultibtree_key( dpmultibtree *t )
    {
        this->t = t;
        this->size = 0;
        this->ptr = 0;
    }

    //ctor
    dpmultibtree_key::dpmultibtree_key( const dpmultibtree_key &c )
    {
        this->t = c.t;
        this->ptr = 0;
        this->set( c.ptr, c.size );
    }

    //dtor
    dpmultibtree_key::~dpmultibtree_key( void )
    {
        if( this->ptr )
            delete[] this->ptr;
    }

    //set value
    void dpmultibtree_key::set( char *k, unsigned int sz )
    {
        if( this->ptr )
            delete[] this->ptr;
        this->ptr = new char[ sz ];
        this->size = sz;
        if( this->ptr )
            memcpy( this->ptr, k, sz );
    }

    //equals
    bool dpmultibtree_key::operator==(const dpmultibtree_key &b ) const
    {
        return this->t->compareKey( this->ptr, b.ptr, this->size, b.size ) == 0;
    }

    //not equals
    bool dpmultibtree_key::operator!=(const dpmultibtree_key &b) const
    {
        return this->t->compareKey( this->ptr, b.ptr, this->size, b.size ) != 0;
    }

    //less than
    bool dpmultibtree_key::operator<(const dpmultibtree_key &b) const
    {
        return this->t->compareKey( this->ptr, b.ptr, this->size, b.size ) < 0;
    }

    //greater than
    bool dpmultibtree_key::operator>(const dpmultibtree_key &b) const
    {
        return this->t->compareKey( this->ptr, b.ptr, this->size, b.size ) > 0;
    }

    //less than or equal
    bool dpmultibtree_key::operator<=(const dpmultibtree_key &b) const
    {
        return this->t->compareKey( this->ptr, b.ptr, this->size, b.size ) <= 0;
    }

    //greater than or equal
    bool dpmultibtree_key::operator>=(const dpmultibtree_key &b) const
    {
        return this->t->compareKey( this->ptr, b.ptr, this->size, b.size ) >= 0;
    }

    //assignment
    dpmultibtree_key& dpmultibtree_key::operator=( const dpmultibtree_key& b )
    {
        this->t = b.t;
        this->ptr = 0;
        this->set( b.ptr, b.size );
        return *this;
    }

};
