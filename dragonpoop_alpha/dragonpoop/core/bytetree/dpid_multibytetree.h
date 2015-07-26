
#ifndef dragonpoop_dpid_multibytetree_h
#define dragonpoop_dpid_multibytetree_h

#include "multibytetree.h"
#include "../dpid/dpid.h"

namespace dragonpoop
{

    class dpid_multibytetree : public multibytetree
    {
    private:

    protected:

        //ovverride to handle deleteion of leaf
        virtual void onRemoveLeaf( void *o );
        //ovverride to generate branches
        virtual multibytetree *genBranch( void );

    public:

        //ctor
        dpid_multibytetree( void );
        //dtor
        virtual ~dpid_multibytetree( void );
        //find leaf
        void *findLeaf( dpid id );
        //find leaves
        void findLeaves( dpid id, std::list<void *> *l );
        //add leaf
        void addLeaf( dpid id, void *o );

    };
    
};

#endif