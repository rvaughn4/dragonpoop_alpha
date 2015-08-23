
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
    
    //set key
    void dpid_btree::setKey( char *k, unsigned int sz )
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
    int dpid_btree::compareKey( char *k, unsigned int sz )
    {
        dpid *c = (dpid *)k;
        if( sz != sizeof( dpid ) )
            return 5;
        
        if( this->k.randno > c->randno )
            return dpbtree_left;
        if( this->k.randno < c->randno )
            return dpbtree_right;
        
        if( this->k.counter > c->counter )
            return dpbtree_left;
        if( this->k.counter < c->counter )
            return dpbtree_right;

        if( this->k.addr > c->addr )
            return dpbtree_left;
        if( this->k.addr < c->addr )
            return dpbtree_right;
        
        if( this->k.ticks > c->ticks )
            return dpbtree_left;
        if( this->k.ticks < c->ticks )
            return dpbtree_right;

        if( this->k.epoch > c->epoch )
            return dpbtree_left;
        if( this->k.epoch < c->epoch )
            return dpbtree_right;
        
        return dpbtree_leaf;
    }
    
};
