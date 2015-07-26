
#include "dpid_bytetree.h"

namespace dragonpoop
{

    //ctor
    dpid_bytetree::dpid_bytetree( void )
    {
    }

    //dtor
    dpid_bytetree::~dpid_bytetree( void )
    {
        this->clear();
    }

    //find leaf
    void *dpid_bytetree::findLeaf( dpid id )
    {
        return this->bytetree::findLeaf( (char *)&id, sizeof( id ) );
    }

    //add leaf
    void dpid_bytetree::addLeaf( dpid id, void *o )
    {
        this->bytetree::addLeaf( (char *)&id, sizeof( id ), o );
    }

    //ovverride to handle deleteion of leaf
    void dpid_bytetree::onRemoveLeaf( void *o )
    {

    }

    //ovverride to generate branches
    bytetree *dpid_bytetree::genBranch( void )
    {
        return new dpid_bytetree();
    }

};
