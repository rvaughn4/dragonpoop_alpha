
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
    gl->loadModel( "low_dragon", "", "3drt_dragon_low.dpmodel", 0, &lr );
    o.unlock();
    main_wait_loader( c, lr );
    delete lr;

    gl = (dragonpoop::gfx_writelock *)o.writeLock( gr, "main" );
    gl->loadModel( "med_dragon", "", "3drt_dragon_medium.dpmodel", 0, &lr );
    o.unlock();
    main_wait_loader( c, lr );
    delete lr;

    gl = (dragonpoop::gfx_writelock *)o.writeLock( gr, "main" );
    gl->loadModel( "high_dragon", "", "3drt_dragon_high.dpmodel", 0, &lr );
    o.unlock();
    main_wait_loader( c, lr );
    delete lr;

    dragonpoop::dpid mid;
    main_pause( c, 5 );
    
    gl = (dragonpoop::gfx_writelock *)o.writeLock( gr, "main" );
    mid = gl->makeModelInstance( "low_dragon", 0 );
    gl->startAnimation( "low_dragon", mid, "gethit", 1, 5 );
    o.unlock();
    
    gl = (dragonpoop::gfx_writelock *)o.writeLock( gr, "main" );
    mid = gl->makeModelInstance( "med_dragon", 0 );
    gl->startAnimation( "med_dragon", mid, "run", 1, 0.2f );
    o.unlock();
    
    gl = (dragonpoop::gfx_writelock *)o.writeLock( gr, "main" );
    mid = gl->makeModelInstance( "high_dragon", 0 );
    gl->startAnimation( "high_dragon", mid, "flying idle", 1, 2 );
    o.unlock();
    

    gl = (dragonpoop::gfx_writelock *)o.writeLock( gr, "main" );

    //gl->saveModel( "low_dragon", "", "3drt_dragon_low.ms3d", 0 );
    //gl->saveModel( "med_dragon", "", "3drt_dragon_medium.ms3d", 0 );
    //gl->saveModel( "high_dragon", "", "3drt_dragon_high.ms3d", 0 );
 
    o.unlock();
  
    while( c->isRunning() )
        std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );

    delete c;

    return 0;
}
