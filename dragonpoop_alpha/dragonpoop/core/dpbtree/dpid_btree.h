
#ifndef dragonpoop_dpid_btree_h
#define dragonpoop_dpid_btree_h

#include "dpbtree.h"
#include "../dpid/dpid.h"

namespace dragonpoop
{
    
    class dpid_btree : public dpbtree
    {
    private:
        
    protected:
        
        //ovverride to handle deleteion of leaf
        virtual void onRemoveLeaf( void *o );
        //ovverride to generate branches
        virtual dpbtree *genBranch( void );
        
    public:
        
        //ctor
        dpid_btree( void );
        //dtor
        virtual ~dpid_btree( void );
        //find leaf
        void *findLeaf( dpid id );
        //add leaf
        void addLeaf( dpid id, void *o );
        
    };
    
};

#endif