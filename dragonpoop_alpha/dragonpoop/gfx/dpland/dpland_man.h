
#ifndef dragonpoop_dpland_man_h
#define dragonpoop_dpland_man_h

#include "../../core/shared_obj/shared_obj.h"
#include "../dpposition/dpposition.h"
#include "dpland_defines.h"

#include <stdint.h>
 
namespace dragonpoop
{
    
    class dpthread_lock;
    class core;
    class dpland_man_ref;
    class dpland_man_readlock;
    class dpland_man_writelock;
    class gfx_ref;
    
    class dpland_man : public shared_obj
    {
        
    private:
        
        core *c;
        gfx_ref *g;
        struct
        {
            int64_t x, z;
        } pos;
        
        //run tiles
        void runTiles( dpthread_lock *thd, dpland_man_writelock *g );
        //create new tiles
        void makeTiles( dpthread_lock *thd, dpland_man_writelock *g );
        //returns true if tile exists
        bool hasTile( int64_t x, int64_t z );
        
    protected:
        
        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //run
        void run( dpthread_lock *thd, dpland_man_writelock *g );
        
    public:
        
        //ctor
        dpland_man( core *c );
        //dtor
        virtual ~dpland_man( void );
        //return core
        core *getCore( void );
        
        friend class dpland_man_readlock;
        friend class dpland_man_writelock;
        
    };
    
};

#endif