
#ifndef dragonpoop_dpposition_h
#define dragonpoop_dpposition_h

#include <stdint.h>
#include "../dpvertex/dpxyz_f.h"

namespace dragonpoop
{

#pragma pack( 1 )
    struct dpposition_inner_half
    {
        double x, y, z;
        uint64_t t;
        bool bReverse;
    };
#pragma pack()

#pragma pack( 1 )
    struct dpposition_inner
    {
        dpposition_inner_half start, end;
    };
#pragma pack( 0 )
    
    class dpposition
    {
        
    private:
        
        dpposition_inner i;
        dpxyz_f old_rot;
        
    protected:
        
    public:
        
        //ctor
        dpposition( void );
        //ctor
        dpposition( dpposition *c );
        //dtor
        virtual ~dpposition( void );
        //express difference as float ( arg - this )
        void getDifference( dpposition *arg, uint64_t tm_epoch_now, dpxyz_f *pout );
        //set location
        void setLocation( dpposition *arg, uint64_t tm_epoch_now, uint64_t tm_epoch_when );
        //get data
        void getData( dpposition_inner *pout );
        //set data
        void setData( dpposition_inner *pin );
        //copy
        void copy( dpposition *c );
        //returns true if moving
        bool isMoving( uint64_t tm_epoch_now );
        //gets velocity and speed
        float getSpeed( dpxyz_f *pvel );
        //get rotational direction
        void getDirection( dpxyz_f *pout );
        //move position incrementally
        void move( dpxyz_f *pin, uint64_t t_epoch_now, uint64_t t_epoch_when, bool doReverse );
        //stop moving
        void stop( uint64_t t_epoch_now );
        //move immediatly
        void move( dpxyz_f *x );
        //find current position
        void getPosition( dpposition_inner_half *h, uint64_t t_now );
        
    };
    
};

#endif