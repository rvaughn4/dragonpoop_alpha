
#ifndef dragonpoop_model_component_tree_bytypeid_h
#define dragonpoop_model_component_tree_bytypeid_h

#include "../../../core/bytetree/multibytetree.h"
#include "../../../core/dpid/dpid.h"

namespace dragonpoop
{
    
    class model_component;
    
#pragma pack( 1 )
    struct model_component_tree_bytypeid_block
    {
        uint16_t mtype;
        dpid id;
    };
#pragma pack( )
    
    class model_component_tree_bytypeid : public multibytetree
    {
    private:
        
    protected:
        
    public:
        
        //ctor
        model_component_tree_bytypeid( void );
        //dtor
        virtual ~model_component_tree_bytypeid( void );
        //find leaves
        void findLeaves( uint16_t mtype, dpid id, std::list<model_component *> *l );
        //find leaf
        model_component *findLeaf( uint16_t mtype, dpid id );
        //add leaf
        void addLeaf( uint16_t mtype, dpid id, model_component *o );
        //remove leaf
        void removeLeaf( model_component *o );
        
    };
    
};

#endif