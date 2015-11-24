
#ifndef dragonpoop_dpheight_cache_h
#define dragonpoop_dpheight_cache_h

#include "../../core/shared_obj/shared_obj.h"
#include "../dpposition/dpposition.h"
#include <atomic>
#include <memory>

namespace dragonpoop
{

    class dpheight_cache_ref;

    class dpheight_cache : public shared_obj
    {

    private:

        dpposition pos;
        std::shared_ptr<std::atomic<uint64_t>> t;
        float *fv;
        unsigned int w, h, tile_size;
        int64_t x, z;

    protected:

        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //set camera position
        void setPosition( dpposition *p );
        //get camera position
        dpposition *getPosition( void );
        //get position time
        uint64_t getTime( void );
        //set time
        void setTime( uint64_t t );
        //resize and set center
        void setDimensions( double w, double h, double x, double z, double tile_size );
        //set height at coord
        void setHeight( double x, double z, float h );
        //get height at coord
        float getHeight( double x, double z );
        //get height at tile
        float _getHeight( unsigned int ix, unsigned int iz );
        //clear
        void clear( void );
        //sync
        void sync( dpheight_cache_ref *r );

    public:

        //ctor
        dpheight_cache( dpmutex_master *mm );
        //dtor
        virtual ~dpheight_cache( void );


        friend class dpheight_cache_writelock;
        friend class dpheight_cache_readlock;
        friend class dpheight_cache_ref;

    };

};

#endif
