
#include "dragonpoop/dragonpoop.h"

int main( int argc, const char * argv[] )
{

    dragonpoop::core *c = new dragonpoop::core();
    
    dragonpoop::gfx_ref *gr;
    dragonpoop::gfx_writelock *gl;
    dragonpoop::shared_obj_guard o;
    dragonpoop::model_instance_ref *m;
   // dragonpoop::model_loader_ref *lr;
   // dragonpoop::model_loader_readlock *lw;
//    int i;
    
    gr = c->getGfx();
    gl = (dragonpoop::gfx_writelock *)o.writeLock( gr );

    gl->loadModel( "test1", "", "beast.dpmodel", 0, 0/*&lr*/ );
    gl->loadModel( "test2", "", "felhound_hi_milkshape.dpmodel", 0, 0/*&lr*/ );
    
    m = gl->makeModelInstance( "test1" );
    delete m;
    m = gl->makeModelInstance( "test2" );
    delete m;
    o.unlock();

    //i = 0;
   // while( c->isRunning() && i < 500 )
    //{
      //  std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );
        //if( !lr->isLinked() )
          //  i = 5000;
        //lw = (dragonpoop::model_loader_readlock *)o.tryReadLock( lr, 100 );
        //if( lw )
          // if( !lw->isRunning() )
            //    i = 5000;
        //o.unlock();
        //i++;
   // }

    //gl = (dragonpoop::gfx_writelock *)o.writeLock( gr );
    //gl->saveModel( "test", "", "out_beast.ms3d", 0 );
   // gl->saveModel( "test", "", "beast.dpmodel", 0 );
 
   // o.unlock();
  
    while( c->isRunning() )
        std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );

    delete c;

    return 0;
}
