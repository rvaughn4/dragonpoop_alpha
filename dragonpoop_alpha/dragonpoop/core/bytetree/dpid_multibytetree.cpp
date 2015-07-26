
#include "dpid_multibytetree.h"
#include "../dpid/dpid.h"

namespace dragonpoop
{

    //ctor
    dpid_multibytetree::dpid_multibytetree( void )
    {

    }

    //dtor
    dpid_multibytetree::~dpid_multibytetree( void )
    {
        this->clear();
    }

    //find leaf
    void *dpid_multibytetree::findLeaf( dpid id )
    {
        return this->multibytetree::findLeaf( (char *)&id, sizeof( id ) );
    }

    //find leaves
    void dpid_multibytetree::findLeaves( dpid id, std::list<void *> *l )
    {
        this->multibytetree::findLeaves( (char *)&id, sizeof( id ), l );
    }

    //add leaf
    void dpid_multibytetree::addLeaf( dpid id, void *o )
    {
        this->multibytetree::addLeaf( (char *)&id, sizeof( id ), o );
    }

    //ovverride to handle deleteion of leaf
    void dpid_multibytetree::onRemoveLeaf( void *o )
    {

    }

    //ovverride to generate branches
    multibytetree *dpid_multibytetree::genBranch( void )
    {
        return new dpid_multibytetree();
    }

};
