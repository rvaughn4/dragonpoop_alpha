
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
    
    //ovverride to handle deleteion of leaf
    void dpid_btree::onRemoveLeaf( void *o )
    {
        
    }
    
    //ovverride to generate branches
    dpbtree *dpid_btree::genBranch( void )
    {
        return new dpid_btree();
    }
    
};
