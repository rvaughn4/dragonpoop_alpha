
#ifndef dragonpoop_bytetree_h
#define dragonpoop_bytetree_h

#include "../dpbtree/dptree.h"

namespace dragonpoop
{

    class bytetree : public dptree
    {
    private:

        //branches
        bytetree *branches[ 256 ];
        //leaf
        void *o;

    protected:

        //clear leaves
        virtual void clearLeaves( void );
        //clear branches
        virtual void clearBranches( void );
        //ovverride to handle deleteion of leaf
        virtual void onRemoveLeaf( void *o );
        //ovverride to generate branches
        virtual bytetree *genBranch( void );
        
    public:

        //ctor
        bytetree( void );
        //dtor
        virtual ~bytetree( void );
        //find leaf
        virtual void *findLeaf( char *key, unsigned int key_size );
        //add leaf
        virtual void addLeaf( char *key, unsigned int key_size, void *o );
        //remove leaf
        virtual void removeLeaf( void *o );

    };

};

#endif