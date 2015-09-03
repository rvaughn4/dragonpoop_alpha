
#include "model_instance_joint_cache.h"
#include "../model_instance_joint/model_instance_joint.h"
#include "../../../dpvertex/dpvertex.h"
#include <math.h>
#include "../../model_matrix/model_vector.h"
#include "../../model_matrix/model_quaternion.h"
#include "../../model_matrix/model_matrix.h"

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
    void model_instance_joint_cache::transform( dpvertex *v )
    {

        model_instance_joint_cache_element *e;
        e = this->getElement( v->bones[ 0 ].id );
        if( !e )
            return;
        e->up.transform( &v->pos );
        e->down.transform( &v->pos );
        
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
    
    //return count
    uint16_t model_instance_joint_cache::getCount( void )
    {
        return (uint16_t)this->cnt;
    }
    
    //compute matrixes
    void model_instance_joint_cache::updateMaticies( float r )
    {
        unsigned int i, e;
        model_instance_joint_cache_element *p;
        
        e = this->getCount();
        for( i = 0; i < e; i++ )
        {
            p = this->getElement( i );
            if( !p )
                continue;
            p->wasUpdated = 0;
        }

        for( i = 0; i < e; i++ )
        {
            p = this->getElement( i );
            if( !p )
                continue;
            this->computeMatrix( p, r );
        }
    }
  
    //compute matrix for element, computing all parents first
    void model_instance_joint_cache::computeMatrix( model_instance_joint_cache_element *e, float r )
    {
        float rs, re;
        
        re = r;
        if( re > 1 )
            re = 1;
        if( re < 0 )
            re = 0;
        rs = 1.0f - re;
        
        e->up.setIdentity();
        this->doUpMatrix( e, &e->up, rs, re );
        
        e->down.setIdentity();
        this->doDownMatrix( e, &e->down );
    }
    
    //do matrix up
    void model_instance_joint_cache::doUpMatrix( model_instance_joint_cache_element *e, dpmatrix *m, float rs, float re )
    {
        model_instance_joint_cache_element *p;
        dpxyz_f o, t;
        
        p = this->getElement( e->pid );

        if( p )
            this->doUpMatrix( p, m, rs, re );
        
        //bone up
        m->translate( -e->bone_pos.x, -e->bone_pos.y, -e->bone_pos.z );
      //  m->rotateXrad( -e->bone_rot.x );
       // m->rotateYrad( -e->bone_rot.y );
       // m->rotateZrad( -e->bone_rot.z );
        
        //animation
        this->angleLerp( &e->rot_start, &e->rot_end, &o, rs, re );
        t.x = e->pos_start.x * rs + e->pos_end.x * re;
        t.y = e->pos_start.y * rs + e->pos_end.y * re;
        t.z = e->pos_start.z * rs + e->pos_end.z * re;
        
        m->rotateZrad( o.z );
        m->rotateYrad( o.y );
        m->rotateXrad( o.x );
        m->translate( t.x, t.y, t.z );
    }
    
    //do matrix down
    void model_instance_joint_cache::doDownMatrix( model_instance_joint_cache_element *e, dpmatrix *m )
    {
        model_instance_joint_cache_element *p;

        p = this->getElement( e->pid );

       // m->rotateZrad( e->bone_rot.z );
       // m->rotateYrad( e->bone_rot.y );
//m->rotateXrad( e->bone_rot.x );
        m->translate( e->bone_pos.x, e->bone_pos.y, e->bone_pos.z );

        if( p )
            this->doDownMatrix( p, m );
    }
    
    //do angle lerp
    void model_instance_joint_cache::angleLerp( dpxyz_f *a, dpxyz_f *b, dpxyz_f *o, float rs, float re )
    {
        model_quaternion q, qa, qb;
        model_matrix mx;
        model_vector v;
        
        qa.setAngle( a );
        qb.setAngle( b );
        q.slerp( &qa, &qb, re );
        
        mx.setQuat( &q );
        v.setIdentity();
        mx.setPosition( &v );

        mx.getAngles( o );
    }
    
};
