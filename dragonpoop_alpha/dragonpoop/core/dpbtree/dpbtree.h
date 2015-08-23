
#ifndef dragonpoop_dpbtrees_h
#define dragonpoop_dpbtrees_h

#include "dptree.h"

namespace dragonpoop
{
    
#define dpbtree_left 0
#define dpbtree_right 1
#define dpbtree_leaf 2
    
    class dpbtree : public dptree
    {
    private:
        
        //branches
        dpbtree *leftbranch, *rightbranch;
        //leaf
        void *o;
        //key
        struct
        {
            unsigned int size;
            char *buffer;
        } key;
        
    protected:
        
        //clear leaves
        virtual void clearLeaves( void );
        //clear branches
        virtual void clearBranches( void );
        //ovverride to handle deleteion of leaf
        virtual void onRemoveLeaf( void *o );
        //ovverride to generate branches
        virtual dpbtree *genBranch( void );
        //set key
        void setKey( char *k, unsigned int sz );
        //compare key
        int compareKey( char *k, unsigned int sz );
        
    public:
        
        //ctor
        dpbtree( void );
        //dtor
        virtual ~dpbtree( void );
        //find leaf
        virtual void *findLeaf( char *key, unsigned int key_size );
        //add leaf
        virtual void addLeaf( char *key, unsigned int key_size, void *o );
        //clear
        virtual void clear( void );
        //remove leaf
        virtual void removeLeaf( void *o );
        
    };
    
};

#endif