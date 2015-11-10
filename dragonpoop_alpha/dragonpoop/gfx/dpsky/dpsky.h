
#ifndef dragonpoop_dpsky_h
#define dragonpoop_dpsky_h

#include "../dpvertex/dpvertex_buffer.h"
#include "../dpvertex/dpindex_buffer.h"
#include "../dpbitmap/dpbitmap.h"

namespace dragonpoop
{

    struct dpsky_stuff
    {
        struct
        {
            struct
            {
                dpvertex_buffer vb;
                dpindex_buffer ib;
            } front, back, top, bottom, left, right;
        } skybox;
        
        struct
        {
            struct
            {
                dpbitmap front, back, top, bottom, left, right;
            } stars, mask;
        } skyboxtex;
        
    };

};

#endif