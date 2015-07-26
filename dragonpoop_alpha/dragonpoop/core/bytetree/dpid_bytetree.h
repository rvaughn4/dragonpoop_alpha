
#ifndef dragonpoop_dpid_bytetree_h
#define dragonpoop_dpid_bytetree_h

#include "bytetree.h"
#include "../dpid/dpid.h"

namespace dragonpoop
{

    class dpid_bytetree : public bytetree
    {
    private:

    protected:

        //ovverride to handle deleteion of leaf
        virtual void onRemoveLeaf( void *o );
        //ovverride to generate branches
        virtual bytetree *genBranch( void );

    public:

        //ctor
        dpid_bytetree( void );
        //dtor
        virtual ~dpid_bytetree( void );
        //find leaf
        void *findLeaf( dpid id );
        //add leaf
        void addLeaf( dpid id, void *o );

    };
    
};

#endif