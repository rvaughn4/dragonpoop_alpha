
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
        virtual dptree *genBranch( void );
        
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
        //get leaves
        virtual void getLeaves( std::list< void *> *l );
        //find leaves
        virtual void findLeaves( char *key, unsigned int key_size, std::list<void *> *l );

    };

};

#endif