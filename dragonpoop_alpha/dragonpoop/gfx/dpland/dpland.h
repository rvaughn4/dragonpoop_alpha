
#ifndef dragonpoop_dpland_h
#define dragonpoop_dpland_h

#include "../dpposition/dpposition.h"
#include "../../core/dpid/dpid.h"
#include "../dpvertex/dpvertex_buffer.h"
#include "../dpvertex/dpindex_buffer.h"

namespace dragonpoop
{
    
    class dpland
    {
        
    private:
        
        struct
        {
            int64_t x, z;
        } pos;
        dpid id;
        dpvertex_buffer vb;
        dpindex_buffer ib;
        float land_sz, tile_sz, tex_size;
        
        //build land
        void build( void );
        
    protected:
        
    public:
        
        //ctor
        dpland( dpid id, int64_t x, int64_t z, float land_sz, float tile_sz, float tex_size );
        //dtor
        virtual ~dpland( void );
        //get position
        void getPosition( dpposition *p );
        //get position
        void getPosition( int64_t *x, int64_t *z );
        //returns id
        dpid getId( void );
        //returns vertex buffer
        dpvertex_buffer *getVB( void );
        //returns index buffer
        dpindex_buffer *getIB( void );
        
    };
    
};

#endif