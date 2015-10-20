
#include "dragonpoop/dragonpoop.h"
#include "dragonpoop/renderer/api_stuff/opengl1o5_x11/opengl1o5_x11.h"

int main( int argc, const char * argv[] )
{

    dragonpoop::opengl1o5_x11 *tw;
    dragonpoop::core *c = new dragonpoop::core();
    
    tw = new dragonpoop::opengl1o5_x11( 640, 480, "hello!", c->getMutexMaster() );
    while( tw->isOpen() )
    {
        tw->run();
        std::this_thread::sleep_for( std::chrono::milliseconds( 20 ) );
    }
    delete tw;
    

    while( c->isRunning() )
        std::this_thread::sleep_for( std::chrono::milliseconds( 30 ) );

    delete c;

    return 0;
}
