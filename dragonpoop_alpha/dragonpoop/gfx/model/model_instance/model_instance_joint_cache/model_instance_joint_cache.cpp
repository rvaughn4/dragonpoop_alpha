
#include "model_instance_joint_cache.h"
#include "../model_instance_joint/model_instance_joint.h"
#include "../../../dpvertex/dpvertex.h"

namespace dragonpoop
{
    
    //ctor
    model_instance_joint_cache::model_instance_joint_cache( void )
    {
        this->size = 0;
        this->cnt = 0;
        this->buffer = 0;
    }
    
    //dtor
    model_instance_joint_cache::~model_instance_joint_cache( void )
    {
        model_instance_joint_cache_element *b;
        
        b = this->buffer;
        if( b )
            delete[] b;
    }
    
    //clear
    void model_instance_joint_cache::clear( void )
    {
        model_instance_joint_cache_element *b;
        unsigned int i, e;
        
        this->cnt = 0;
        if( !b )
            return;
        
        e = this->size;
        for( i = 0; i < e; i++ )
            b[ i ].id = -1;
    }
    
    //add joint
    void model_instance_joint_cache::addJoint( model_instance_joint *j )
    {
        model_instance_joint_cache_element *e;
        int16_t i;
        
        i = j->getIndex();
        if( i < 0 )
            return;
        this->autoResize( (unsigned int)i );
        e = this->getElement( i );
        if( !e )
            return;
        e->id = i;
        ////
    }
    
    //transform vertex
    void model_instance_joint_cache::transform( dpvertex *v, float ratio )
    {
        
    }
    
    //auto resize
    void model_instance_joint_cache::autoResize( unsigned int ncnt )
    {
        model_instance_joint_cache_element *b, *nb;
        unsigned int ns, i, e;

        b = this->buffer;
        if( b && ncnt < this->cnt && ncnt < this->size )
            return;
        
        if( b && ncnt < this->size )
        {
            this->cnt = ncnt + 1;
            return;
        }
        
        ns = this->size * 2 + 2;
        if( ns <= ncnt )
            ns = ncnt + 2;
        
        nb = new model_instance_joint_cache_element[ ns ];
        if( !nb )
            return;
        e = ns;
        for( i = 0; i < e; i++ )
            nb[ i ].id = -1;
        if( b )
        {
            e = this->size;
            for( i = 0; i < e; i++ )
                nb[ i ] = b[ i ];
            delete[] b;
        }
        this->buffer = nb;
        this->size = ns;
        this->cnt = ncnt;
    }
    
    //get pointer to location
    model_instance_joint_cache_element *model_instance_joint_cache::getElement( int16_t i )
    {
        model_instance_joint_cache_element *b;
        
        b = this->buffer;
        if( !b || i < 0 || i >= this->size )
            return 0;
        
        return &b[ (unsigned int)i ];
    }
    
};
