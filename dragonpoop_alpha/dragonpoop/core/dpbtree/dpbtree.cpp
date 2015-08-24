
#include "dpbtree.h"

namespace dragonpoop
{
    
    //ctor
    dpbtree::dpbtree( void )
    {
    }
    
    //dtor
    dpbtree::~dpbtree( void )
    {
        this->clear();
    }
    
    //find leaf
    void *dpbtree::findLeaf( char *key, unsigned int key_size )
    {
        std::map<dpbtree_key, void *>::iterator i, e;
        dpbtree_key k( this );
        
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
    void dpbtree::findLeaves( char *key, unsigned int key_size, std::list<void *> *l )
    {
        std::map<dpbtree_key, void *>::iterator i, e;
        dpbtree_key k( this );
        
        if( this->m.empty() )
            return;

        k.set( key, key_size );
        e = this->m.end();
        i = this->m.find( k );

        if( i == e )
            return;
        l->push_back( i->second );
    }
    
    //add leaf
    void dpbtree::addLeaf( char *key, unsigned int key_size, void *o )
    {
        dpbtree_key k( this );
        k.set( key, key_size );
        this->m.insert( std::pair<dpbtree_key, void *>( k, o ) );
    }
    
    //clear
    void dpbtree::clear( void )
    {
        this->dptree::clear();
    }
    
    //remove leaf
    void dpbtree::removeLeaf( void *o )
    {
        std::map<dpbtree_key, void *>::iterator i, e;
        
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
    void dpbtree::clearLeaves( void )
    {
        this->m.clear();
    }
    
    //clear branches
    void dpbtree::clearBranches( void )
    {
    }
    
    //ovverride to generate branches
    dptree *dpbtree::genBranch( void )
    {
        return new dpbtree();
    }
    
    //compare key, return >0 if a > b, <0 if a<b or 0 if a==b
    int dpbtree::compareKey( char *k_a, char *k_b, unsigned int k_a_size, unsigned int k_b_size )
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
    
    //get leaves
    void dpbtree::getLeaves( std::list< void *> *l )
    {
        std::map<dpbtree_key, void *>::iterator i, e;
        
        e = this->m.end();
        for( i = this->m.begin(); i != e; ++i )
        {
            l->push_back( i->second );
        }
    }
    
    //ctor
    dpbtree_key::dpbtree_key( dpbtree *t )
    {
        this->t = t;
        this->size = 0;
        this->ptr = 0;
    }

    //ctor
    dpbtree_key::dpbtree_key( const dpbtree_key &c )
    {
        this->t = c.t;
        this->ptr = 0;
        this->set( c.ptr, c.size );
    }
    
    //dtor
    dpbtree_key::~dpbtree_key( void )
    {
        if( this->ptr )
            delete[] this->ptr;
    }
    
    //set value
    void dpbtree_key::set( char *k, unsigned int sz )
    {
        if( this->ptr )
            delete[] this->ptr;
        this->ptr = new char[ sz ];
        this->size = sz;
        if( this->ptr )
            memcpy( this->ptr, k, sz );
    }
    
    //equals
    bool dpbtree_key::operator==(const dpbtree_key &b ) const
    {
        return this->t->compareKey( this->ptr, b.ptr, this->size, b.size ) == 0;
    }
    
    //not equals
    bool dpbtree_key::operator!=(const dpbtree_key &b) const
    {
        return this->t->compareKey( this->ptr, b.ptr, this->size, b.size ) != 0;
    }
    
    //less than
    bool dpbtree_key::operator<(const dpbtree_key &b) const
    {
        return this->t->compareKey( this->ptr, b.ptr, this->size, b.size ) < 0;
    }
    
    //greater than
    bool dpbtree_key::operator>(const dpbtree_key &b) const
    {
        return this->t->compareKey( this->ptr, b.ptr, this->size, b.size ) > 0;
    }
    
    //less than or equal
    bool dpbtree_key::operator<=(const dpbtree_key &b) const
    {
        return this->t->compareKey( this->ptr, b.ptr, this->size, b.size ) <= 0;
    }
    
    //greater than or equal
    bool dpbtree_key::operator>=(const dpbtree_key &b) const
    {
        return this->t->compareKey( this->ptr, b.ptr, this->size, b.size ) >= 0;
    }

    //assignment
    dpbtree_key& dpbtree_key::operator=( const dpbtree_key& b )
    {
        this->t = b.t;
        this->ptr = 0;
        this->set( b.ptr, b.size );
        return *this;
    }
    
};
