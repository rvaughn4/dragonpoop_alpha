
#ifndef dragonpoop_dpmultibtree_h
#define dragonpoop_dpmultibtree_h

#include "dptree.h"
#include <atomic>

namespace dragonpoop
{
    
#define dpmultibtree_left 0
#define dpmultibtree_right 1
#define dpmultibtree_leaf 2
    
    class dpmultibtree : public dptree
    {
    private:
        
        //branches
        std::atomic<dpmultibtree *> leftbranch, rightbranch;
        //leaf
        struct
        {
            std::atomic<void **> buffer;
            std::atomic<unsigned int> size;
        } leaves;
        //key
        struct
        {
            std::atomic<unsigned int> size;
            std::atomic<char *> buffer;
        } key;
        std::atomic<bool> hasKey;
        
        //add new leaf using free space
        bool _addUseFreeSpace( void * );
        //add new leaf by increasing size of buffer
        bool _addAllocSpace( void *v );
        //remove leaf by freeing slot
        bool _remove( void *v );
        //clear leaves buffer
        void _clear( void );
        //copy leaves
        void _copy( std::list< void *> *l );
        
    protected:
        
        //clear leaves
        virtual void clearLeaves( void );
        //clear branches
        virtual void clearBranches( void );
        //ovverride to handle deleteion of leaf
        virtual void onRemoveLeaf( void *o );
        //ovverride to generate branches
        virtual dptree *genBranch( void );
        //set key
        virtual void setKey( char *k, unsigned int sz );
        //compare key
        virtual int compareKey( char *k, unsigned int sz );
        
    public:
        
        //ctor
        dpmultibtree( void );
        //dtor
        virtual ~dpmultibtree( void );
        //find leaf
        virtual void *findLeaf( char *key, unsigned int key_size );
        //add leaf
        virtual void addLeaf( char *key, unsigned int key_size, void *o );
        //clear
        virtual void clear( void );
        //remove leaf
        virtual void removeLeaf( void *o );
        //get leaves
        virtual void getLeaves( std::list< void *> *l );
        //find leaves
        virtual void findLeaves( char *key, unsigned int key_size, std::list<void *> *l );
        
    };
    
};

#endif