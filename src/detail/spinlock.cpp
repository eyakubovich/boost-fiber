
//          Copyright Oliver Kowalke 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/fiber/detail/spinlock.hpp>

#include <boost/assert.hpp>
#include <boost/thread/thread.hpp>

#include <boost/fiber/detail/scheduler.hpp>

namespace boost {
namespace fibers {
namespace detail {

spinlock::spinlock() :
    state_( UNLOCKED)
{}

void
spinlock::lock()
{
    for (;;)
    {
        // access to CPU's cache
        // first access to state_ -> cache miss
        // sucessive acccess to state_ > cache hit
        while ( LOCKED == state_)
        {
            // lock can only be held by another thread
            // give up CPU to other threads to speed up lock's release
            this_thread::yield();
        }
        // state_ was released by other
        // cached copies are invalidated -> cache miss
        // test-and-set over the bus 
        if ( UNLOCKED == state_.exchange( LOCKED) )
            return;
    }
#if 0
    state_t expected = UNLOCKED;
    while ( ! state_.compare_exchange_strong( expected, LOCKED) )
    {
        // busy-wait
        if ( is_fiber)
            scheduler::instance()->yield();
        else
            this_thread::yield();
        expected = UNLOCKED; 
    }
#endif
}

void
spinlock::unlock()
{
    BOOST_ASSERT( LOCKED == state_);

    state_ = UNLOCKED;
}

}}}
