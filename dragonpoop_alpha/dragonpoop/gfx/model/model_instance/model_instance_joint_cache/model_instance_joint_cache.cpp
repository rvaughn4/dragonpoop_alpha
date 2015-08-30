
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
        j->fillCacheEntity( e );
    }
    
    //transform vertex
    void model_instance_joint_cache::transform( dpvertex *v, float ratio )
    {
        dpxyz_f outps, outns, outpe, outne;
        float s_ratio;
        
        this->transform( &v->pos, &outps, &outpe, v->bones[ 0 ].id );
        this->transform( &v->normal, &outns, &outne, v->bones[ 0 ].id );
        
        if( ratio > 1 )
            ratio = 1;
        s_ratio = 1.0f - ratio;

        v->pos.x = outps.x * s_ratio + outpe.x * ratio;
        v->pos.y = outps.y * s_ratio + outpe.y * ratio;
        v->pos.z = outps.z * s_ratio + outpe.z * ratio;
        
        v->normal.x = outns.x * s_ratio + outne.x * ratio;
        v->normal.y = outns.y * s_ratio + outne.y * ratio;
        v->normal.z = outns.z * s_ratio + outne.z * ratio;
        
        /*
        dpxyz_f *pos_in, pos_a_start, pos_a_end, pos_out_start, pos_out_end;
        dpxyz_f *norm_in, norm_a_start, norm_a_end, norm_out_start, norm_out_end;
        float w, aw, s_ratio;
        unsigned int i, e;
        
        pos_in = &v->pos;
        norm_in = &v->normal;
        
        pos_a_start.x = pos_a_start.y = pos_a_start.z = 0;
        pos_a_end.x = pos_a_end.y = pos_a_end.z = 0;
        norm_a_start.x = norm_a_start.y = norm_a_start.z = 0;
        norm_a_end.x = norm_a_end.y = norm_a_end.z = 0;
        
        e = dpvertex_bones_size;
        for( i = 0; i < e; i++ )
        {
            if( v->bones[ i ].id < 0 )
            {
                i = e;
                continue;
            }
            w = v->bones[ i ].w;
            aw += w;
            this->transform( pos_in, &pos_out_start, &pos_out_end, v->bones[ i ].id );
            this->transform( norm_in, &norm_out_start, &norm_out_end, v->bones[ i ].id );
            
            pos_a_start.x += pos_out_start.x * w;
            pos_a_start.y += pos_out_start.y * w;
            pos_a_start.z += pos_out_start.z * w;

            pos_a_end.x += pos_out_end.x * w;
            pos_a_end.y += pos_out_end.y * w;
            pos_a_end.z += pos_out_end.z * w;
            
            norm_a_start.x += norm_out_start.x * w;
            norm_a_start.y += norm_out_start.y * w;
            norm_a_start.z += norm_out_start.z * w;
            
            norm_a_end.x += norm_out_end.x * w;
            norm_a_end.y += norm_out_end.y * w;
            norm_a_end.z += norm_out_end.z * w;
        }
        
        if( aw <= 0.0f )
            aw = 1.0f;
        s_ratio = 1.0f - ratio;
        
        
        pos_in->x = pos_a_start.x * s_ratio + pos_a_end.x * ratio;
        pos_in->y = pos_a_start.y * s_ratio + pos_a_end.y * ratio;
        pos_in->z = pos_a_start.z * s_ratio + pos_a_end.z * ratio;
        
        norm_in->x = norm_a_start.x * s_ratio + norm_a_end.x * ratio;
        norm_in->y = norm_a_start.y * s_ratio + norm_a_end.y * ratio;
        norm_in->z = norm_a_start.z * s_ratio + norm_a_end.z * ratio;
        
        pos_in->x /= aw;
        pos_in->y /= aw;
        pos_in->z /= aw;
        
        norm_in->x /= aw;
        norm_in->y /= aw;
        norm_in->z /= aw;

        pos_in->x *= aw;
        pos_in->y *= aw;
        pos_in->z *= aw;
        
        norm_in->x *= aw;
        norm_in->y *= aw;
        norm_in->z *= aw;
         
         */
    }
    
    //transform vertex
    void model_instance_joint_cache::transform( dpxyz_f *in, dpxyz_f *out_start, dpxyz_f *out_end, int16_t i )
    {
        model_instance_joint_cache_element *e;
        
        e = this->getElement( i );
        if( !e )
            return;

        *out_start = *in;
        *out_end = *in;

        e->start.transform( out_start );
        e->end.transform( out_end );
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
