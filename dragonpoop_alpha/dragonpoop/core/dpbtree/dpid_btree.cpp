
#include "dpid_btree.h"

namespace dragonpoop
{
    
    //ctor
    dpid_btree::dpid_btree( void )
    {
    }
    
    //dtor
    dpid_btree::~dpid_btree( void )
    {
        this->clear();
    }
    
    //find leaf
    void *dpid_btree::findLeaf( dpid id )
    {
        return this->dpbtree::findLeaf( (char *)&id, sizeof( id ) );
    }
    
    //add leaf
    void dpid_btree::addLeaf( dpid id, void *o )
    {
        this->dpbtree::addLeaf( (char *)&id, sizeof( id ), o );
    }
    
    //compare key, return >0 if a > b, <0 if a<b or 0 if a==b
    int dpid_btree::compareKey( char *k_a, char *k_b, unsigned int k_a_size, unsigned int k_b_size )
    {
        if( k_a_size != sizeof( dpid ) || k_b_size != sizeof( dpid ) )
            return -1;
        return dpid_signed_compare( (dpid *)k_a, (dpid *)k_b );
    }
    
};
