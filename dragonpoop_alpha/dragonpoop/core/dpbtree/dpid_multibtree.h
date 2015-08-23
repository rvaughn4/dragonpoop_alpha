
#ifndef dragonpoop_dpid_multibtree_h
#define dragonpoop_dpid_multibtree_h

#include "dpmultibtree.h"
#include "../dpid/dpid.h"

namespace dragonpoop
{
    
    class dpid_multibtree : public dpmultibtree
    {
    private:
        
        dpid k;
        
    protected:
        
        //ovverride to handle deleteion of leaf
        virtual void onRemoveLeaf( void *o );
        //ovverride to generate branches
        virtual dptree *genBranch( void );
        //set key
        virtual void setKey( char *k, unsigned int sz );
        //compare key
        virtual int compareKey( char *k, unsigned int sz );
        
    public:
        
        //ctor
        dpid_multibtree( void );
        //dtor
        virtual ~dpid_multibtree( void );
        //find leaf
        void *findLeaf( dpid id );
        //add leaf
        void addLeaf( dpid id, void *o );
        //find leaves
        void findLeaves( dpid id, std::list<void *> *l );
        
    };
    
};

#endif