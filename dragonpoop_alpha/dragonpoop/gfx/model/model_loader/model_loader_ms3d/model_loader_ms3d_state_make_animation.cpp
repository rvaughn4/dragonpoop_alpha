
#include "model_loader_ms3d_state_make_animation.h"
#include "model_loader_ms3d_state_make_frames.h"
#include "../../../../core/dpbuffer/dpbuffer.h"
#include "model_loader_ms3d_animation.h"
#include "model_loader_ms3d_state_cleanup.h"
#include "model_loader_ms3d.h"
#include "../model_loader_writelock.h"
#include "../../../../core/shared_obj/shared_obj_guard.h"
#include "../../model_animation/model_animation.h"
#include "../../model_ref.h"
#include "../../model_writelock.h"
#include "../../../../core/dpthread/dpthread_lock.h"
#include <fstream>

namespace dragonpoop
{
    
    //ctor
    model_loader_ms3d_state_make_animation::model_loader_ms3d_state_make_animation( dpbuffer *b, model_ref *m )
    {
        this->b = b;
        this->m = m;
    }
    
    //dtor
    model_loader_ms3d_state_make_animation::~model_loader_ms3d_state_make_animation( void )
    {
        
    }
    
    //run state, returns next state
    model_loader_state *model_loader_ms3d_state_make_animation::run( dpthread_lock *thd, model_loader_writelock *ml )
    {
        shared_obj_guard o;
        model_writelock *m;
        model_animation *ma;
        model_loader_ms3d *t;
        std::string sname;
        
        m = (model_writelock *)o.writeLock( this->m, "model_loader_ms3d_state_make_animation::run" );
        if( !m )
            return new model_loader_ms3d_state_cleanup( this->b, this->m, 0 );
        
        t = (model_loader_ms3d *)ml->getLoader();
        
        ma = m->makeAnimation( thd->genId() );
        if( !ma )
            return new model_loader_ms3d_state_cleanup( this->b, this->m, 0 );
        ma->setFps( t->anim.fps );
        ma->setLength( t->anim.cnt_frames );
        t->anim_id = ma->getId();
        
        sname.assign( "ms3d_all" );
        ma->setName( &sname );
        
        this->readAnimations( thd, ml );
        
        return new model_loader_ms3d_state_make_frames( this->b, this->m );
    }
   
    //read text file and create animations
    void model_loader_ms3d_state_make_animation::readAnimations( dpthread_lock *thd, model_loader_writelock *ml )
    {
        std::fstream f;
        model_loader_ms3d *ldr;
        model_writelock *m;
        shared_obj_guard o;
        std::string ss, ds;
        unsigned int s, e;
        model_animation *a;
        
        m = (model_writelock *)o.tryWriteLock( this->m, 1000, "model_loader_ms3d_state_make_frames::makeAnimationFrames" );
        if( !m )
            return;
        ldr = (model_loader_ms3d *)ml->getLoader();
        
        ss.append( ldr->fname );
        ss.append( ".animations.list.txt" );
        
        f.open( ss.c_str(), f.in | f.binary );
        if( !f.is_open() )
            return;
        
        while( !f.eof() )
        {
            f >> s;
            f >> ds;
            f >> e;
            std::getline( f, ss );
            ss = trim_copy( ss );
            
            a = m->makeAnimation( thd->genId() );
            if( !a )
                continue;
            a->setName( &ss );
            a->setLength( e - s );
            a->setFps( ldr->anim.fps );
        }
        
    }
    
};
