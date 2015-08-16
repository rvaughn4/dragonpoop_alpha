
#include "model_loader_ms3d_state_parse_materials.h"
#include "model_loader_ms3d_state_parse_animation.h"
#include "../../../../core/dpbuffer/dpbuffer.h"
#include "model_loader_ms3d_material.h"
#include "model_loader_ms3d_state_cleanup.h"
#include "model_loader_ms3d.h"
#include "../model_loader_writelock.h"
#include <string.h>

namespace dragonpoop
{

    //ctor
    model_loader_ms3d_state_parse_materials::model_loader_ms3d_state_parse_materials( dpbuffer *b, model_ref *m )
    {
        this->b = b;
        this->m = m;
    }

    //dtor
    model_loader_ms3d_state_parse_materials::~model_loader_ms3d_state_parse_materials( void )
    {

    }

    //run state, returns next state
    model_loader_state *model_loader_ms3d_state_parse_materials::run( dpthread_lock *thd, model_loader_writelock *ml )
    {

        ms3d_model_material_section sh;
        ms3d_model_material_m h;
        unsigned int i;
        model_loader_ms3d *m;
        std::vector<ms3d_model_material_m> *v;

        m = (model_loader_ms3d *)ml->getLoader();

        v = m->materials;
        if( v )
            delete v;
        v = new std::vector<ms3d_model_material_m>();
        m->materials = v;

        if( this->b->readBytes( (uint8_t *)&sh, sizeof( sh ) ) < sizeof( sh ) )
            return new model_loader_ms3d_state_cleanup( this->b, this->m, 0 );

        memset( &h, 0, sizeof( h ) );
        v->clear();

        for( i = 0; i < sh.cnt; i++ )
        {
            if( this->b->readBytes( (uint8_t *)&h.f, sizeof( h.f ) ) < sizeof( h.f ) )
                return new model_loader_ms3d_state_cleanup( this->b, this->m, 0 );
            v->push_back( h );
        }

        return new model_loader_ms3d_state_parse_animation( this->b, this->m );
    }

};
