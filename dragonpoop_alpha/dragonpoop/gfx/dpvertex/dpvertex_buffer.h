
#ifndef dragonpoop_dpvertex_buffer_h
#define dragonpoop_dpvertex_buffer_h

#include "dpvertex.h"

namespace dragonpoop
{

    class dpvertex_buffer
    {

    private:

        struct
        {
            dpvertex *ptr;
            unsigned int cnt, max;
        } buffer;

    protected:

    public:

        //ctor
        dpvertex_buffer( void );
        //dtor
        virtual ~dpvertex_buffer( void );
        //copy
        void copy( dpvertex_buffer *c );
        //get size
        unsigned int getSize( void );
        //get buffer
        dpvertex *getBuffer( void );
        //add vertex
        uint16_t addVertex( dpvertex *v );
        //clear
        void clear( void );

    };
    
};

#endif