
#include "model_component_tree_bytype.h"

namespace dragonpoop
{
    
    //ctor
    model_component_tree_bytype::model_component_tree_bytype( void )
    {
        
    }
    
    //dtor
    model_component_tree_bytype::~model_component_tree_bytype( void )
    {
        
    }
    
    //find leaves
    void model_component_tree_bytype::findLeaves( uint16_t mtype, std::list<model_component *> *l )
    {
        this->multibytetree::findLeaves( (char *)&mtype, sizeof( uint16_t ), (std::list<void *> *)l );
    }
    
    //find leaf
    model_component *model_component_tree_bytype::findLeaf( uint16_t mtype )
    {
        return (model_component *)this->multibytetree::findLeaf( (char *)&mtype, sizeof( uint16_t ) );
    }
    
    //add leaf
    void model_component_tree_bytype::addLeaf( uint16_t mtype, model_component *o )
    {
        this->multibytetree::addLeaf( (char *)&mtype, sizeof( uint16_t ), (void *)o );
    }
    
    //remove leaf
    void model_component_tree_bytype::removeLeaf( model_component *o )
    {
        this->multibytetree::removeLeaf( (void *)o );
    }

};
