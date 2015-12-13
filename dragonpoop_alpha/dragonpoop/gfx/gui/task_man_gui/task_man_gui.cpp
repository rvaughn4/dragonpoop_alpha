
#include "task_man_gui.h"
#include "../../../core/shared_obj/shared_obj_guard.h"
#include "../../gfx_writelock.h"
#include "../../../core/core.h"
#include "../../../core/dptaskpool/dptaskpool_logger.h"
#include "../../../core/dptaskpool/dptaskpool_logger_ref.h"
#include "../../../core/dptaskpool/dptaskpool_logger_writelock.h"
#include "../../../core/dptaskpool/dptaskpool_ref.h"
#include "../../../core/dptaskpool/dptaskpool_readlock.h"
#include "../../../core/dpthread/dpthread_lock.h"
#include <sstream>

namespace dragonpoop
{

    //ctor
    task_man_gui::task_man_gui( gfx_writelock *g, dpid id, dpid pid ) : window_gui( g, id, pid, 100, 100, 500, 700, "Tasks" )
    {
        dptaskpool_ref *tpr;
        dptaskpool_readlock *tpl;
        shared_obj_guard o;

        tpr = g->getTasks();
        tpl = (dptaskpool_readlock *)o.tryReadLock( tpr, 100, "task_man_gui::task_man_gui" );
        if( tpl )
            this->rlggr = tpl->getLogger();
        else
            this->rlggr = 0;
        o.unlock();
        delete tpr;

        this->setDraggable( 1 );
        this->lggr = new dptaskpool_logger( g->getCore()->getMutexMaster() );
    }

    //dtor
    task_man_gui::~task_man_gui( void )
    {
        shared_obj_guard o;

        o.tryWriteLock( this, 5000, "task_man_gui::~task_man_gui" );
        o.unlock();
        this->unlink();

        o.tryWriteLock( this, 5000, "task_man_gui::~task_man_gui" );
        delete this->lggr;
        delete this->rlggr;
    }

    //override to do processing
    void task_man_gui::doProcessing( dpthread_lock *thd, gui_writelock *g )
    {
        shared_obj_guard o;
        dptaskpool_logger_writelock *l;
        uint64_t t;
        std::string s, sl;
        dptaskpool_logger_value *vl;
        dptaskpool_logger_value *v;
        unsigned int i, e, lid, lper, r, lc;

        this->window_gui::doProcessing( thd, g );

        t = thd->getTicks();
        if( t - this->lastSync < 500 )
            return;
        this->lastSync = t;

        l = (dptaskpool_logger_writelock *)o.tryWriteLock( this->lggr, 100, "task_man_gui::doProcessing" );
        if( !l )
            return;
        l->sync( this->rlggr );

        lid = 0;
        lper = 0;
        lc = 0;

        sl.assign( "Queued\r\n" );

        e = l->getTaskList( &vl );
        for( i = 0; i < e; i++ )
        {
            std::stringstream ss;

            v = &vl[ i ];

            if( v->tid != lid )
            {
                if( lper > 100 )
                    lper = 100;
                r = lper;
                if( r < 50 )
                    ss << "\a000255000";
                else
                {
                    if( r < 90 )
                        ss << "\a255255000";
                    else
                        ss << "\a255000000";
                }
                ss << "\t\t" << lc << "\t\t\t" << lper << "%\r\n\a000000000";
                ss << "Thread " << v->tid << "\r\n";
                lid = v->tid;
                lper = 0;
                lc = 0;
            }

            if( v->isStatic )
                ss << "\a000000255";
            ss << "\t" << v->name << "\t\t\t";
            r = v->percent_usage;
            if( r < 50 )
                ss << "\a000255000";
            else
            {
                if( r < 90 )
                    ss << "\a255255000";
                else
                    ss << "\a255000000";
            }
            ss << v->percent_usage << "%\r\n\a000000000";
            lper += v->percent_usage;
            lc++;

            if( i + 1 >= e )
            {
                if( lper > 100 )
                    lper = 100;
                r = lper;
                if( r < 50 )
                    ss << "\a000255000";
                else
                {
                    if( r < 90 )
                        ss << "\a255255000";
                    else
                        ss << "\a255000000";
                }
                ss << "\t\t" << lc << "\t\t\t" << lper << "%\r\n\a000000000";
                ss << l->getTime() << "";
            }

            sl.append( ss.str() );
        }

        this->setText( &sl );
        this->redraw();
    }

};
