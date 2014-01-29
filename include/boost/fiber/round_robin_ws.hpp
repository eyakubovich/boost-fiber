//          Copyright Oliver Kowalke 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_DEFAULT_SCHEDULER_WS_H
#define BOOST_FIBERS_DEFAULT_SCHEDULER_WS_H

#include <deque>

#include <boost/assert.hpp>
#include <boost/config.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/thread/lock_types.hpp> 

#include <boost/fiber/fiber_manager.hpp>
#include <boost/fiber/detail/config.hpp>
#include <boost/fiber/detail/fiber_base.hpp>
#include <boost/fiber/detail/main_notifier.hpp>
#include <boost/fiber/detail/notify.hpp>
#include <boost/fiber/detail/spinlock.hpp>
#include <boost/fiber/detail/ws_queue.hpp>
#include <boost/fiber/fiber.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

# if defined(BOOST_MSVC)
# pragma warning(push)
# pragma warning(disable:4251 4275)
# endif

namespace boost {
namespace fibers {

class BOOST_FIBERS_DECL round_robin_ws : public sched_algorithm
{
private:
    detail::ws_queue            rqueue_;

public:
    round_robin_ws() BOOST_NOEXCEPT;

    ~round_robin_ws() BOOST_NOEXCEPT;

    virtual void awakened( detail::notify::ptr_t const& fib);
    virtual detail::notify::ptr_t pick_next();
    virtual void priority( detail::notify::ptr_t const&, int) BOOST_NOEXCEPT;

    fiber steal_from();
    void migrate_to( fiber const&);
};

}}

# if defined(BOOST_MSVC)
# pragma warning(pop)
# endif

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif // BOOST_FIBERS_DEFAULT_SCHEDULER_WS_H
