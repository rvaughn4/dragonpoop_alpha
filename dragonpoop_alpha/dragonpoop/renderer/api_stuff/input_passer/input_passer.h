
#ifndef dragonpoop_input_passer_h
#define dragonpoop_input_passer_h

#include "../../../core/shared_obj/shared_obj.h"
#include "../../../gfx/dpmatrix/dpmatrix.h"
#include <atomic>
#include <memory>
#include <string>

namespace dragonpoop
{

    class input_passer_ref;

    struct input_passer_kb_input
    {
        char key[ 256 ];
        bool bDown;
        bool bAlive;
    };

    struct input_passer_mouse_input
    {
        int x, y;
        bool isLb, isRb;
        bool bDown;
        bool bAlive;
    };

    #define input_passer_MAX_INPUT 255

    class input_passer : public shared_obj
    {

    private:

        std::shared_ptr<std::atomic<uint64_t>> t;
        dpmatrix m;

        struct
        {
            struct
            {
                input_passer_kb_input values[ input_passer_MAX_INPUT ];
                unsigned int cnt, push_index, pop_index;
            } keyb;
            struct
            {
                input_passer_mouse_input values[ input_passer_MAX_INPUT ];
                unsigned int cnt, push_index, pop_index;
            } mouse;
        } input;

        //attempts to push keyboard input at current index, increments index
        bool _pushKBOnce( std::string *sname, bool bDown );
        //attempts to push mouse input at current index, increments index
        bool _pushMouseOnce( int x, int y, bool bLb, bool bRb, bool bDown );
        //calls pushkbonce until unused array index found or max tries
        bool _pushKB( std::string *sname, bool bDown );
        //calls pushmouseonce until unused array index found or max tries
        bool _pushMouse( int x, int y, bool bLb, bool bRb, bool bDown );
        //attempts to pop keyboard input at current index, increments index
        bool _popKBOnce( std::string *sname, bool *bDown );
        //attempts to pop mouse input at current index, increments index
        bool _popMouseOnce( int *x, int *y, bool *bLb, bool *bRb, bool *bDown );
        //calls popkbonce until used array index found or max tries
        bool _popKB( std::string *sname, bool *bDown );
        //calls popmouseonce until used array index found or max tries
        bool _popMouse( int *x, int *y, bool *bLb, bool *bRb, bool *bDown );

    protected:

        //generate write lock
        virtual shared_obj_writelock *genWriteLock( shared_obj *p, dpmutex_writelock *l );
        //generate read lock
        virtual shared_obj_readlock *genReadLock( shared_obj *p, dpmutex_readlock *l );
        //generate ref
        virtual shared_obj_ref *genRef( shared_obj *p, std::shared_ptr<shared_obj_refkernal> *k );
        //get position time
        uint64_t getTime( void );
        //set time
        void setTime( uint64_t t );
        //sync
        void sync( input_passer_ref *r );
        //returns true if has input
        bool hasInput( void );
        //returns true if has mouse input
        bool hasMouseInput( void );
        //returns true if has keyboard input
        bool hasKeyboardInput( void );
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
        //set matrix
        void setMatrix( dpmatrix *m );
        //get matrix
        void getMatrix( dpmatrix *m );

    public:

        //ctor
        input_passer( dpmutex_master *mm );
        //dtor
        virtual ~input_passer( void );

        friend class input_passer_writelock;
        friend class input_passer_readlock;
        friend class input_passer_ref;

    };

};

#endif
