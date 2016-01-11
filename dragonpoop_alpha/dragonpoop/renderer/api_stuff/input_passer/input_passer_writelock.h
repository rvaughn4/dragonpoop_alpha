
#ifndef dragonpoop_input_passer_writelock_h
#define dragonpoop_input_passer_writelock_h

#include "../../../core/shared_obj/shared_obj_writelock.h"
#include <string>

namespace dragonpoop
{
    class input_passer;
    class input_passer_ref;

    class input_passer_writelock : public shared_obj_writelock
    {

    private:

        input_passer *t;

    protected:

        //ctor
        input_passer_writelock( input_passer *t, dpmutex_writelock *l );
        //dtor
        virtual ~input_passer_writelock( void );

    public:

        //get position time
        uint64_t getTime( void );
        //returns true if has input
        bool hasInput( void );
        //returns true if has mouse input
        bool hasMouseInput( void );
        //returns true if has keyboard input
        bool hasKeyboardInput( void );
        //set time
        void setTime( uint64_t t );
        //sync
        void sync( input_passer_ref *r );
        //retrieve keyboard input
        bool getKeyboardInput( std::string *sname, bool *bIsDown );
        //retrieve mouse input
        bool getMouseInput( int *px, int *py, bool *bIsLeft, bool *bIsRight, bool *bIsDown );
        //add keyboard input
        bool addKeyboardInput( std::string *sname, bool bIsDown );
        //add mouse input
        bool addMouseInput( int x, int y, bool bIsLeft, bool bIsRight, bool bIsDown );
        //clear all input
        void clear( void );

        friend class input_passer;
    };

};

#endif
