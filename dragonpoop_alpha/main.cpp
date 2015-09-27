
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
        b.loadFile( "example.bmp" );
        bm->resize( b.getWidth(), b.getHeight() );
        bm->blit( &b, 0 );
    }
    
    //override to paint forground texture
    virtual void repaintFg( dragonpoop::gui_writelock *g, dragonpoop::dpbitmap *bm, float w, float h )
    {
        
    }
    
public:
    
    //ctor
    test_gui( dragonpoop::gfx_writelock *g, dragonpoop::dpid id ) : dragonpoop::gui( g, id, dragonpoop::dpid_null() )
    {
        this->enableBg( 1 );
        this->setPosition( 0, 0 );
        this->setWidthHeight( 500, 500 );
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
    dragonpoop::gui *tg;
    
    gr = c->getGfx();
    
    lr = 0;
  
    gl = (dragonpoop::gfx_writelock *)o.writeLock( gr, "main" );
    gl->loadModel( "low_dragon", "", "3drt_dragon_high.dpmodel", 0, &lr );
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
    tg = new test_gui( gl, mid );
    gl->addGui( tg );
    o.unlock();
    
    main_pause( c, 5 );
    gl = (dragonpoop::gfx_writelock *)o.writeLock( gr, "main" );
    gl->startAnimation( "low_dragon", mid, "fly_forward", 1, 1 );
    gl->startAnimation( "low_dragon", mmid, "run", 1, 1 );
    o.unlock();
    
    main_pause( c, 5 );
    gl = (dragonpoop::gfx_writelock *)o.writeLock( gr, "main" );

    //gl->saveModel( "low_dragon", "", "3drt_felhound_high.dpmodel", 0 );
 
    o.unlock();
  
    while( c->isRunning() )
        std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );

    delete tg;
    delete c;

    return 0;
}
