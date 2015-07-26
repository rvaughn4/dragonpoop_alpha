
#ifndef dragonpoop_dpindex_buffer_h
#define dragonpoop_dpindex_buffer_h

#include "dpindex.h"
#include "../../core/dpid/dpid.h"
#include "dpvertex.h"

namespace dragonpoop
{

    class dpvertex_buffer;

    class dpindex_buffer
    {

    private:

        struct
        {
            dpindex *ptr;
            unsigned int cnt, max;
        } buffer;

    protected:

    public:

        //ctor
        dpindex_buffer( void );
        //dtor
        virtual ~dpindex_buffer( void );
        //copy
        void copy( dpindex_buffer *c );
        //get size
        unsigned int getSize( void );
        //get buffer
        dpindex *getBuffer( void );
        //add index
        void addIndex( dpindex *i );
        //add index by vertex
        void addIndex( dpvertex_buffer *vb, dpvertex *v, dpid id );
        //clear
        void clear( void );

    };

};

#endif