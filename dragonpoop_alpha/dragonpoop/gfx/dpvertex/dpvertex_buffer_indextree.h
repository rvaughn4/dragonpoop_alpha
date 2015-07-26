
#ifndef dragonpoop_dpvertex_buffer_indextree_h
#define dragonpoop_dpvertex_buffer_indextree_h

#include "../../core/bytetree/dpid_bytetree.h"
#include "dpindex.h"
#include "dpvertex.h"
#include <stdint.h>

namespace dragonpoop
{

    class dpvertex_buffer_indextree : public bytetree
    {
    private:

    protected:

    public:

        //ctor
        dpvertex_buffer_indextree( void );
        //dtor
        virtual ~dpvertex_buffer_indextree( void );
        //find leaf
        bool findLeaf( dpvertex *v, uint16_t *p_offset );
        //add leaf
        void addLeaf( dpvertex *v, uint16_t offset );
        
    };
    
};

#endif