
#include "model_loader_ms3d_state_make_materials.h"
#include "model_loader_ms3d_state_make_groups.h"
#include "../../../../core/dpbuffer/dpbuffer.h"
#include "../model_loader_writelock.h"
#include "model_loader_ms3d.h"
#include "../../model_ref.h"
#include "../../model_writelock.h"
#include "../../../../core/shared_obj/shared_obj_guard.h"
#include "model_loader_ms3d_state_cleanup.h"
#include "../../model_material/model_material.h"
#include "../../../../core/dpthread/dpthread_lock.h"

#include <iostream>

namespace dragonpoop
{
    
    //ctor
    model_loader_ms3d_state_make_materials::model_loader_ms3d_state_make_materials( dpbuffer *b, model_ref *m )
    {
        this->b = b;
        this->m = m;
    }
    
    //dtor
    model_loader_ms3d_state_make_materials::~model_loader_ms3d_state_make_materials( void )
    {
        
    }
    
    //run state, returns next state
    model_loader_state *model_loader_ms3d_state_make_materials::run( dpthread_lock *thd, model_loader_writelock *ml )
    {
        shared_obj_guard o;
        model_writelock *m;
        unsigned int i, e;
        model_loader_ms3d *t;
        ms3d_model_material_m *v;
        std::vector<ms3d_model_material_m> *l;
        
        m = (model_writelock *)o.writeLock( this->m, "model_loader_ms3d_state_make_materials::run" );
        if( !m )
            return new model_loader_ms3d_state_cleanup( this->b, this->m, 0 );
        
        t = (model_loader_ms3d *)ml->getLoader();
        
        l = t->materials;
        e = (unsigned int)l->size();
        for( i = 0; i < e; i++ )
        {
            v = &( ( *l )[ i ] );
            this->makeMaterial( thd, ml, v, m );
        }
        o.unlock();
        
        return new model_loader_ms3d_state_make_groups( this->b, this->m );
    }
    
    
    //make triangle
    void model_loader_ms3d_state_make_materials::makeMaterial( dpthread_lock *thd, model_loader_writelock *ml, ms3d_model_material_m *t, model_writelock *m )
    {
        model_material *mv;
        std::string s;
        dprgba c;
        
        mv = m->makeMaterial( thd->genId() );
        if( !mv )
            return;
        t->id = mv->getId();
        s.assign( (char *)t->f.name, sizeof( t->f.name ) );
        mv->setName( &s );
        
        //ambient
        c.r = t->f.colors.ambient.r;
        c.g = t->f.colors.ambient.g;
        c.b = t->f.colors.ambient.b;
        c.a = t->f.colors.ambient.a;
        mv->setAmbientColor( &c );
        
        //diffuse
        c.r = t->f.colors.diffuse.r;
        c.g = t->f.colors.diffuse.g;
        c.b = t->f.colors.diffuse.b;
        c.a = t->f.colors.diffuse.a;
        mv->setDiffuseColor( &c );
        
        //specular
        c.r = t->f.colors.specular.r;
        c.g = t->f.colors.specular.g;
        c.b = t->f.colors.specular.b;
        c.a = t->f.colors.specular.a;
        mv->setSpecularColor( &c );
        
        //emissive
        c.r = t->f.colors.emissive.r;
        c.g = t->f.colors.emissive.g;
        c.b = t->f.colors.emissive.b;
        c.a = t->f.colors.emissive.a;
        mv->setEmissiveColor( &c );
        
        mv->setOpacity( t->f.transparency );
        mv->setShine( t->f.shininess );
        
        s.assign( (char *)t->f.tex_filename, sizeof( t->f.tex_filename ) );
        //s.assign( "beast1.jpg" );
        //s.assign( "3drt_dragon_original.bmp" );
        s.assign( "3drt_felhound_high.bmp" );
        
        if( s.size() > 0 )
            this->loadBitmap( mv->getDiffuseTexture(), &s );

        s.assign( (char *)t->f.alpha_filename, sizeof( t->f.alpha_filename ) );
        if( s.size() > 0 )
            this->loadBitmap( mv->getAlphaMaskTexture(), &s );
    }
    
    //load bitmap
    bool model_loader_ms3d_state_make_materials::loadBitmap( dpbitmap *bm, std::string *fname )
    {
        bm->reset();
        return bm->loadFile( fname->c_str() );
    }
    
};
