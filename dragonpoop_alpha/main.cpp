
#include "dragonpoop/dragonpoop.h"

void main_pause( dragonpoop::core *c, unsigned int s )
{
    unsigned int i;
    s *= 1000;
    for( i = 0; i < s && c->isRunning(); i += 100 )
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
}

void main_wait_loader( dragonpoop::core *c, dragonpoop::model_loader_ref *lr )
{
    dragonpoop::model_loader_readlock *lw;
    bool b = 1;
    dragonpoop::shared_obj_guard o;
    
    while( b )
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );

        if( !lr || !lr->isLinked() )
        {
            b = 0;
            continue;
        }
        lw = (dragonpoop::model_loader_readlock *)o.tryReadLock( lr, 100, "main_wait_loader" );
        if( !lw )
            continue;
        if( !lw->isRunning() )
        {
            b = 0;
            continue;
        }
        o.unlock();
    }
    
}

class test_gui : public dragonpoop::gui
{
    
protected:
    
    //override to paint background texture
    virtual void repaintBg( dragonpoop::gui_writelock *g, dragonpoop::dpbitmap *bm, float w, float h )
    {
        dragonpoop::dpbitmap b;
        dragonpoop::dprgba c;
        
        c.r = 50;
        c.g = 55;
        c.b = 50;
        c.a = 255;
        
        b.loadFile( "white_gui_box.bmp" );
        bm->resize( b.getWidth(), b.getHeight() );
        b.multiply( &c );
        bm->blit( &b, 0 );
    }

    
    //override to paint background texture
    virtual void repaintFg( dragonpoop::gui_writelock *g, dragonpoop::dpbitmap *bm, float w, float h )
    {
        this->gui::repaintFg( g, bm, w, h );
    }
    
    //override to handle mouse button
    virtual void handleMouseClick( float x, float y, bool isRight, bool isDown )
    {
        this->gui::handleMouseClick( x, y, isRight, isDown );
        //dragonpoop::gui_dims p;
      //  this->getDimensions( &p );
    //    if( !isRight && isDown )
  //          this->setPosition( x + p.x, y + p.y );
    }
    
public:
    
    //ctor
    test_gui( dragonpoop::gfx_writelock *g, dragonpoop::dpid id, float x, float y, float w, float h ) : dragonpoop::gui( g, id, dragonpoop::dpid_null() )
    {
        this->enableBg( 1 );
        this->enableFg( 1 );
        this->setPosition( x, y );
        this->setWidthHeight( w, h );
        this->setText( " Hello! \t\tabcdefghi \n jklmopqrstuvwxyz \fancient !@#$%^&*() 1234567890" );
    }
    
    //dtor
    virtual ~test_gui( void )
    {
        
    }


};

int main( int argc, const char * argv[] )
{

    dragonpoop::core *c = new dragonpoop::core();
    
    dragonpoop::gfx_ref *gr;
    dragonpoop::gfx_writelock *gl;
    dragonpoop::shared_obj_guard o;
    dragonpoop::model_loader_ref *lr;
    dragonpoop::gui *tg, *tg2;
    
    gr = c->getGfx();
    
    lr = 0;
  
    gl = (dragonpoop::gfx_writelock *)o.writeLock( gr, "main" );
    gl->loadModel( "low_dragon", "", "3drt_dragon_low.dpmodel", 0, &lr );
    o.unlock();
    main_wait_loader( c, lr );
    delete lr;

    dragonpoop::dpid mid, mmid;
    main_pause( c, 5 );
    gl = (dragonpoop::gfx_writelock *)o.writeLock( gr, "main" );
    mid = gl->makeModelInstance( "low_dragon", 0 );
    mmid = gl->makeModelInstance( "low_dragon", 0 );
    o.unlock();

    gl = (dragonpoop::gfx_writelock *)o.writeLock( gr, "main" );
    tg = new test_gui( gl, mid, 10, 10, 800, 800 );
    gl->addGui( tg );
    tg2 = new test_gui( gl, mmid, 300, 300, 800, 800 );
    gl->addGui( tg2 );
    o.unlock();
    
    main_pause( c, 5 );
    gl = (dragonpoop::gfx_writelock *)o.writeLock( gr, "main" );
    gl->startAnimation( "low_dragon", mid, "turn left", 1, 1 );
    gl->startAnimation( "low_dragon", mmid, "turn right", 1, 1 );
    o.unlock();
    
    main_pause( c, 5 );
    gl = (dragonpoop::gfx_writelock *)o.writeLock( gr, "main" );

    //gl->saveModel( "low_dragon", "", "3drt_felhound_high.dpmodel", 0 );
 
    o.unlock();
    delete tg2;
  
    while( c->isRunning() )
        std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );

    delete tg;
    delete c;

    return 0;
}
