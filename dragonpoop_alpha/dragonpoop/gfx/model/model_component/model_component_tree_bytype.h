
#ifndef dragonpoop_model_component_tree_bytype_h
#define dragonpoop_model_component_tree_bytype_h

#include "../../../core/bytetree/multibytetree.h"
#include <stdint.h>

namespace dragonpoop
{

    class model_component;

    class model_component_tree_bytype : public multibytetree
    {
    private:

    protected:

    public:

        //ctor
        model_component_tree_bytype( void );
        //dtor
        virtual ~model_component_tree_bytype( void );
        //find leaves
        void findLeaves( uint16_t mtype, std::list<model_component *> *l );
        //find leaf
        model_component *findLeaf( uint16_t mtype );
        //add leaf
        void addLeaf( uint16_t mtype, model_component *o );
        //remove leaf
        void removeLeaf( model_component *o );

    };

};

#endif
