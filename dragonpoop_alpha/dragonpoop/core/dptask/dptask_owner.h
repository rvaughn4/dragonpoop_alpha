
#ifndef dragonpoop_dptask_owner_h
#define dragonpoop_dptask_owner_h

namespace dragonpoop
{

    class dpthread_lock;
    class dptask_writelock;

    class dptask_owner
    {

    private:

    protected:

    public:

        //ctor
        dptask_owner( void );
        //dtor
        virtual ~dptask_owner( void );
        //run by task
        virtual void run( dptask_writelock *tl, dpthread_lock *th ) = 0;

    };
    
};

#endif