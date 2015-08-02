
#include "dragonpoop/dragonpoop.h"

int main( int argc, const char * argv[] )
{

    dragonpoop::core *c = new dragonpoop::core();
    
    dragonpoop::gfx_ref *gr;
    dragonpoop::gfx_writelock *gl;
    dragonpoop::shared_obj_guard o;
    dragonpoop::dpthread *t;
    dragonpoop::dpthread_lock *thd;
    
    gr = c->getGfx();
    gl = (dragonpoop::gfx_writelock *)o.writeLock( gr );
    
    t = new dragonpoop::dpthread( c->getMutexMaster(), 256 );
    thd = t->lock();
    
    gl->loadModel( thd, "test", "felhound_hi_milkshape.ms3d", 0, 0 );
    
    delete thd;
    delete t;
    
    o.unlock();
    
    

    while( c->isRunning() )
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

    delete c;

    return 0;
}
