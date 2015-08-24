
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
        
        //compare key, return >0 if a > b, <0 if a<b or 0 if a==b
        virtual int compareKey( char *k_a, char *k_b, unsigned int k_a_size, unsigned int k_b_size );
 
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