
//          Copyright Oliver Kowalke 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/fiber/round_robin_ws.hpp"

#include <algorithm>
#include <cmath>
#include <memory>
#include <utility>

#include <boost/assert.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/scope_exit.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/thread.hpp>

#include "boost/fiber/detail/scheduler.hpp"
#include "boost/fiber/exceptions.hpp"

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace boost {
namespace fibers {

round_robin_ws::round_robin_ws() BOOST_NOEXCEPT :
    rqueue_()
{}

round_robin_ws::~round_robin_ws() BOOST_NOEXCEPT
{
    // fibers will be destroyed (stack-unwinding)
    // if last reference goes out-of-scope
    // therefore destructing wqueue_ && rqueue_
    // will destroy the fibers in this scheduler
    // if not referenced on other places
}

void
round_robin_ws::awakened( detail::notify::ptr_t const& fib)
{
    rqueue_.push(fib);
}

detail::notify::ptr_t
round_robin_ws::pick_next()
{
    detail::notify::ptr_t victim;
    rqueue_.try_pop(victim);
    return victim;
}

void
round_robin_ws::priority( detail::notify::ptr_t const& f, int prio) BOOST_NOEXCEPT
{
    BOOST_ASSERT( f);

    // set only priority to fiber
    // round-robin does not respect priorities
    f->priority( prio);
}

fiber
round_robin_ws::steal_from()
{
    detail::notify::ptr_t n;
    if ( rqueue_.try_pop( n) ) {
        // ugly but safe as try_pop won't return main_notifier
        // b/c it has thread affinity set
        detail::fiber_base::ptr_t f(static_cast<detail::fiber_base*>(n.get()));
        return fiber( f);
    }
    return fiber();
}

}}

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif
