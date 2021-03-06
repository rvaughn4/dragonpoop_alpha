
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
#include "renderer_gui_man_writelock.h"
#include "renderer_gui_man_readlock.h"
#include "../api_stuff/render_api/render_api_vertexbuffer_ref.h"
#include "../api_stuff/render_api/render_api_indexbuffer_ref.h"
#include "../api_stuff/render_api/render_api_texture_ref.h"
#include "../api_stuff/render_api/render_api_context_writelock.h"
#include "../api_stuff/render_api/render_api_commandlist_writelock.h"
#include "../../gfx/dpvertex/dpvertexindex_buffer.h"
#include "../../core/dpbtree/dpid_btree.h"

#include <string.h>
#include <iostream>

namespace dragonpoop
{

    //ctor
    renderer_gui::renderer_gui( gui_writelock *g ) : shared_obj( g->getCore()->getMutexMaster() )
    {
        this->g = (gui_ref *)g->getRef();
        this->c = g->getCore();
        this->id = g->getId();
        g->getDimensions( &this->pos );
        this->bHasBg = g->hasBg();
        this->bHasFg = g->hasFg();
        this->bIsHover = g->isHoverable();
        this->bIsEdit = g->isEditable();
        this->bSyncBg = this->bSyncFg = 1;
        this->bSyncBgRen = this->bSyncFgRen = 0;
        this->bSyncPos = 1;
        this->bSyncPosRen = 0;
        this->z = 1;
        this->hv = 0;
        this->opacity = 1;
        this->fade = 0;
        this->bIsAlive = 1;
        this->clickfade = 0;
        this->bIsFade = g->isFade();
        this->render_ib_bg = this->render_ib_fg = 0;
        this->render_tex_bg = this->render_tex_fg = 0;
        this->render_vb_bg = this->render_vb_fg = 0;
        this->focus_id = dpid_null();
        this->hover_id = this->focus_id;
        memset( &this->drag_pos, 0, sizeof( this->drag_pos ) );
        this->follow_cursor.bEnabled = g->followsCursor( &this->follow_cursor.x, &this->follow_cursor.y );
    }

    //dtor
    renderer_gui::~renderer_gui( void )
    {
        shared_obj_guard o;

        o.tryWriteLock( this, 1000, "renderer_gui::~renderer_gui" );
        o.unlock();
        this->unlink();

        o.tryWriteLock( this, 1000, "renderer_gui::~renderer_gui" );
        o.unlock();

        this->deleteChildren();
        gui_ref *g;
        g = this->g;
        delete g;
    }

    //return core
    core *renderer_gui::getCore( void )
    {
        return this->c;
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
    void renderer_gui::redoMatrix( dpthread_lock *thd, renderer_gui_man_writelock *r, dpmatrix *p_matrix )
    {
        std::list<renderer_gui *> *l;
        std::list<renderer_gui *>::iterator i;
        renderer_gui *p;
        renderer_gui_writelock *pl;
        shared_obj_guard o;
        float z;
        dpid hid;
        gui_ref *gr;

        gr = this->g;
        if( !gr->isLinked() )
            this->bIsAlive = 0;

        if( this->bIsAlive )
        {
            if( this->fade < 0.95f )
                this->fade += ( 1.0f - this->fade ) * 0.3f;
            else
                this->fade = 1.0f;
        }
        else
        {
            if( this->fade > 0.1f )
                this->fade *= 0.3f;
            else
                this->fade = 0;
        }

        if( this->bIsHover )
        {
            if( this->bIsDrag )
                this->hv += ( GUI_HOVER_MAX - this->hv ) * 0.3f;
            else
            {
                if( this->clickfade > 0.005f )
                    this->clickfade *= 0.2f;
                else
                    this->clickfade = 0;
                if( this->clickfade > 0.01f && !this->pos.bDrag )
                    this->hv += ( 0.0f - this->hv ) * 0.7f;
                else
                {
                    hid = r->getHoverId();
                    if( this->compareId( hid ) )
                        this->hv += ( GUI_HOVER_MAX - this->hv ) * 0.3f;
                    else
                        this->hv += ( 0.0f - this->hv ) * 0.3f;
                }
            }
        }
        if( p_matrix )
            this->mat.copy( p_matrix );
        else
            this->mat.setIdentity();

        z = (float)this->z / -8.0f;
        if( this->follow_cursor.bEnabled )
            this->mat.translate( this->follow_cursor.cx + this->follow_cursor.x, this->follow_cursor.cy + this->follow_cursor.y, z );
        else
            this->mat.translate( this->pos.x + this->drag_pos.ox, this->pos.y + this->drag_pos.oy, z );

        if( this->bIsHover )
        {
            if( this->bIsFade )
                z = 1.0f - this->fade * this->fade;
            else
                z = 0;
            z += 0.02f * this->hv / GUI_HOVER_MAX;

            if( z > 0.001f )
            {
                this->mat.translate( -this->pos.w * 0.5f * z, -this->pos.h * 0.5f * z, 0 );
                this->mat.scale( z + 1, z + 1, 1 );
            }

            if( this->bIsFade )
            {
                z = 1.0f - this->fade;
                if( z > 0.01f )
                {
                    this->mat.translate( -this->pos.w * 0.5f, -this->pos.h * 0.5f, -z * 2 );
                    this->mat.rotateY( 5.0f * z );
                    this->mat.rotateZ( 10.0f * z );
                    this->mat.translate( this->pos.w * 0.5f, this->pos.h * 0.5f, 0 );
                }
            }
        }

        this->undo_mat.inverse( &this->mat );

        l = &this->children;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (renderer_gui_writelock *)o.tryWriteLock( p, 3, "renderer_gui::redoMatrix" );
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
    bool renderer_gui::processMouse( renderer_gui_man_writelock *r, float x, float y, float px, float py, bool lb, bool rb, dpid focus_id )
    {
        dpxyz_f p;
        gui_writelock *g;
        shared_obj_guard o;
        std::list<renderer_gui *> *l;
        std::list<renderer_gui *>::iterator i;
        renderer_gui *pr;
        renderer_gui_writelock *pl;
        bool bIsFocus;
        unsigned int z, max_z;

        this->follow_cursor.cx = px;
        this->follow_cursor.cy = py;

        p.x = x;
        p.y = y;
        p.z = (float)this->z / -8.0f;
        this->undo_mat.transform( &p );

        this->bIsHover = 0;
        this->z++;
/*
        bIsFocus = dpid_compare( &this->id, &this->focus_id );

        if( p.x < 0 || p.y < 0 )
            bIsFocus = 0;
        if( p.x >= this->pos.w || p.y >= this->pos.h )
            bIsFocus = 0;

        this->hover_id = dpid_null();
        if( lb )
            this->focus_id = dpid_null();

        this->hover_id = this->id;
        if( lb )
            this->focus_id = this->id;

        if( lb  && bIsFocus )
        {
            if( !this->bIsDrag )
            {
                this->drag_pos.x = x;
                this->drag_pos.y = y;
                this->bIsDrag = this->pos.bDrag;
            }
            else
            {
                this->drag_pos.ox = x - this->drag_pos.x;
                this->drag_pos.oy = y - this->drag_pos.y;
            }
        }

        if( !lb || !bIsFocus )
        {
            if( this->bIsDrag )
            {
                this->pos.x += this->drag_pos.ox;
                this->pos.y += this->drag_pos.oy;
                g = (gui_writelock *)o.tryWriteLock( this->g, 100, "renderer_gui::processMouse" );
                if( g )
                {
                    g->setPosition( this->pos.x, this->pos.y );
                    o.unlock();
                }
            }
            this->drag_pos.ox = 0;
            this->drag_pos.oy = 0;
            this->bIsDrag = 0;
        }
*/

        max_z = 0;
        l = &this->children;
        for( i = l->begin(); i != l->end(); ++i )
        {
            pr = *i;
            z = pr->getZ();
            if( z >= max_z )
                max_z = z + 1;
        }

        for( z = 0; z < max_z; z++ )
        {
            for( i = l->begin(); i != l->end(); ++i )
            {
                pr = *i;
                if( pr->getZ() != z )
                    continue;
                pl = (renderer_gui_writelock *)o.tryWriteLock( pr, 100, "renderer_gui::processMouse" );
                if( !pl )
                    continue;
                //if( pl->)
                if( pl->processMouse( r, x, y, p.x, p.y, lb, rb, focus_id ) )
                {
                    this->hover_id = pl->getHoverId();
                    if( lb )
                        this->z = 0;
                    if( lb )
                        this->focus_id = pl->getFocusId();

                    this->bIsHover = 1;
                    return 1;
                }
            }
        }

        this->focus_id = this->id;
        if( p.x < 0 || p.y < 0 )
            return 0;
        if( p.x >= this->pos.w || p.y >= this->pos.h )
            return 0;
        this->bIsHover = 1;
        if( lb )
            this->z = 0;

        g = (gui_writelock *)o.tryWriteLock( this->g, 100, "renderer_gui::processMouse" );
        if( !g )
            return 1;
        g->processMouse( p.x, p.y, x, y, lb, rb );
        if( lb )
            this->clickfade = 100;

        std::cout << "processed mouse " << p.x << " " << p.y << " " << (int)lb << " " << (int)rb << " \r\n";

        return 1;
    }

    //process kb input
    bool renderer_gui::processKb( renderer_gui_man_writelock *r, std::string *sname, bool bIsDown )
    {
        gui_writelock *g;
        shared_obj_guard o;
        std::list<renderer_gui *> *l;
        std::list<renderer_gui *>::iterator i;
        renderer_gui *p;
        renderer_gui_writelock *pl;
        unsigned int max_z, z;

        l = &this->children;
        max_z = 0;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            if( p->getZ() >= max_z )
                max_z = p->getZ() + 1;
        }

        for( z = 0; z < max_z; z++ )
        {
            for( i = l->begin(); i != l->end(); ++i )
            {
                p = *i;
                if( p->getZ() != z )
                    continue;
                pl = (renderer_gui_writelock *)o.tryWriteLock( p, 500, "renderer::processGuiKbInput" );
                if( !pl )
                    continue;
                if( pl->processKb( r, sname, bIsDown ) )
                    return 1;
            }
        }

       // if( this->z > 0 )
         //   return 0;

        g = (gui_writelock *)o.tryWriteLock( this->g, 1000, "renderer_gui::processKb" );
        if( !g )
            return 1;

        g->processKb( sname, bIsDown );

        std::cout << "processed kb " << sname->c_str() << " " << (int)bIsDown << "\r\n";

        return 1;
    }

    //returns opacity
    float renderer_gui::getOpacity( void )
    {
        if( !this->bIsHover )
            return this->fade * this->opacity;
        return this->fade * this->opacity * ( 0.9f + 0.1f * this->hv / GUI_HOVER_MAX );
    }

    //returns true if alive
    bool renderer_gui::isAlive( void )
    {
        if( this->g )
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
    bool renderer_gui::getFocusChild( renderer_gui_man_writelock *r, dpid *fid )
    {
        *fid = this->focus_id;
        return !dpid_isZero( fid );
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

    //gets selected text from gui (copy or cut)
    bool renderer_gui::getSelectedText( std::string *s, bool bDoCut )
    {
        gui_writelock *g;
        shared_obj_guard o;

        g = (gui_writelock *)o.tryWriteLock( this->g, 1000, "renderer_gui::getSelectedText" );
        if( !g )
            return 0;

        return g->getSelectedText( s, bDoCut );
    }

    //sets selected text in gui (paste)
    bool renderer_gui::setSelectedText( std::string *s )
    {
        gui_writelock *g;
        shared_obj_guard o;

        g = (gui_writelock *)o.tryWriteLock( this->g, 1000, "renderer_gui::setSelectedText" );
        if( !g )
            return 0;

        return g->setSelectedText( s );
    }

    //get hovering gui id
    dpid renderer_gui::getHoverId( void )
    {
        return this->hover_id;
    }

    //get focus gui id
    dpid renderer_gui::getFocusId( void )
    {
        return this->focus_id;
    }

    //run gui from background task
    void renderer_gui::run( dpthread_lock *thd, render_api_context_writelock *ctx, renderer_gui_man_writelock *mgr )
    {
        gui_readlock *pl;
        shared_obj_guard o;
        dpbitmap *bm;

        this->runChildren( thd, ctx, mgr );

        if( this->bSyncBg || this->bSyncFg || this->bSyncPos )
        {

            pl = (gui_readlock *)o.tryReadLock( this->g, 100, "renderer_gui::runFromTask" );
            if( pl )
            {
                if( this->bSyncPos )
                {
                    pl->getDimensions( &this->pos );
                    this->bHasBg = pl->hasBg();
                    this->bHasFg = pl->hasFg();
                    this->bIsHover = pl->isHoverable();
                    this->bIsEdit = pl->isEditable();
                    this->bIsFade = pl->isFade();
                    this->follow_cursor.bEnabled = pl->followsCursor( &this->follow_cursor.x, &this->follow_cursor.y );
                    if( this->updateVb( &this->pos, ctx ) )
                        this->bSyncPos = 0;
                }

                if( !this->bHasBg && this->bSyncBg )
                    this->bSyncBg = 0;

                if( this->bHasBg && this->bSyncBg )
                {
                    bm = pl->getBg();
                    if( this->updateBg( bm, ctx ) )
                        this->bSyncBg = 0;
                }

                if( !this->bHasFg && this->bSyncFg )
                    this->bSyncFg = 0;

                if( this->bHasFg && this->bSyncFg )
                {
                    bm = pl->getFg();
                    if( this->updateFg( bm, ctx ) )
                        this->bSyncFg = 0;
                }
            }
        }
    }

    //render model
    void renderer_gui::render( dpthread_lock *thd, renderer_gui_man_writelock *ml, dpmatrix *m_world, render_api_context_writelock *ctx, render_api_commandlist_writelock *clist )
    {
        std::list<renderer_gui *> *l;
        std::list<renderer_gui *>::iterator i;
        renderer_gui *p;
        renderer_gui_writelock *pl;
        shared_obj_guard o;
        dpmatrix mat;

        mat.copy( m_world );
        mat.multiply( &this->mat );

        if( this->hasBg() || this->hasFg() )
        {
            clist->setMatrix( &mat );
            clist->setAlpha( this->getOpacity() );
            clist->setTexture( 0, 1 );
        }
        if( this->hasBg() )
        {
            clist->setTexture( this->render_tex_bg, 0 );
            clist->setIndexBuffer( this->render_ib_bg );
            clist->setVertexBuffer( this->render_vb_bg );
            clist->draw();
        }
        if( this->hasFg() )
        {
            clist->setTexture( this->render_tex_fg, 0 );
            clist->setIndexBuffer( this->render_ib_fg );
            clist->setVertexBuffer( this->render_vb_fg );
            clist->draw();
        }

        l = &this->children;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (renderer_gui_writelock *)o.tryWriteLock( p, 30, "renderer_gui::render" );
            if( !pl )
                continue;
            if( !pl->compareId( this->id ) )
                pl->render( thd, ml, m_world, ctx, clist );
        }
    }

    //override to handle bg texture update
    bool renderer_gui::updateBg( dpbitmap *bm, render_api_context_writelock *ctx )
    {
        if( this->render_tex_bg )
            delete this->render_tex_bg;
        this->render_tex_bg = ctx->makeTexture( bm );
        return this->render_tex_bg != 0;
    }

    //override to handle fg texture update
    bool renderer_gui::updateFg( dpbitmap *bm, render_api_context_writelock *ctx )
    {
        if( this->render_tex_fg )
            delete this->render_tex_fg;
        this->render_tex_fg = ctx->makeTexture( bm );
        return this->render_tex_fg != 0;
    }

    //override to handle vb update in renderer task
    bool renderer_gui::updateVb( gui_dims *p, render_api_context_writelock *ctx )
    {
        dpvertexindex_buffer vb;
        float w, h, bw, tw;

        bw = p->border_w;
        tw = p->border_tex_w;
        w = p->w;
        h = p->h;

        //fg
        makeBox( &vb, 0, 0, w, h, 0, 0, 1, 1 );
        if( this->render_vb_fg )
            delete this->render_vb_fg;
        this->render_vb_fg = ctx->makeVertexBuffer( vb.getVB() );
        if( !this->render_vb_fg )
            return 0;
        if( this->render_ib_fg )
            delete this->render_ib_fg;
        this->render_ib_fg = ctx->makeIndexBuffer( vb.getIB() );
        if( !this->render_ib_fg )
            return 0;

        //bg

        //tl
        makeBox( &vb, 0,         0,          bw,             bw,             0,          0,          tw, tw );
        //top
        makeBox( &vb, bw,        0,          w - bw - bw,    bw,             tw,         0,          1 - tw - tw, tw );
        //tr
        makeBox( &vb, w - bw,    0,          bw,             bw,             1 - tw,     0,          tw, tw );
        //left
        makeBox( &vb, 0,         bw,         bw,             h - bw - bw,    0,          tw,         tw, 1 - tw - tw );
        //center
        makeBox( &vb, bw,        bw,         w - bw - bw,    h - bw - bw,    tw,         tw,         1 - tw - tw, 1 - tw - tw );
        //right
        makeBox( &vb, w - bw,    bw,         bw,             h - bw - bw,    1 - tw,     tw,         tw, 1 - tw - tw );
        //bl
        makeBox( &vb, 0,         h - bw,     bw,             bw,             0,          1 - tw,     tw, tw );
        //bottom
        makeBox( &vb, bw,        h - bw,     w - bw - bw,    bw,             tw,         1 - tw,     1 - tw - tw, tw );
        //br
        makeBox( &vb, w - bw,    h - bw,     bw,             bw,             1 - tw,     1 - tw,     tw, tw );

        if( this->render_vb_bg )
            delete this->render_vb_bg;
        this->render_vb_bg = ctx->makeVertexBuffer( vb.getVB() );
        if( !this->render_vb_bg )
            return 0;
        if( this->render_ib_bg )
            delete this->render_ib_bg;
        this->render_ib_bg = ctx->makeIndexBuffer( vb.getIB() );
        if( !this->render_ib_bg )
            return 0;

        return 1;
    }

    //delete children
    void renderer_gui::deleteChildren( void )
    {
        std::list<renderer_gui *> *l, d;
        std::list<renderer_gui *>::iterator i;
        renderer_gui *p;

        l = &this->children;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            d.push_back( p );
        }
        l->clear();

        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            delete p;
        }
    }

    //run children
    void renderer_gui::runChildren( dpthread_lock *thd, render_api_context_writelock *ctx, renderer_gui_man_writelock *mgr )
    {
        std::list<renderer_gui *> *l, d;
        std::list<renderer_gui *>::iterator i;
        renderer_gui *p;
        renderer_gui_writelock *pl;
        shared_obj_guard o;

        this->syncChildren( thd, mgr );

        l = &this->children;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (renderer_gui_writelock *)o.tryWriteLock( p, 10, "renderer_gui::runChildren" );
            if( !pl )
                continue;
            pl->run( thd, ctx, mgr );
            if( !p->isAlive() )
                d.push_back( p );
            o.unlock();
        }

        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            this->children.remove( p );
            delete p;
        }
    }

    //sync children
    void renderer_gui::syncChildren( dpthread_lock *thd, renderer_gui_man_writelock *mgr )
    {
        std::list<renderer_gui *> *l, d;
        std::list<renderer_gui *>::iterator i;
        renderer_gui *p;
        renderer_gui_readlock *pl;
        shared_obj_guard o, ogul;
        dpid_btree pt;
        std::list<gui_ref *> lg, ng;
        std::list<gui_ref *>::iterator ig;
        gui_ref *gr;
        uint64_t t;
        gui_readlock *gul, *grl;
        gui_writelock *gwl;

        t = thd->getTicks();
        if( t - this->t_last_children_sync < 200 )
            return;
        this->t_last_children_sync = t;


        gul = (gui_readlock *)ogul.tryReadLock( this->g, 100, "renderer_gui::syncChildren" );
        if( !gul )
            return;

        l = &this->children;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (renderer_gui_readlock *)o.tryReadLock( p, 100, "renderer_gui::syncChildren" );
            if( !pl )
                continue;
            pt.addLeaf( pl->getId(), (void *)p );
            o.unlock();
        }

        gul->getChildren( &lg );

        for( ig = lg.begin(); ig != lg.end(); ++ig )
        {
            gr = *ig;
            grl = (gui_readlock *)o.tryReadLock( gr, 100, "renderer_gui::syncChildren" );
            if( !grl )
                continue;
            p = (renderer_gui *)pt.findLeaf( grl->getId() );
            if( p )
                pt.removeLeaf( p );
            else
                ng.push_back( gr );
        }

        for( ig = ng.begin(); ig != ng.end(); ++ig )
        {
            gr = *ig;
            gwl = (gui_writelock *)o.tryWriteLock( gr, 100, "renderer_gui::syncChildren" );
            if( !gwl )
                continue;
            p = mgr->genGui( gwl );
            if( !p )
                continue;

            gwl->setRenderer( p );
            this->children.push_back( p );
        }

        pt.getLeaves( (std::list<void *> *)&d );
        l = &d;
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            this->children.remove( p );
            delete p;
        }
    }

};
