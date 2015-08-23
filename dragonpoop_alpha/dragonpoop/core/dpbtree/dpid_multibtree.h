
#ifndef dragonpoop_dpid_multibtree_h
#define dragonpoop_dpid_multibtree_h

#include "dpbtree.h"
#include "../dpid/dpid.h"

namespace dragonpoop
{
    
    class dpid_multibtree : public dpbtree
    {
    private:
        
        dpid k;
        
    protected:
        
        //ovverride to handle deleteion of leaf
        virtual void onRemoveLeaf( void *o );
        //ovverride to generate branches
        virtual dpbtree *genBranch( void );
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
        
    };
    
};

#endif