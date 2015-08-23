
#ifndef dragonpoop_dptree_h
#define dragonpoop_dptree_h

#include <list>

namespace dragonpoop
{
    
    class dptree
    {
    private:
       
    protected:
        
        //clear leaves
        virtual void clearLeaves( void ) = 0;
        //clear branches
        virtual void clearBranches( void ) = 0;
        //ovverride to handle deleteion of leaf
        virtual void onRemoveLeaf( void *o );
        //ovverride to generate branches
        virtual dptree *genBranch( void ) = 0;
        
    public:
        
        //ctor
        dptree( void );
        //dtor
        virtual ~dptree( void );
        //find leaf
        virtual void *findLeaf( char *key, unsigned int key_size ) = 0;
        //add leaf
        virtual void addLeaf( char *key, unsigned int key_size, void *o ) = 0;
        //clear
        virtual void clear( void );
        //remove leaf
        virtual void removeLeaf( void *o ) = 0;
        //get leaves
        virtual void getLeaves( std::list< void *> *l ) = 0;
        //find leaves
        virtual void findLeaves( char *key, unsigned int key_size, std::list<void *> *l ) = 0;
 
    };
    
};

#endif