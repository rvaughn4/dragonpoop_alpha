
#include "model_loader_ms3d_state_parse_joints.h"
#include "model_loader_ms3d_state_parse_extras.h"
#include "../../../../core/dpbuffer/dpbuffer.h"
#include "model_loader_ms3d_joint.h"
#include "model_loader_ms3d_state_cleanup.h"
#include "model_loader_ms3d.h"
#include "../model_loader_writelock.h"
#include <string.h>

namespace dragonpoop
{

    //ctor
    model_loader_ms3d_state_parse_joints::model_loader_ms3d_state_parse_joints( dpbuffer *b, model_ref *m )
    {
        this->b = b;
        this->m = m;
    }

    //dtor
    model_loader_ms3d_state_parse_joints::~model_loader_ms3d_state_parse_joints( void )
    {

    }

    bool model_loader_ms3d_state_parse_joints__kf( dpbuffer *b, unsigned int max, std::vector<ms3d_model_joint_keyframe> *l );

    //run state, returns next state
    model_loader_state *model_loader_ms3d_state_parse_joints::run( dpthread_lock *thd, model_loader_writelock *ml )
    {

        ms3d_model_joint_section sh;
        ms3d_model_joint_m h, *p;
        unsigned int i;
        model_loader_ms3d *m;
        std::vector<ms3d_model_joint_m> *v;

        m = (model_loader_ms3d *)ml->getLoader();

        v = m->joints;
        if( v )
            delete v;
        v = new std::vector<ms3d_model_joint_m>();
        m->joints = v;

        if( this->b->readBytes( (uint8_t *)&sh, sizeof( sh ) ) < sizeof( sh ) )
            return new model_loader_ms3d_state_cleanup( this->b, this->m, 0 );

        memset( &h.f, 0, sizeof( h.f ) );
        v->clear();

        for( i = 0; i < sh.cnt; i++ )
        {
            if( this->b->readBytes( (uint8_t *)&h.f, sizeof( h.f ) ) < sizeof( h.f ) )
                return new model_loader_ms3d_state_cleanup( this->b, this->m, 0 );
            v->push_back( h );
            p = &( *v )[ i ];
            if( !model_loader_ms3d_state_parse_joints__kf( this->b, p->f.cnt_rot_frames, &p->rotate_frames ) )
                return new model_loader_ms3d_state_cleanup( this->b, this->m, 0 );
            if( !model_loader_ms3d_state_parse_joints__kf( this->b, p->f.cnt_pos_frames, &p->translate_frames ) )
                return new model_loader_ms3d_state_cleanup( this->b, this->m, 0 );
        }

        return new model_loader_ms3d_state_parse_extras( this->b, this->m );
    }

    bool model_loader_ms3d_state_parse_joints__kf( dpbuffer *b, unsigned int max, std::vector<ms3d_model_joint_keyframe> *l )
    {
        unsigned int i;
        ms3d_model_joint_keyframe k;
        
        for( i = 0; i < max; i++ )
        {
            if( !b->readBytes( (uint8_t *)&k, sizeof( k ) ) )
                return 0;
            l->push_back( k );
        }

        return 1;
    }

};
