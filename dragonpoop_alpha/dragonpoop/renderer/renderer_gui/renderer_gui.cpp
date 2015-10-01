
#include "renderer_gui.h"
#include "renderer_gui_readlock.h"
#include "renderer_gui_writelock.h"
#include "renderer_gui_ref.h"
#include "../../gfx/gui/gui_writelock.h"
#include "../../gfx/gui/gui_readlock.h"
#include "../../gfx/gui/gui_ref.h"
#include "../../core/core.h"
#include "../renderer_writelock.h"
#include "../../core/shared_obj/shared_obj_guard.h"
#include "../../core/dpthread/dpthread_lock.h"

#include <thread>

namespace dragonpoop
{
    
    //ctor
    renderer_gui::renderer_gui( gui_writelock *g ) : shared_obj( g->getCore()->getMutexMaster() )
    {
        this->g = (gui_ref *)g->getRef();
        this->c = g->getCore();
        this->id = g->getId();
        this->pid = g->getParentId();
        g->getDimensions( &this->pos );
        this->bHasBg = g->hasBg();
        this->bHasFg = g->hasFg();
        this->bSyncBg = this->bSyncFg = 1;
        this->bSyncPos = 0;
        this->z = g->getZ();
        this->hv = 0;
        this->opacity = 1;
        this->fade = 0;
        this->bIsAlive = 1;
    }
    
    //dtor
    renderer_gui::~renderer_gui( void )
    {
        delete g;
    }
    
    //return core
    core *renderer_gui::getCore( void )
    {
        return this->c;
    }
    
    //compares parent id
    bool renderer_gui::compareParentId( dpid id )
    {
        return dpid_compare( &id, &this->pid );
    }
    
    //generate read lock
    shared_obj_readlock *renderer_gui::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new renderer_gui_readlock( (renderer_gui *)p, l );
    }
    
    //generate write lock
    shared_obj_writelock *renderer_gui::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new renderer_gui_writelock( (renderer_gui *)p, l );
    }
    
    //generate ref
    shared_obj_ref *renderer_gui::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new renderer_gui_ref( (renderer_gui *)p, k );
    }
    
    //run gui
    void renderer_gui::run( dpthread_lock *thd, renderer_gui_writelock *g )
    {
        gui_readlock *pl;
        shared_obj_guard o;
        dpbitmap *bm;
        uint64_t t;
        
        
        if( this->bSyncBg || this->bSyncFg || this->bSyncPos )
        {

            t = thd->getTicks();
            if( !this->bSyncPos && t - this->t_last_tex_update < 20 )
                return;
            this->t_last_tex_update = t;
            
            pl = (gui_readlock *)o.tryReadLock( this->g, 200, "renderer_gui::run" );
            if( pl )
            {
                
                if( this->bSyncPos )
                {
                    this->pid = pl->getParentId();
                    pl->getDimensions( &this->pos );
                    this->bHasBg = pl->hasBg();
                    this->bHasFg = pl->hasFg();
                    this->z = pl->getZ();
                    this->updateVb( g, pl, &this->pos );
                    this->bSyncPos = 0;
                }
                
                if( this->bHasBg && this->bSyncBg )
                {
                    bm = pl->getBg();
                    this->updateBg( g, pl, bm );
                    this->bSyncBg = 0;
                }
                
                if( this->bHasFg && this->bSyncFg )
                {
                    bm = pl->getFg();
                    this->updateFg( g, pl, bm );
                    this->bSyncFg = 0;
                }
                
            }
        }        
    }
    
    //render model
    void renderer_gui::render( dpthread_lock *thd, renderer_writelock *r, renderer_gui_readlock *m, dpmatrix *m_world )
    {
        std::list<renderer_gui *> l;
        std::list<renderer_gui *>::iterator i;
        renderer_gui *p;
        renderer_gui_readlock *pl;
        shared_obj_guard o;
        dpmatrix mat;
        
        r->getChildrenGuis( &l, this->id );
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            pl = (renderer_gui_readlock *)o.tryReadLock( p, 30, "renderer_gui::render" );
            if( !pl )
                continue;
            if( !pl->compareId( this->id ) )
                pl->render( thd, r, m_world );
        }
        
        mat.copy( m_world );
        mat.multiply( &this->mat );
        
        r->renderGui( thd, m, &mat );
    }
    
    //returns id
    dpid renderer_gui::getId( void )
    {
        return this->id;
    }
    
    //compares id
    bool renderer_gui::compareId( dpid id )
    {
        return dpid_compare( &id, &this->id );
    }
    
    //get dimensions
    void renderer_gui::getDimensions( gui_dims *p )
    {
        *p = this->pos;
    }
    
    //returns true if has background texture
    bool renderer_gui::hasBg( void )
    {
        return this->bHasBg;
    }
    
    //returns true if has forground texture
    bool renderer_gui::hasFg( void )
    {
        return this->bHasFg;
    }
    
    //get parent id
    dpid renderer_gui::getParentId( void )
    {
        return this->pid;
    }
    
    //override to handle bg texture update
    void renderer_gui::updateBg( renderer_gui_writelock *rl, gui_readlock *gl, dpbitmap *bm )
    {
        
    }
    
    //override to handle fg texture update
    void renderer_gui::updateFg( renderer_gui_writelock *rl, gui_readlock *gl, dpbitmap *bm )
    {
        
    }
    
    //makes a box in vb
    void makeBox( dpvertexindex_buffer *vb, float x, float y, float w, float h, float tx, float ty, float tw, float th )
    {
        dpvertex b;

        b.normal.x = 0;
        b.normal.y = 0;
        b.normal.z = 1;
        b.pos.z = -1;
        
        //tri_0
        
        //tl
        b.pos.x = x;
        b.pos.y = y;
        b.texcoords[ 0 ].s = tx;
        b.texcoords[ 0 ].t = ty;
        vb->addVertexUnique( &b );
        
        //bl
        b.pos.x = x;
        b.pos.y = y + h;
        b.texcoords[ 0 ].s = tx;
        b.texcoords[ 0 ].t = ty + th;
        vb->addVertexUnique( &b );
        
        //tr
        b.pos.x = x + w;
        b.pos.y = y;
        b.texcoords[ 0 ].s = tx + tw;
        b.texcoords[ 0 ].t = ty;
        vb->addVertexUnique( &b );
        
        //tri_1
        
        //tr
        b.pos.x = x + w;
        b.pos.y = y;
        b.texcoords[ 0 ].s = tx + tw;
        b.texcoords[ 0 ].t = ty;
        vb->addVertexUnique( &b );
        
        //bl
        b.pos.x = x;
        b.pos.y = y + h;
        b.texcoords[ 0 ].s = tx;
        b.texcoords[ 0 ].t = ty + th;
        vb->addVertexUnique( &b );
        
        //br
        b.pos.x = x + w;
        b.pos.y = y + h;
        b.texcoords[ 0 ].s = tx + tw;
        b.texcoords[ 0 ].t = ty + th;
        vb->addVertexUnique( &b );
    }
    
    //override to handle vb update
    void renderer_gui::updateVb( renderer_gui_writelock *rl, gui_readlock *gl, gui_dims *p )
    {
        dpvertexindex_buffer *vb;
        float w, h, bw, tw;
        
        bw = p->border_w;
        tw = p->border_tex_w;
        w = p->w;
        h = p->h;

        //fg
        vb = &this->fg_vb;
        vb->clear();
        makeBox( vb, 0, 0, w, h, 0, 0, 1, 1 );
        
        //bg
        vb = &this->bg_vb;
        vb->clear();

        //tl
        makeBox( vb, 0,         0,          bw,             bw,             0,          0,          tw, tw );
        //top
        makeBox( vb, bw,        0,          w - bw - bw,    bw,             tw,         0,          1 - tw - tw, tw );
        //tr
        makeBox( vb, w - bw,    0,          bw,             bw,             1 - tw,     0,          tw, tw );
        //left
        makeBox( vb, 0,         bw,         bw,             h - bw - bw,    0,          tw,         tw, 1 - tw - tw );
        //center
        makeBox( vb, bw,        bw,         w - bw - bw,    h - bw - bw,    tw,         tw,         1 - tw - tw, 1 - tw - tw );
        //right
        makeBox( vb, w - bw,    bw,         bw,             h - bw - bw,    1 - tw,     tw,         tw, 1 - tw - tw );
        //bl
        makeBox( vb, 0,         h - bw,     bw,             bw,             0,          1 - tw,     tw, tw );
        //bottom
        makeBox( vb, bw,        h - bw,     w - bw - bw,    bw,             tw,         1 - tw,     1 - tw - tw, tw );
        //br
        makeBox( vb, w - bw,    h - bw,     bw,             bw,             1 - tw,     1 - tw,     tw, tw );
    }
    
    //called to force pos update
    void renderer_gui::syncPos( void )
    {
        this->bSyncPos = 1;
    }
    
    //called to force bg update
    void renderer_gui::syncBg( void )
    {
        this->bSyncBg = 1;
    }
    
    //called to force fg update
    void renderer_gui::syncFg( void )
    {
        this->bSyncFg = 1;
    }
    
    //redo matrix
    void renderer_gui::redoMatrix( dpthread_lock *thd, renderer_writelock *r, renderer_gui_writelock *m, dpmatrix *p_matrix )
    {
        std::list<renderer_gui *> l;
        std::list<renderer_gui *>::iterator i;
        renderer_gui *p;
        renderer_gui_writelock *pl;
        shared_obj_guard o;
        float z;
        dpid hid;
        
        if( this->bIsAlive )
            this->fade += ( 1.0f - this->fade ) * 0.3f;
        if( !this->bIsAlive )
            this->fade += ( 0.0f - this->fade ) * 0.3f;
        
        hid = r->getHoverId();
        if( this->compareId( hid ) )
            this->hv += ( GUI_HOVER_MAX - this->hv ) * 0.3f;
        else
            this->hv += ( 0.0f - this->hv ) * 0.3f;
        
        this->mat.copy( p_matrix );

        z = (float)this->z / -8.0f;
        this->mat.translate( this->pos.x, this->pos.y, z );

        z = 1.0f - this->fade * this->fade;
        z += 0.05f * this->hv / GUI_HOVER_MAX;
        if( z > 0.001f )
        {
            this->mat.translate( -this->pos.w * 0.5f * z, -this->pos.h * 0.5f * z, 0 );
            this->mat.scale( z + 1, z + 1, 1 );
        }
        
        z = 1.0f - this->fade;
        if( z > 0.01f )
        {
            this->mat.translate( -this->pos.w * 0.5f, -this->pos.h * 0.5f, -z * 2 );
            this->mat.rotateY( 5.0f * z );
            this->mat.rotateZ( 10.0f * z );
            this->mat.translate( this->pos.w * 0.5f, this->pos.h * 0.5f, 0 );
        }

        this->undo_mat.inverse( &this->mat );
        
        r->getChildrenGuis( &l, this->id );
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            pl = (renderer_gui_writelock *)o.tryWriteLock( p, 100, "renderer_gui::redoMatrix" );
            if( !pl )
                continue;
            pl->redoMatrix( thd, r, &this->mat );
        }
    }
    
    //returns z order
    unsigned int renderer_gui::getZ( void )
    {
        return this->z;
    }
    
    //process mouse input
    bool renderer_gui::processMouse( float x, float y, bool lb, bool rb )
    {
        dpxyz_f p;
        gui_writelock *g;
        shared_obj_guard o;
        
        p.x = x;
        p.y = y;
        p.z = (float)this->z / -8.0f;
        this->undo_mat.transform( &p );
        
        if( p.x < 0 || p.y < 0 )
            return 0;
        if( p.x >= this->pos.w || p.y >= this->pos.h )
            return 0;
        
        g = (gui_writelock *)o.tryWriteLock( this->g, 300, "renderer_gui::processMouse" );
        if( !g )
            return 1;
        g->processMouse( p.x, p.y, lb, rb );

        return 1;
    }
    
    //process kb input
    bool renderer_gui::processKb( std::string *sname, bool bIsDown )
    {
        gui_writelock *g;
        shared_obj_guard o;

        g = (gui_writelock *)o.tryWriteLock( this->g, 300, "renderer_gui::processKb" );
        if( !g )
            return 1;
        
        g->processKb( sname, bIsDown );
        return 1;
    }
    
    //returns opacity
    float renderer_gui::getOpacity( void )
    {
        return this->fade * this->opacity * ( 0.9f + 0.1f * this->hv / GUI_HOVER_MAX );
    }
    
    //returns true if alive
    bool renderer_gui::isAlive( void )
    {
        if( this->bIsAlive )
            return 1;
        return this->fade > 0.1f;
    }
    
    //kills gui
    void renderer_gui::kill( void )
    {
        this->bIsAlive = 0;
    }
    
    //returns true if has focus
    bool renderer_gui::hasFocus( void )
    {
        return this->z == 0;
    }
    
    //gets gui id of focused child
    bool renderer_gui::getFocusChild( renderer_writelock *r, dpid *fid )
    {
        std::list<renderer_gui *> l;
        std::list<renderer_gui *>::iterator i;
        renderer_gui *p;
        renderer_gui_writelock *pl;
        shared_obj_guard o;

        r->getChildrenGuis( &l, this->id );
        for( i = l.begin(); i != l.end(); ++i )
        {
            p = *i;
            if( p->getZ() != 0 )
                continue;
            pl = (renderer_gui_writelock *)o.tryWriteLock( p, 100, "renderer_gui::getFocusChild" );
            if( !pl )
                continue;
            if( pl->getFocusChild( r, fid ) )
                return 1;
            *fid = pl->getId();
            return 1;
        }
        
        return 0;
    }
    
    //return bg vb
    dpvertexindex_buffer *renderer_gui::getBgBuffer( void )
    {
        return &this->bg_vb;
    }
    
    //return fg vb
    dpvertexindex_buffer *renderer_gui::getFgBuffer( void )
    {
        return &this->fg_vb;
    }
    
};
