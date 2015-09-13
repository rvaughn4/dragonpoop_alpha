
#include "dragonpoop/dragonpoop.h"

int main( int argc, const char * argv[] )
{

    dragonpoop::core *c = new dragonpoop::core();
    
    dragonpoop::gfx_ref *gr;
    dragonpoop::gfx_writelock *gl;
    dragonpoop::shared_obj_guard o;
    dragonpoop::model_instance_ref *m;
    
    gr = c->getGfx();
    gl = (dragonpoop::gfx_writelock *)o.writeLock( gr );

    int i = 0;
    while( c->isRunning() && i < 3 )
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
        i++;
    }
 
    gl->loadModel( "test", "", "beast.ms3d", 0, 0 );//felhound_hi_milkshape.ms3d
    //beast.ms3d
    m = gl->makeModelInstance( "test" );

    i = 0;
    while( c->isRunning() && i < 5 )
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );
        i++;
    }

    gl = (dragonpoop::gfx_writelock *)o.writeLock( gr );
    gl->saveModel( "test", "", "out_beast.ms3d", 0 );
    
    o.unlock();
    
    while( c->isRunning() )
        std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );

    delete m;
    delete c;

    return 0;
}
