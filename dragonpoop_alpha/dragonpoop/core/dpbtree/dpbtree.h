
#ifndef dragonpoop_dpbtree_h
#define dragonpoop_dpbtree_h

#include "dptree.h"
#include <map>

namespace dragonpoop
{
    
    class dpbtree_key;
    class dpbtree;
    
    class dpbtree_key
    {
        
        char *ptr;
        unsigned int size;
        dpbtree *t;
        
    public:
        
        //ctor
        dpbtree_key( dpbtree *t );
        //copy ctor
        dpbtree_key( const dpbtree_key &c );
        //dtor
        virtual ~dpbtree_key( void );
        //set value
        void set( char *k, unsigned int sz );
        //equals
        bool operator==(const dpbtree_key &b ) const;
        //not equals
        bool operator!=(const dpbtree_key &b) const;
        //less than
        bool operator<(const dpbtree_key &b) const;
        //greater than
        bool operator>(const dpbtree_key &b) const;
        //less than or equal
        bool operator<=(const dpbtree_key &b) const;
        //greater than or equal
        bool operator>=(const dpbtree_key &b) const;
        //assignment
        dpbtree_key& operator=( const dpbtree_key& b );
        
    };
    
    class dpbtree : public dptree
    {
    private:
        
        std::map<dpbtree_key, void *> m;
        
    protected:
        
        //clear leaves
        virtual void clearLeaves( void );
        //clear branches
        virtual void clearBranches( void );
        //ovverride to generate branches
        virtual dptree *genBranch( void );
        //compare key, return >0 if a > b, <0 if a<b or 0 if a==b
        virtual int compareKey( char *k_a, char *k_b, unsigned int k_a_size, unsigned int k_b_size );
        
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
        //get leaves
        virtual void getLeaves( std::list< void *> *l );
        //find leaves
        virtual void findLeaves( char *key, unsigned int key_size, std::list<void *> *l );
        
        friend class dpbtree_key;
    };
    
};

#endif