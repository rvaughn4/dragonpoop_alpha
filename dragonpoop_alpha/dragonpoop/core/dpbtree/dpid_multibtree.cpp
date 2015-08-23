
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
    
    //ovverride to handle deleteion of leaf
    void dpid_multibtree::onRemoveLeaf( void *o )
    {
        
    }
    
    //ovverride to generate branches
    dptree *dpid_multibtree::genBranch( void )
    {
        return new dpid_multibtree();
    }
    
    //set key
    void dpid_multibtree::setKey( char *k, unsigned int sz )
    {
        if( sz != sizeof( dpid ) )
            return;
        this->k.addr = ( (dpid *)k )->addr;
        this->k.counter = ( (dpid *)k )->counter;
        this->k.epoch = ( (dpid *)k )->epoch;
        this->k.randno = ( (dpid *)k )->randno;
        this->k.ticks = ( (dpid *)k )->ticks;
    }
    
    //compare key
    int dpid_multibtree::compareKey( char *k, unsigned int sz )
    {
        dpid *c = (dpid *)k;
        if( sz != sizeof( dpid ) )
            return 5;
        
        if( this->k.randno > c->randno )
            return dpmultibtree_left;
        if( this->k.randno < c->randno )
            return dpmultibtree_right;
        
        if( this->k.counter > c->counter )
            return dpmultibtree_left;
        if( this->k.counter < c->counter )
            return dpmultibtree_right;
        
        if( this->k.addr > c->addr )
            return dpmultibtree_left;
        if( this->k.addr < c->addr )
            return dpmultibtree_right;
        
        if( this->k.ticks > c->ticks )
            return dpmultibtree_left;
        if( this->k.ticks < c->ticks )
            return dpmultibtree_right;
        
        if( this->k.epoch > c->epoch )
            return dpmultibtree_left;
        if( this->k.epoch < c->epoch )
            return dpmultibtree_right;
        
        return dpmultibtree_leaf;
    }
    
};
