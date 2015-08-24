
#ifndef dragonpoop_dpmultibtree_h
#define dragonpoop_dpmultibtree_h

#include "dptree.h"
#include <map>

namespace dragonpoop
{
    
    class dpmultibtree_key;
    class dpmultibtree;
    
    class dpmultibtree_key
    {
        
        char *ptr;
        unsigned int size;
        dpmultibtree *t;
        
    public:
        
        //ctor
        dpmultibtree_key( dpmultibtree *t );
        //copy ctor
        dpmultibtree_key( const dpmultibtree_key &c );
        //dtor
        virtual ~dpmultibtree_key( void );
        //set value
        void set( char *k, unsigned int sz );
        //equals
        bool operator==(const dpmultibtree_key &b ) const;
        //not equals
        bool operator!=(const dpmultibtree_key &b) const;
        //less than
        bool operator<(const dpmultibtree_key &b) const;
        //greater than
        bool operator>(const dpmultibtree_key &b) const;
        //less than or equal
        bool operator<=(const dpmultibtree_key &b) const;
        //greater than or equal
        bool operator>=(const dpmultibtree_key &b) const;
        //assignment
        dpmultibtree_key& operator=( const dpmultibtree_key& b );
        
    };
    
    class dpmultibtree : public dptree
    {
    private:
        
        std::multimap<dpmultibtree_key, void *> m;
        
    protected:
        
        //clear leaves
        virtual void clearLeaves( void );
        //clear branches
        virtual void clearBranches( void );
        //compare keys
        int compareKey( char *k_a, char *k_b, unsigned int k_a_size, unsigned int k_b_size );
        //ovverride to generate branches
        virtual dptree *genBranch( void );

    public:
        
        //ctor
        dpmultibtree( void );
        //dtor
        virtual ~dpmultibtree( void );
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
        
        friend class dpmultibtree_key;
    };
    
};

#endif