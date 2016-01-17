
#ifndef dragonpoop_input_passer_readlock_h
#define dragonpoop_input_passer_readlock_h

#include "../../../core/shared_obj/shared_obj_readlock.h"

namespace dragonpoop
{
    class input_passer;
    class dpmatrix;

    class input_passer_readlock : public shared_obj_readlock
    {

    private:

        input_passer *t;

    protected:

        //ctor
        input_passer_readlock( input_passer *t, dpmutex_readlock *l );
        //dtor
        virtual ~input_passer_readlock( void );

    public:

        //get position time
        uint64_t getTime( void );
        //returns true if has input
        bool hasInput( void );
        //returns true if has mouse input
        bool hasMouseInput( void );
        //returns true if has keyboard input
        bool hasKeyboardInput( void );
        //get matrix
        void getMatrix( dpmatrix *m );

        friend class input_passer;
    };

};

#endif
