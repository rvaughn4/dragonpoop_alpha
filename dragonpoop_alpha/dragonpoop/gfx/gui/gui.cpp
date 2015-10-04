
#include "gui.h"
#include "gui_ref.h"
#include "gui_readlock.h"
#include "gui_writelock.h"
#include "../../core/core.h"
#include "../gfx_writelock.h"
#include "../gfx_readlock.h"
#include "../gfx_ref.h"
#include "../../core/shared_obj/shared_obj_guard.h"
#include "../../renderer/renderer_gui/renderer_gui.h"
#include "../../renderer/renderer_gui/renderer_gui_ref.h"
#include "../../renderer/renderer_gui/renderer_gui_writelock.h"
#include "../dpfont/dpfont.h"
#include <sstream>
#include <string>
#include "../../core/dpthread/dpthread_lock.h"
#include "../../core/dptaskpool/dptaskpool_ref.h"
#include "../../core/dptaskpool/dptaskpool_writelock.h"

namespace dragonpoop
{
    
    //ctor
    gui::gui( gfx_writelock *g, dpid id ) : shared_obj( g->getCore()->getMutexMaster() )
    {
        this->c = g->getCore();
        this->g = (gfx_ref *)g->getRef();
        this->id = id;
        this->pid = dpid_null();
        this->bHasBg = this->bHasFg = 0;
        this->bPosChanged = this->bBgChanged = this->bFgChanged = 0;
        this->bRedraw = 1;
        this->bRepaintBg = 0;
        this->bRepaintFg = 1;
        this->r = 0;
        this->bWasBgDrawn = this->bWasFgDrawn = 0;
        this->z = 1;
        this->bMouseInput = 0;
        this->bOldLb = this->bOldRb = 0;
        this->bLb = this->bRb = 0;
        this->fnt_size = 0;
        this->fnt_clr.r = this->fnt_clr.g = this->fnt_clr.b = 0;
        this->fnt_clr.a = 255;
        this->pos.border_w = 10;
        this->pos.border_tex_w = 0.2f;
        this->cursor = 0;
        this->bIsSel = 0;
        this->sz_div = 0;
        this->bShiftDown = 0;
        this->redraw_timer = 0;
        this->bIsHover = 0;
        this->bIsEdit = 0;
        this->margin_size = 0;
    }
    
    //dtor
    gui::~gui( void )
    {
        shared_obj_writelock *l;
        shared_obj_guard o;
        
        l = o.tryWriteLock( this, 5000, "gui::~gui" );
        o.unlock();
        
        this->unlink();
        delete this->g;
        if( this->r )
            delete this->r;
    }
    
    //return core
    core *gui::getCore( void )
    {
        return this->c;
    }
    
    //generate read lock
    shared_obj_readlock *gui::genReadLock( shared_obj *p, dpmutex_readlock *l )
    {
        return new gui_readlock( (gui *)p, l );
    }
    
    //generate write lock
    shared_obj_writelock *gui::genWriteLock( shared_obj *p, dpmutex_writelock *l )
    {
        return new gui_writelock( (gui *)p, l );
    }
    
    //generate ref
    shared_obj_ref *gui::genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k )
    {
        return new gui_ref( (gui *)p, k );
    }
    
    //run gui
    void gui::run( dpthread_lock *thd, gui_writelock *g )
    {
        shared_obj_guard o;
        renderer_gui_writelock *l;
        uint64_t t;
        gfx_writelock *gl;
        
        t = thd->getTicks();
        if( this->z == 0 )
        {
            if( this->bIsEdit || this->bWasSel )
            {
                this->redraw_timer = 300;
                this->bWasSel = 0;
            }
            else
                this->redraw_timer = 0;
        }
        if( this->z != 0 && !cur_flash )
            this->redraw_timer = 0;
        
        if( this->redraw_timer && t - this->t_last_redraw > this->redraw_timer )
        {
            this->redraw();
            if( this->bIsEdit )
                this->cur_flash = !this->cur_flash;
            else
                this->cur_flash = 0;
        }
        
        if( this->bRedraw && t - this->t_last_redraw > 100 )
        {
            this->t_last_redraw = t;

            if( ( !this->bWasBgDrawn || this->bRepaintBg ) && this->bHasBg )
            {
                this->repaintBg( g, &this->bgtex, this->pos.w, this->pos.h );
                this->bBgChanged = 1;
                this->bWasBgDrawn = 1;
            }
            
            if( ( !this->bWasFgDrawn || this->bRepaintFg ) && this->bHasFg )
            {
                this->repaintFg( g, &this->fgtex, this->pos.w, this->pos.h );
                this->bFgChanged = 1;
                this->bWasFgDrawn = 1;
            }
            
            this->bRedraw = 0;
        }
        
        if( this->bPosChanged || this->bBgChanged || this->bFgChanged )
        {
            if( this->r )
            {
                l = (renderer_gui_writelock *)o.tryWriteLock( this->r, 500, "gui::run" );
                if( l )
                {
                    if( this->bPosChanged )
                        l->syncPos();
                    if( this->bBgChanged )
                        l->syncBg();
                    if( this->bFgChanged )
                        l->syncFg();
                }

                this->bPosChanged = this->bBgChanged = this->bFgChanged = 0;
            }
        }
        
        if( this->bMouseInput )
        {
            if( this->bOldLb != this->bLb )
            {
                gl = (gfx_writelock *)o.tryWriteLock( this->g, 1000, "gui::run" );
                if( gl )
                    this->setFocus( gl );
                o.unlock();
            }
            if( this->bOldLb != this->bLb )
                this->handleMouseClick( this->mx, this->my, 0, this->bLb );
            if( this->bOldRb != this->bRb )
                this->handleMouseClick( this->mx, this->my, 1, this->bRb );
            this->bMouseInput = 0;
            this->bOldLb = this->bLb;
            this->bOldRb = this->bRb;
        }
        
        if( this->bKeyInput )
        {
            this->handleKbEvent( &this->skb, this->bKeyDown );
            this->bKeyInput = 0;
        }
    }
    
    //returns id
    dpid gui::getId( void )
    {
        return this->id;
    }
    
    //compares id
    bool gui::compareId( dpid id )
    {
        return dpid_compare( &this->id, &id );
    }
    
    //set width and height
    void gui::setWidthHeight( float w, float h )
    {
        this->pos.w = w;
        this->pos.h = h;
        this->redraw();
        this->bPosChanged = 1;
    }
    
    //set top left pos
    void gui::setPosition( float x, float y )
    {
        this->pos.x = x;
        this->pos.y = y;
        this->bPosChanged = 1;
    }
    
    //get dimensions
    void gui::getDimensions( gui_dims *p )
    {
        *p = this->pos;
    }
    
    //cause redraw of background and forground texture
    void gui::redraw( void )
    {
        this->bRedraw = 1;
    }
    
    //set background redraw mode
    void gui::enableBgRedraw( bool b )
    {
        this->bRepaintBg = b;
    }
    
    //set forground redraw mode
    void gui::enableFgRedraw( bool b )
    {
        this->bRepaintFg = b;
    }
    
    //set background mode
    void gui::enableBg( bool b )
    {
        this->bHasBg = b;
    }
    
    //set forground mode
    void gui::enableFg( bool b )
    {
        this->bHasFg = b;
    }
    
    //returns true if has background texture
    bool gui::hasBg( void )
    {
        return this->bHasBg;
    }
    
    //returns true if has forground texture
    bool gui::hasFg( void )
    {
        return this->bHasFg;
    }
    
    //override to paint background texture
    void gui::repaintBg( gui_writelock *g, dpbitmap *bm, float w, float h )
    {
        
    }
    
    //override to paint forground texture
    void gui::repaintFg( gui_writelock *g, dpbitmap *bm, float w, float h )
    {
        dprgba c;
        
        if( !this->hasFg() )
        {
            bm->reset();
            return;
        }
        
        if( w > 128 && sz_div )
            w = w / sz_div;
        if( h > 128 && sz_div )
            h = h / sz_div;
        
        bm->resize( w, h );
        c.r = c.b = c.g = c.a = 0;
        bm->clear( &c );
        
        this->drawText( bm );
    }
    
    //reset text loc
    void gui::resetTxtLoc( void )
    {
        this->line_front = 0;
        this->last_txt_line = 0;
        this->txt_loc.clear();
    }
    
    //add text loc
    void gui::addTxtLoc( float x, float y, float w, float h, unsigned int line_no, unsigned int char_no )
    {
        gui_txt_loc l;
        
        if( line_no != this->last_txt_line )
        {
            this->line_front = (unsigned int)this->txt_loc.size();
            this->last_txt_line = line_no;
        }
        
        l.x = x;
        l.y = y;
        l.w = w;
        l.h = h;
        l.front = this->line_front;
        l.line_no = line_no;
        l.char_no = char_no;
        
        this->txt_loc.push_back( l );
    }
    
    //override to customize font rendering
    void gui::drawText( dpbitmap *bm )
    {
        unsigned int i, e, d, fnt_size, sdw_off, ln, tt;
        unsigned char c, *cb;
        int w, h, x, y, cw, ch, lch;
        dprgba clr, clr_dark, clr_light;
        std::string s, fnt_face;
        std::size_t loc_sp;
        dpfont f;
        bool cur_drawn;
        dpxywh cur_pos;
        
        w = bm->getWidth();
        h = bm->getHeight();
        
        fnt_face.assign( "sans" );
        
        fnt_size = this->fnt_size;
        if( !fnt_size )
            fnt_size = ( w + h ) / 20;
        if( this->sz_div )
            fnt_size /= this->sz_div;
        sdw_off = fnt_size / 120 + 1;
        
        clr = this->fnt_clr;
        
        clr_dark.r = clr.r * 0.125f;
        clr_dark.g = clr.g * 0.125f;
        clr_dark.b = clr.b * 0.125f;
        clr_dark.a = clr.a * 0.5f;
        clr_light.r = clr.r * 0.3f + 170;
        clr_light.g = clr.g * 0.3f + 170;
        clr_light.b = clr.b * 0.3f + 170;
        clr_light.a = clr.a * 0.5f;
        
        if( !f.openFont( fnt_face.c_str(), fnt_size ) )
            return;
        
        e = (unsigned int)this->stxt.length();
        cb = (unsigned char *)this->stxt.c_str();
        this->resetTxtLoc();
        
        w -= this->margin_size + this->margin_size;
        h -= this->margin_size + this->margin_size;
        
        x = y = this->margin_size;
        cw = lch = 0;
        ln = 0;
        cur_drawn = this->cur_flash || ( this->z != 0 ) || !this->bIsEdit;
        for( i = 0; i < e; i++ )
        {
            c = cb[ i ];
            
            //handle inline codes first
            
            //27 /e font size
            if( c == 27 && e - i >= 4 )
            {
                std::stringstream ss;

                s.assign( (char *)&cb[ i + 1 ], 3 );
                ss << s;
                ss >> d;
                if( this->sz_div )
                    d /= this->sz_div;
                if( d > 0 && f.openFont( fnt_face.c_str(), d ) )
                {
                    fnt_size = d;
                    sdw_off = fnt_size / 120 + 1;
                }
                else
                {
                    if( !f.openFont( fnt_face.c_str(), fnt_size ) )
                        return;
                }
                    
                i += 3;
                continue;
            }
            
            //12 /f font face
            if( c == 12 && e - i >= 4 )
            {
                s.assign( (char *)&cb[ i + 1 ], e - i );

                loc_sp = s.find( " " );
                if( loc_sp >= e || loc_sp <= 0 )
                    continue;
                s = s.substr( 0, loc_sp );
                i += loc_sp + 1;
                if( s.size() < 4 )
                    continue;
                if( f.openFont( s.c_str(), fnt_size ) )
                    fnt_face = s;
                else
                {
                    if( !f.openFont( fnt_face.c_str(), fnt_size ) )
                        return;
                }
                
                continue;
            }
            
            //7 /a font color
            if( c == 7 && e - i >= 10 )
            {
                std::stringstream ssr, ssg, ssb;
                
                i += 1;
                s.assign( (char *)&cb[ i ], 3 );
                ssr << s;
                ssr >> d;
                clr.r = d;
                
                i += 3;
                s.assign( (char *)&cb[ i ], 3 );
                ssg << s;
                ssg >> d;
                clr.g = d;
                
                i += 3;
                s.assign( (char *)&cb[ i ], 3 );
                ssb << s;
                ssb >> d;
                clr.b = d;
                
                i += 2;
                clr.a = 255;
                
                clr_dark.r = clr.r * 0.125f;
                clr_dark.g = clr.g * 0.125f;
                clr_dark.b = clr.b * 0.125f;
                clr_dark.a = clr.a * 0.5f;
                clr_light.r = clr.r * 0.3f + 170;
                clr_light.g = clr.g * 0.3f + 170;
                clr_light.b = clr.b * 0.3f + 170;
                clr_light.a = clr.a * 0.5f;
                
                continue;
            }
            
            // \r
            if( c == 13 )
            {
                x = this->margin_size;
                continue;
            }
            // \n
            if( c == 10 )
            {
                x = this->margin_size;
                if( lch < fnt_size )
                    lch = fnt_size;
                y += lch;
                ln++;
                continue;
            }
            // \t
            if( c == 9 )
            {
                tt = 0;
                while( tt <= x )
                    tt += 4 * fnt_size;
                x = tt;
                continue;
            }
            
            f.draw( c, 0, 0, 0, &cw, &ch, 0 );
            if( cw + x > w )
            {
                x = this->margin_size;
                if( lch < fnt_size )
                    lch = fnt_size;
                y += lch;
                lch = ch;
                ln++;
            }
            if( ch > lch )
                lch = ch;
            if( x + cw > w || y + ch > h )
                return;
            
            if(
                    ( !cur_drawn && i >= this->cursor )
                    ||
                    (
                        this->bIsSel &&
                        (
                            ( i >= this->cursor && i <= this->sel_cursor )
                            ||
                            ( i >= this->sel_cursor && i <= this->cursor )
                         )
                    )
                )
            {
                cur_pos.p.x = x;
                cur_pos.p.y = y;
                cur_pos.w = cw;
                cur_pos.h = fnt_size * 5 / 4;
                bm->clear( &clr, &cur_pos );
                cur_drawn = 1;
                f.draw( c, x, y, bm, 0, 0, &clr_light );
            }
            else
            {
                f.draw( c, x - sdw_off, y - sdw_off, bm, 0, 0, &clr_light );
                f.draw( c, x + sdw_off, y + sdw_off, bm, 0, 0, &clr_dark );
                f.draw( c, x, y, bm, 0, 0, &clr );
            }
            
            if( this->sz_div )
                this->addTxtLoc( x * this->sz_div, y * this->sz_div, cw * this->sz_div, ch * this->sz_div, ln, i );
            else
                this->addTxtLoc( x, y, cw, ch, ln, i );
            x += cw;
        }

        if( !cur_drawn )
        {
            cur_pos.p.x = x;
            cur_pos.p.y = y;
            cur_pos.w = fnt_size / 3;
            cur_pos.h = fnt_size * 5 / 4;
            bm->clear( &clr, &cur_pos );
        }
        
    }
    
    //set parent id
    void gui::setParentId( dpid id )
    {
        this->pid = id;
    }
    
    //get parent id
    dpid gui::getParentId( void )
    {
        return this->pid;
    }
    
    //returns true if has renderer
    bool gui::hasRenderer( void )
    {
        if( !this->r || !this->r->isLinked() )
            return 0;
        return 1;
    }
    
    //set renderer
    void gui::setRenderer( renderer_gui *g )
    {
        shared_obj_guard o;
        renderer_gui_writelock *l;
        
        if( this->r )
            delete this->r;
        this->r = 0;
        
        l = (renderer_gui_writelock *)o.tryWriteLock( g, 300, "gui::setRenderer" );
        if( !l )
            return;
        this->r = (renderer_gui_ref *)l->getRef();
    }

    //returns pointer to bg texture
    dpbitmap *gui::getBg( void )
    {
        return &this->bgtex;
    }
    
    //returns pointer to fg texture
    dpbitmap *gui::getFg( void )
    {
        return &this->fgtex;
    }
    
    //returns z order
    unsigned int gui::getZ( void )
    {
        return this->z;
    }
    
    //sets focus
    void gui::setFocus( gfx_readlock *g )
    {
        std::list<gui_ref *> l;
        g->getGuis( &l );
        this->setFocus( &l );
    }
    
    //sets focus
    void gui::setFocus( gfx_writelock *g )
    {
        std::list<gui_ref *> l;
        g->getGuis( &l );
        this->setFocus( &l );
    }
    
    //sets focus
    void gui::setFocus( std::list<gui_ref *> *l )
    {
        shared_obj_guard o;
        gui_writelock *pl;
        gui_ref *p;
        std::list<gui_ref *>::iterator i;
        
        for( i = l->begin(); i != l->end(); ++i )
        {
            p = *i;
            pl = (gui_writelock *)o.tryWriteLock( p, 100, "gui::setFocus" );
            if( !pl )
                continue;
            if( pl->t->z < 20 )
            {
                pl->t->z++;
                pl->t->bPosChanged = 1;
            }
        }
        o.unlock();
        
        this->bPosChanged = 1;
        this->z = 0;
    }
    
    //returns true if has focus
    bool gui::hasFocus( void )
    {
        return this->z == 0;
    }
 
    //process mouse input
    void gui::processMouse( float x, float y, bool lb, bool rb )
    {
        this->mx = x;
        this->my = y;
        this->bLb = lb;
        this->bRb = rb;
        this->bMouseInput = 1;
    }
    
    //process kb input
    void gui::processKb( std::string *skey, bool bDown )
    {
        this->skb.assign( *skey );
        this->bKeyDown = bDown;
        this->bKeyInput = 1;
    }
    
    //override to handle mouse button
    void gui::handleMouseClick( float x, float y, bool isRight, bool isDown )
    {
        if( !isRight && isDown )
        {
            if( this->bShiftDown )
            {
                this->bIsSel = 1;
                this->sel_cursor = this->findCursor( x, y );
            }
            else
            {
                this->bWasSel = this->bIsSel;
                this->bIsSel = 0;
                this->cursor = this->findCursor( x, y );
            }
            this->cur_flash = 1;
            if( this->bIsSel || this->bIsEdit )
                this->redraw();
        }
    }
    
    //override to handle keyboard button
    void gui::handleKbEvent( std::string *skey, bool isDown )
    {

        if( isDown )
        {

            if( this->bIsEdit )
            {
                
                if( skey->size() == 1 )
                {
                    this->insert( skey->c_str() );
                    this->redraw();
                    return;
                }
                
                if( skey->compare( "Enter" ) == 0 || skey->compare( "Return" ) == 0 )
                {
                    this->insert( "\n" );
                    this->redraw();
                    return;
                }
                
                if( skey->compare( "Tab" ) == 0 )
                {
                    this->insert( "\t" );
                    this->redraw();
                    return;
                }
                
                if( skey->compare( "Backspace" ) == 0 )
                {
                    this->backspace();
                    this->redraw();
                    return;
                }
                
                if( skey->compare( "Delete" ) == 0 )
                {
                    this->delete_();
                    this->redraw();
                    return;
                }
            }
            
            if( skey->compare( "Left" ) == 0 )
            {
                this->left();
                if( this->bIsEdit || this->bShiftDown || this->bIsSel )
                    this->redraw();
                return;
            }
            
            if( skey->compare( "Right" ) == 0 )
            {
                this->right();
                if( this->bIsEdit || this->bShiftDown || this->bIsSel )
                    this->redraw();
                return;
            }

            if( skey->compare( "Shift" ) == 0 )
            {
                this->bShiftDown = 1;
                this->redraw();
            }
        }
        else
        {
            
            if( skey->compare( "Shift" ) == 0 )
            {
                this->bShiftDown = 0;
                this->redraw();
            }

        }
    }
    
    //set text
    void gui::setText( const char *c )
    {
        this->stxt.assign( c );
        this->cursor = (unsigned int)this->stxt.length();
        this->bIsSel = 0;
    }
    
    //set text
    void gui::setText( std::string *s )
    {
        this->stxt.assign( *s );
        this->cursor = (unsigned int)this->stxt.length();
        this->bIsSel = 0;
    }
    
    //get text
    void gui::getText( std::string *s )
    {
        s->assign( this->stxt );
    }
    
    //set font size
    void gui::setFontSize( unsigned int s )
    {
        this->fnt_size = s;
    }
    
    //get font size
    unsigned int gui::getFontSize( void )
    {
        return this->fnt_size;
    }
    
    //set font color
    void gui::setFontColor( dprgba *c )
    {
        this->fnt_clr = *c;
    }
    
    //get font color
    dprgba *gui::getFontColor( void )
    {
        return &this->fnt_clr;
    }

    //backspace text
    void gui::backspace( void )
    {
        if( this->bIsSel )
            return this->delete_();
        
        if( this->cursor > this->stxt.size() )
            this->cursor = (unsigned int)this->stxt.size();
        if( this->cursor < 1 )
            return;
        
        this->cursor--;
        this->stxt.replace( this->cursor, 1, "" );
    }
    
    //delete text
    void gui::delete_( void )
    {
        unsigned int a, b;
        
        if( this->bIsSel )
        {
            if( this->cursor <= this->sel_cursor )
            {
                a = this->cursor;
                b = this->sel_cursor;
            }
            else
            {
                b = this->cursor;
                a = this->sel_cursor;
            }
            this->bWasSel = this->bIsSel;
            this->bIsSel = 0;
        }
        else
        {
            a = this->cursor;
            b = this->cursor + 1;
        }
        
        if( a > this->stxt.size() )
            a = (unsigned int)this->stxt.size();
        if( b > this->stxt.size() )
            b = (unsigned int)this->stxt.size();
        if( a >= this->stxt.size() )
            return;
        this->stxt.replace( a, b - a + 1, "" );
        this->cursor = a;
    }
    
    //insert text
    void gui::insert( const char *c )
    {
        std::string s( c );
        
        unsigned int a, b;
        
        if( this->bIsSel )
        {
            if( this->cursor <= this->sel_cursor )
            {
                a = this->cursor;
                b = this->sel_cursor;
            }
            else
            {
                b = this->cursor;
                a = this->sel_cursor;
            }
            this->bWasSel = this->bIsSel;
            this->bIsSel = 0;
        }
        else
        {
            a = this->cursor;
            b = this->cursor;
        }
        
        if( a > this->stxt.size() )
            a = (unsigned int)this->stxt.size();
        if( b > this->stxt.size() )
            b = (unsigned int)this->stxt.size();
        
        if( a >= this->stxt.size() )
        {
            this->stxt.append( s );
            this->cursor = a + (unsigned int)s.size();
            return;
        }
        
        if( a == b )
        {
            this->stxt.insert( a, s );
            this->cursor = a + (unsigned int)s.size();
            return;
        }
        
        this->stxt.replace( a, b - a + 1, s );
        this->cursor = a + (unsigned int)s.size();
    }
    
    //move cursor left
    void gui::left( void )
    {
        if( this->bShiftDown )
        {
            if( !this->bIsSel )
                this->sel_cursor = this->cursor;
            this->bIsSel = 1;
            if( this->sel_cursor > 0 )
                this->sel_cursor--;
        }
        else
        {
            this->bWasSel = this->bIsSel;
            this->bIsSel = 0;
            if( this->cursor > 0 )
                this->cursor--;
        }
        this->cur_flash = 1;
    }
    
    //move cursor right
    void gui::right( void )
    {
        if( this->bShiftDown )
        {
            if( !this->bIsSel )
                this->sel_cursor = this->cursor;
            this->bIsSel = 1;
            if( this->sel_cursor < this->stxt.size() )
                this->sel_cursor++;
        }
        else
        {
            this->bWasSel = this->bIsSel;
            this->bIsSel = 0;
            if( this->cursor < this->stxt.size() )
                this->cursor++;
        }
        this->cur_flash = 1;
    }
    
    //move cursor to end
    void gui::end( void )
    {
        
    }
    
    //move cursor to home
    void gui::home( void )
    {
        
    }
    
    //move cursor to top
    void gui::top( void )
    {
        this->cursor = 0;
    }
    
    //move cursor to bottom
    void gui::bottom( void )
    {
        this->cursor = (unsigned int)this->stxt.size();
    }
    
    //move cursor up
    void gui::up( void )
    {
        
    }
    
    //move cursor down
    void gui::down( void )
    {
        
    }
    
    //find cursor by mouse coords
    unsigned int gui::findCursor( float x, float y )
    {
        unsigned int i, e, f;
        gui_txt_loc *t;
        
        e = (unsigned int)this->txt_loc.size();
        for( f = i = 0; i < e; i++ )
        {
            t = &this->txt_loc[ i ];
            if( t->x <= x && t->y <= y )
                f = t->char_no;
        }
        
        return f;
    }

    //gets selected text from gui (copy or cut)
    bool gui::getSelectedText( std::string *s, bool bDoCut )
    {
        unsigned int a, b;
        
        if( !this->bIsSel || this->cursor == this->sel_cursor )
            return 0;
        
        if( this->cursor <= this->sel_cursor )
        {
            a = this->cursor;
            b = this->sel_cursor;
        }
        else
        {
            b = this->cursor;
            a = this->sel_cursor;
        }
        this->bIsSel = 0;
        
        if( a > this->stxt.size() )
            a = (unsigned int)this->stxt.size();
        if( b > this->stxt.size() )
            b = (unsigned int)this->stxt.size();
        if( a >= this->stxt.size() )
            return 0;

        s->assign( this->stxt.substr( a, b - a + 1 ) );
        
        //if( bDoCut )
          //  this->delete_();
        
        return 1;
    }
    
    //sets selected text in gui (paste)
    bool gui::setSelectedText( std::string *s )
    {
        this->insert( s->c_str() );
        return 1;
    }
    
    //add gui
    void gui::addGui( gui *g )
    {
        gfx_writelock *gl;
        shared_obj_guard o, o1;
        gui_writelock *gul;
        
        gl = (gfx_writelock *)o.tryWriteLock( this->g, 2000, "gui::addGui" );
        if( !gl )
            return;
        gul = (gui_writelock *)o1.tryWriteLock( g, 2000, "gui::addGui" );
        if( !gul )
            return;
        
        gul->setParentId( this->id );
        o1.unlock();
        
        gl->addGui( g );
    }
    
    //set editable
    void gui::setEditMode( bool b )
    {
        this->bIsEdit = b;
    }
    
    //returns true if editable
    bool gui::isEditable( void )
    {
        return this->bIsEdit;
    }
    
    //sets hoverable
    void gui::setHoverMode( bool b )
    {
        this->bIsHover = b;
    }
    
    //returns true if hoverable
    bool gui::isHoverable( void )
    {
        return this->bIsHover;
    }
    
    //generates new id
    dpid gui::genId( void )
    {
        dptaskpool_ref *r;
        dptaskpool_writelock *l;
        shared_obj_guard o;
        
        r = this->c->getTaskpool();
        if( !r )
            return dpid_null();
        l = (dptaskpool_writelock *)o.tryWriteLock( r, 2000, "gui::genId" );
        if( !l )
            return dpid_null();
        
        return l->genId();
    }

    //set margin size
    void gui::setMargin( unsigned int m )
    {
        this->margin_size = m;
    }
    
    //get margin size
    unsigned int gui::getMargin( void )
    {
        return this->margin_size;
    }
    
};
