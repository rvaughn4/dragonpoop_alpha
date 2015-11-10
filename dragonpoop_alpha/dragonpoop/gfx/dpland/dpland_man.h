
#ifndef dragonpoop_dpland_man_h
#define dragonpoop_dpland_man_h

#include "../../core/shared_obj/shared_obj.h"
#include "../dpposition/dpposition.h"
#include "dpland_defines.h"
#include "../dpvertex/dpvertex_buffer.h"
#include "../dpvertex/dpindex_buffer.h"
#include "../dpbitmap/dpbitmap.h"

#include <stdint.h>
 
namespace dragonpoop
{
    
    class dpthread_lock;
    class core;
    class dpland_man_ref;
    class dpland_man_readlock;
    class dpland_man_writelock;
    class gfx_ref;
    class dptask;
    class dpland_man_task;
    class dptaskpool_writelock;
    class gfx;
    class dpland;

    class dpland_man : public shared_obj
    {
        
    private:
        
        core *c;
        gfx_ref *g;
        struct
        {
            int64_t x, z;
        } pos;
        dptask *tsk;
        dpland_man_task *gtsk;
        std::list<dpland *> tiles;
        float land_sz, tile_sz, tex_sz, world_sz;

        //start task
        void _startTask( dptaskpool_writelock *tp, unsigned int ms_delay );
        //kill task
        void _killTask( void );
        //delete task
        void _deleteTask( void );
        //run tiles
        void runTiles( dpthread_lock *thd, dpland_man_writelock *g );
        //returns tile
        dpland *getTile( int64_t x, int64_t z );
        //makes a tile at coords
        void makeTile( dpthread_lock *thd, int64_t x, int64_t z );
        
    protected:
        
        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //run
        void run( dpthread_lock *thd, dpland_man_writelock *g );
        //get tiles
        void getTiles( std::list<dpland *> *l );
        
    public:
        
        //ctor
        dpland_man( core *c, gfx *g, dptaskpool_writelock *tp );
        //dtor
        virtual ~dpland_man( void );
        //return core
        core *getCore( void );
        
        friend class dpland_man_readlock;
        friend class dpland_man_writelock;
        
    };
    
};

#endif