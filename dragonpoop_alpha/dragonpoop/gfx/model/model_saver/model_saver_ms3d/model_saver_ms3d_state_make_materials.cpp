
#include "model_saver_ms3d_state_make_materials.h"
#include "model_saver_ms3d_state_make_groups.h"
#include "../../../../core/dpbuffer/dpbuffer.h"
#include "model_saver_ms3d_material.h"
#include "model_saver_ms3d_state_cleanup.h"
#include "model_saver_ms3d.h"
#include "../model_saver_writelock.h"
#include "../../../../core/shared_obj/shared_obj_guard.h"
#include "../../model_material/model_material.h"
#include "../../model_ref.h"
#include "../../model_readlock.h"
#include "../../../../core/dpthread/dpthread_lock.h"

namespace dragonpoop
{
    
    //ctor
    model_saver_ms3d_state_make_materials::model_saver_ms3d_state_make_materials( dpbuffer *b, model_ref *m )
    {
        this->b = b;
        this->m = m;
    }
    
    //dtor
    model_saver_ms3d_state_make_materials::~model_saver_ms3d_state_make_materials( void )
    {
        
    }
    
    //run state, returns next state
    model_saver_state *model_saver_ms3d_state_make_materials::run( dpthread_lock *thd, model_saver_writelock *ml )
    {
        shared_obj_guard o;
        model_readlock *m;
        model_saver_ms3d *t;
        std::list<model_material *> l;
        std::list<model_material *>::iterator i;
        model_material *p;
        std::vector<ms3d_model_material_m> *lf;
        
        m = (model_readlock *)o.readLock( this->m );
        if( !m )
            return new model_saver_ms3d_state_cleanup( this->b, this->m, 0 );
        t = (model_saver_ms3d *)ml->getSaver();
        
        lf = t->materials;
        if( lf )
            delete lf;
        lf = new std::vector<ms3d_model_material_m>();
        t->materials = lf;
        
        m->getMaterials( &l );
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            this->makeMaterial( t, m, p );
        }
        
        return new model_saver_ms3d_state_make_groups( this->b, this->m );
    }
    
    //makes joints for animation
    void model_saver_ms3d_state_make_materials::makeMaterial( model_saver_ms3d *t, model_readlock *m, model_material *p )
    {
        std::vector<ms3d_model_material_m> *lf;
        ms3d_model_material_m v;
        std::string s;
        dprgba x;

        lf = t->materials;
        
        v.id = p->getId();
        v.f.mode = 0;
        v.f.transparency = p->getOpacity();
        v.f.shininess = p->getShine();
        
        //diffuse
        p->getDiffuseColor( &x );
        v.f.colors.diffuse.r = x.r;
        v.f.colors.diffuse.g = x.g;
        v.f.colors.diffuse.b = x.b;
        v.f.colors.diffuse.a = x.a;
        
        //specular
        p->getSpecularColor( &x );
        v.f.colors.specular.r = x.r;
        v.f.colors.specular.g = x.g;
        v.f.colors.specular.b = x.b;
        v.f.colors.specular.a = x.a;

        //emissive
        p->getEmissiveColor( &x );
        v.f.colors.emissive.r = x.r;
        v.f.colors.emissive.g = x.g;
        v.f.colors.emissive.b = x.b;
        v.f.colors.emissive.a = x.a;
        
        //ambient
        p->getAmbientColor( &x );
        v.f.colors.ambient.r = x.r;
        v.f.colors.ambient.g = x.g;
        v.f.colors.ambient.b = x.b;
        v.f.colors.ambient.a = x.a;
        
        memset( v.f.name, 0, sizeof( v.f.name ) );
        p->getName( &s );
        s.copy( (char *)v.f.name, sizeof( v.f.name ) );

        
        lf->push_back( v );
    }
    
};
