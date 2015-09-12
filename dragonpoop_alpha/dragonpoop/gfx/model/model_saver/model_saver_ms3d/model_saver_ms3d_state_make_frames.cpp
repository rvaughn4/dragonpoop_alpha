
#include "model_saver_ms3d_state_make_frames.h"
#include "model_saver_ms3d_state_make_joints.h"
#include "../../../../core/dpbuffer/dpbuffer.h"
#include "model_saver_ms3d_animation.h"
#include "model_saver_ms3d_state_cleanup.h"
#include "model_saver_ms3d.h"
#include "../model_saver_writelock.h"
#include "../../../../core/shared_obj/shared_obj_guard.h"
#include "../../model_animation/model_animation.h"
#include "../../model_ref.h"
#include "../../model_readlock.h"
#include "../../../../core/dpthread/dpthread_lock.h"
#include "../../model_animation_frame/model_animation_frame.h"

namespace dragonpoop
{
    
    //ctor
    model_saver_ms3d_state_make_frames::model_saver_ms3d_state_make_frames( dpbuffer *b, model_ref *m )
    {
        this->b = b;
        this->m = m;
    }
    
    //dtor
    model_saver_ms3d_state_make_frames::~model_saver_ms3d_state_make_frames( void )
    {
        
    }
    
    //run state, returns next state
    model_saver_state *model_saver_ms3d_state_make_frames::run( dpthread_lock *thd, model_saver_writelock *ml )
    {
        shared_obj_guard o;
        model_readlock *m;
        ms3d_model_animation_m *ma;
        model_saver_ms3d *t;
        std::vector<ms3d_model_animation_m> *la;
        std::vector<ms3d_model_frame> *lf;
        unsigned int i, e;
        
        m = (model_readlock *)o.readLock( this->m );
        if( !m )
            return new model_saver_ms3d_state_cleanup( this->b, this->m, 0 );
        t = (model_saver_ms3d *)ml->getSaver();
        
        lf = t->frames;
        if( lf )
            delete lf;
        lf = new std::vector<ms3d_model_frame>();
        t->frames = lf;
        
        la = t->anims;
        if( !la )
            return new model_saver_ms3d_state_cleanup( this->b, this->m, 0 );
        
        e = (unsigned int)la->size();
        for( i = 0; i < e; i++ )
        {
            ma = &( *la )[ i ];
            this->makeFrames( t, ma, m );
        }
        
        o.unlock();

        return new model_saver_ms3d_state_make_joints( this->b, this->m );
    }

    //makes frames for animation
    void model_saver_ms3d_state_make_frames::makeFrames( model_saver_ms3d *t, ms3d_model_animation_m *a, model_readlock *m )
    {
        std::list<model_animation_frame *> l;
        std::list<model_animation_frame *>::iterator i;
        model_animation_frame *p;
        ms3d_model_frame f;
        std::vector<ms3d_model_frame> *lf;
        std::vector<ms3d_model_frame> llf;
        std::vector<float> lt;
        unsigned int j, k, r, d;
        
        lf = t->frames;
        m->getAnimationFrames( &l, a->id );
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;

            f.t = p->getTime();
            f.id = p->getFrameId();
            f.afid = p->getId();
            
            lt.push_back( f.t );
            llf.push_back( f );
        }
        
        std::sort( lt.begin(), lt.end() );
        
        k = (unsigned int)lt.size();
        d = (unsigned int)llf.size();
        for( j = 0; j < k; j++ )
        {
            for( r = 0; r < d; r++ )
            {
                if( llf[ r ].t == lt[ j ] )
                {
                    lf->push_back( llf[ r ] );
                    r = d;
                }
            }
        }
    
    }
    
};
