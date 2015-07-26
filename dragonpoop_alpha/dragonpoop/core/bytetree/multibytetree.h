
#ifndef dragonpoop_multibytetree_h
#define dragonpoop_multibytetree_h

#include <list>

namespace dragonpoop
{

    class multibytetree
    {
    private:

        //branches
        multibytetree *branches[ 256 ];
        //leaves
        std::list<void *> leaves;

    protected:

        //clear leaves
        void clearLeaves( void );
        //clear branches
        void clearBranches( void );
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
        void *findLeaf( char *key, unsigned int key_size );
        //find leaves
        void findLeaves( char *key, unsigned int key_size, std::list<void *> *l );
        //add leaf
        void addLeaf( char *key, unsigned int key_size, void *o );
        //clear
        void clear( void );
        //remove leaf
        void removeLeaf( void *o );
        //get leaves
        void getLeaves( std::list< void *> *l );
        
    };
    
};

#endif