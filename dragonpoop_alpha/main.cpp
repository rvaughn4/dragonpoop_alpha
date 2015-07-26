
#include "dragonpoop/dragonpoop.h"

int main( int argc, const char * argv[] )
{

    dragonpoop::core *c = new dragonpoop::core();

    while( c->isRunning() )
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

    delete c;

    return 0;
}
