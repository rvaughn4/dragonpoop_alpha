
#include "model_component_tree_bytypeid.h"

namespace dragonpoop
{
    
    //ctor
    model_component_tree_bytypeid::model_component_tree_bytypeid( void )
    {
        
    }
    
    //dtor
    model_component_tree_bytypeid::~model_component_tree_bytypeid( void )
    {
        
    }
    
    //find leaves
    void model_component_tree_bytypeid::findLeaves( uint16_t mtype, dpid id, std::list<model_component *> *l )
    {
        model_component_tree_bytypeid_block b;
        
        b.mtype = mtype;
        b.id = id;
        
        this->multibytetree::findLeaves( (char *)&b, sizeof( b ), (std::list<void *> *)l );
    }

    //find leaf
    model_component *model_component_tree_bytypeid::findLeaf( uint16_t mtype, dpid id )
    {
        model_component_tree_bytypeid_block b;
        
        b.mtype = mtype;
        b.id = id;
        
        return (model_component *)this->multibytetree::findLeaf( (char *)&b, sizeof( b ) );
    }
    
    //add leaf
    void model_component_tree_bytypeid::addLeaf( uint16_t mtype, dpid id, model_component *o )
    {
        model_component_tree_bytypeid_block b;
        
        b.mtype = mtype;
        b.id = id;
        
        this->multibytetree::addLeaf( (char *)&b, sizeof( b ), (void *)o );
    }
    
    //remove leaf
    void model_component_tree_bytypeid::removeLeaf( model_component *o )
    {
        this->multibytetree::removeLeaf( (void *)o );
    }
    
};
