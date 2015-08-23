
#ifndef dragonpoop_multibytetree_h
#define dragonpoop_multibytetree_h

#include "../dpbtree/dptree.h"

namespace dragonpoop
{

    class multibytetree : public dptree
    {
    private:

        //branches
        multibytetree *branches[ 256 ];
        //leaves
        std::list<void *> leaves;

    protected:

        //clear leaves
        virtual void clearLeaves( void );
        //clear branches
        virtual void clearBranches( void );
        //ovverride to handle deleteion of leaf
        virtual void onRemoveLeaf( void *o );
        //ovverride to generate branches
        virtual multibytetree *genBranch( void );

    public:

        //ctor
        multibytetree( void );
        //dtor
        virtual ~multibytetree( void );
        //find leaf
        virtual void *findLeaf( char *key, unsigned int key_size );
        //find leaves
        virtual void findLeaves( char *key, unsigned int key_size, std::list<void *> *l );
        //add leaf
        virtual void addLeaf( char *key, unsigned int key_size, void *o );
        //remove leaf
        virtual void removeLeaf( void *o );
        //get leaves
        virtual void getLeaves( std::list< void *> *l );
        
    };
    
};

#endif