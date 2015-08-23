
#include "dptree.h"

namespace dragonpoop
{
    
    //ctor
    dptree::dptree( void )
    {
        
    }
    
    //dtor
    dptree::~dptree( void )
    {
        
    }
    
    //ovverride to handle deleteion of leaf
    void dptree::onRemoveLeaf( void *o )
    {
        
    }
    
    //clear
    void dptree::clear( void )
    {
        this->clearBranches();
        this->clearLeaves();
    }
};

