
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

int main( int argc, const char * argv[] )
{

    dragonpoop::core *c = new dragonpoop::core();
    
    dragonpoop::gfx_ref *gr;
    dragonpoop::gfx_writelock *gl;
    dragonpoop::shared_obj_guard o;
    dragonpoop::model_loader_ref *lr;
    
    gr = c->getGfx();
    
    lr = 0;
  
    gl = (dragonpoop::gfx_writelock *)o.writeLock( gr, "main" );
    gl->loadModel( "test1", "", "dragon_anim.ms3d", 0, &lr );
    o.unlock();
    main_wait_loader( c, lr );
    delete lr;
/*
    gl = (dragonpoop::gfx_writelock *)o.writeLock( gr, "main" );
    gl->loadModel( "test2", "", "3drt_dragon_low.ms3d", 0, &lr );
    o.unlock();
    main_wait_loader( c, lr );
    delete lr;
*/
    dragonpoop::dpid mid;
    main_pause( c, 5 );
    
    gl = (dragonpoop::gfx_writelock *)o.writeLock( gr, "main" );
    mid = gl->makeModelInstance( "test1", 0 );
    o.unlock();
/*
    int i;
    while( c->isRunning() && i < 2 )
    {
        i++;
        main_pause( c, 1 );
        gl = (dragonpoop::gfx_writelock *)o.writeLock( gr, "main" );
        gl->makeModelInstance( "test1", 0 );
        o.unlock();

        main_pause( c, 3 );
        gl = (dragonpoop::gfx_writelock *)o.writeLock( gr, "main" );
        gl->makeModelInstance( "test2", 0 );
        o.unlock();
    }
  //  main_pause( c, 2 );
*/
    gl = (dragonpoop::gfx_writelock *)o.writeLock( gr, "main" );

    gl->startAnimation( "test1", mid, "gethit", 0, 1 );
    gl->startAnimation( "test1", mid, "walk-1", 1, 1 );

    //gl = (dragonpoop::gfx_writelock *)o.writeLock( gr );
    //gl->saveModel( "test", "", "out_beast.ms3d", 0 );
   // gl->saveModel( "test1", "", "3drt_dragon_high.dpmodel", 0 );
  //  gl->saveModel( "test2", "", "3drt_dragon_low.ms3d", 0 );
 
    o.unlock();
  
    while( c->isRunning() )
        std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );

    delete c;

    return 0;
}
