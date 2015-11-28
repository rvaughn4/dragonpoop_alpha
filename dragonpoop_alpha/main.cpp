
#include "dragonpoop/dragonpoop.h"
#include <iostream>

int main( int argc, const char * argv[] )
{

    dragonpoop::core *c = new dragonpoop::core();

    dragonpoop::gfx_ref *g;
    dragonpoop::gfx_readlock *gl;
    dragonpoop::shared_obj_guard o;
    dragonpoop::model_man_ref *m;
    dragonpoop::model_man_writelock *ml;
    dragonpoop::model_loader_ref *mlr;
    dragonpoop::model_saver_ref *msr;

    g = c->getGfx();
    gl = (dragonpoop::gfx_readlock *)o.tryReadLock( g, 100, "" );
    if( gl )
        gl->getModels( &m );
    o.unlock();
    delete g;

    while( c->isRunning() )
        std::this_thread::sleep_for( std::chrono::milliseconds( 30 ) );
/*
    ml = (dragonpoop::model_man_writelock *)o.tryWriteLock( m, 100, "" );
    if( ml )
        ml->loadModel( "t", "", "dragon_anim.ms3d", 0, &mlr );
    o.unlock();

    while( mlr->isLinked() )
        std::this_thread::sleep_for( std::chrono::milliseconds( 30 ) );
    delete mlr;
    std::cout << "model loaded!\r\n";

    ml = (dragonpoop::model_man_writelock *)o.tryWriteLock( m, 100, "" );
    if( ml )
        ml->saveModel( "t", "", "3drt_dragon_high.dpmodel", &msr );
    o.unlock();

    while( msr->isLinked() )
        std::this_thread::sleep_for( std::chrono::milliseconds( 30 ) );
    delete msr;
    std::cout << "model saved!\r\n";
*/

    delete m;

    delete c;

    return 0;
}
