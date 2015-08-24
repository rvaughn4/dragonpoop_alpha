
#ifndef dragonpoop_dpid_multibtree_h
#define dragonpoop_dpid_multibtree_h

#include "dpmultibtree.h"
#include "../dpid/dpid.h"

namespace dragonpoop
{
    
    class dpid_multibtree : public dpmultibtree
    {
    private:
        
    protected:
        
        //compare key, return >0 if a > b, <0 if a<b or 0 if a==b
        virtual int compareKey( char *k_a, char *k_b, unsigned int k_a_size, unsigned int k_b_size );
        
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