
#ifndef dragonpoop_bytetree_h
#define dragonpoop_bytetree_h

namespace dragonpoop
{

    class bytetree
    {
    private:

        //branches
        bytetree *branches[ 256 ];
        //leaf
        void *o;

    protected:

        //clear leaves
        void clearLeaves( void );
        //clear branches
        void clearBranches( void );
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
        void *findLeaf( char *key, unsigned int key_size );
        //add leaf
        void addLeaf( char *key, unsigned int key_size, void *o );
        //clear
        void clear( void );
        //remove leaf
        void removeLeaf( void *o );

    };

};

#endif