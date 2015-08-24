
#include "dpid_multibtree.h"

namespace dragonpoop
{
    
    //ctor
    dpid_multibtree::dpid_multibtree( void )
    {
    }
    
    //dtor
    dpid_multibtree::~dpid_multibtree( void )
    {
        this->clear();
    }
    
    //find leaf
    void *dpid_multibtree::findLeaf( dpid id )
    {
        return this->dpmultibtree::findLeaf( (char *)&id, sizeof( id ) );
    }
    
    //find leaves
    void dpid_multibtree::findLeaves( dpid id, std::list<void *> *l )
    {
        this->dpmultibtree::findLeaves( (char *)&id, sizeof( id ), l );
    }
    
    //add leaf
    void dpid_multibtree::addLeaf( dpid id, void *o )
    {
        this->dpmultibtree::addLeaf( (char *)&id, sizeof( id ), o );
    }
    
    //compare key, return >0 if a > b, <0 if a<b or 0 if a==b
    int dpid_multibtree::compareKey( char *k_a, char *k_b, unsigned int k_a_size, unsigned int k_b_size )
    {
        if( k_a_size != sizeof( dpid ) || k_b_size != sizeof( dpid ) )
            return -1;
        return dpid_signed_compare( (dpid *)k_a, (dpid *)k_b );
    }
    
};
