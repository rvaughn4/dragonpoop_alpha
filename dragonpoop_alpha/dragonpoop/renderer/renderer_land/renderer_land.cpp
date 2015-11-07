
#include "renderer_land.h"
#include "../api_stuff/render_api/render_api_indexbuffer_ref.h"
#include "../api_stuff/render_api/render_api_vertexbuffer_ref.h"
#include "../api_stuff/render_api/render_api_context_writelock.h"
#include "../api_stuff/render_api/render_api_commandlist_writelock.h"
#include "../../gfx/dpmatrix/dpmatrix.h"
#include "../../gfx/dpland/dpland.h"
#include "../../core/dpthread/dpthread_lock.h"

namespace dragonpoop
{

    //ctor
    renderer_land::renderer_land( dpland *l, render_api_context_writelock *ctx )
    {
        this->id = l->getId();
        l->getPosition( &this->pos );
        this->ib = ctx->makeIndexBuffer( l->getIB() );
        this->vb = ctx->makeVertexBuffer( l->getVB() );
    }
    
    //dtor
    renderer_land::~renderer_land( void )
    {
        delete this->ib;
        delete this->vb;
    }
    
    //get position
    void renderer_land::getPosition( dpposition *p )
    {
        p->copy( &this->pos );
    }
    
    //returns id
    dpid renderer_land::getId( void )
    {
        return this->id;
    }
    
    //render model
    void renderer_land::render( dpthread_lock *thd, dpposition *campos, dpmatrix *m_world, render_api_context_writelock *ctx, render_api_commandlist_writelock *clist )
    {
        dpmatrix ml, m;
        dpxyz_f d;
        
        campos->getDifference( &this->pos, thd->getTicks(), &d );
        ml.setTranslation( d.x, d.y, d.z );
        m.copy( m_world );
        m.multiply( &ml );
        
        clist->setAlpha( 1 );
        clist->setMatrix( &m );
        clist->setTexture( 0, 0 );
        clist->setTexture( 0, 1 );
        clist->setIndexBuffer( this->ib );
        clist->setVertexBuffer( this->vb );
        clist->draw();
    }

};
