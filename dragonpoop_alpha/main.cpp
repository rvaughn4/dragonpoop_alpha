
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

        if( !lr->isLinked() )
        {
            b = 0;
            continue;
        }
        lw = (dragonpoop::model_loader_readlock *)o.tryReadLock( lr, 100 );
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
    dragonpoop::model_instance_ref *m;
    dragonpoop::model_loader_ref *lr;
    
    gr = c->getGfx();
    gl = (dragonpoop::gfx_writelock *)o.writeLock( gr );

    gl->loadModel( "test1", "", "felhound_hi_milkshape.ms3d", 0, &lr );
  //  main_wait_loader( c, lr );
    delete lr;
    
    gl->loadModel( "test2", "", "felhound_hi_milkshape.dpmodel", 0, &lr );
    main_wait_loader( c, lr );
    delete lr;

    main_pause( c, 2 );
    
    m = gl->makeModelInstance( "test1" );
    delete m;

    main_pause( c, 2 );
    
    m = gl->makeModelInstance( "test2" );
    delete m;
    o.unlock();


    //gl = (dragonpoop::gfx_writelock *)o.writeLock( gr );
    //gl->saveModel( "test", "", "out_beast.ms3d", 0 );
   // gl->saveModel( "test", "", "beast.dpmodel", 0 );
 
   // o.unlock();
  
    while( c->isRunning() )
        std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );

    delete c;

    return 0;
}
