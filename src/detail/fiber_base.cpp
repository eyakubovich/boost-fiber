
//          Copyright Oliver Kowalke 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/fiber/detail/fiber_base.hpp"

#include <exception>

#include <boost/exception_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/thread/locks.hpp>

#include "boost/fiber/detail/scheduler.hpp"
#include <boost/fiber/exceptions.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace boost {
namespace fibers {
namespace detail {

void
fiber_base::set_terminated_() BOOST_NOEXCEPT
{
    state_t previous = state_.exchange( TERMINATED);
    BOOST_ASSERT( RUNNING == previous);
}

void
fiber_base::entry_thunk(intptr_t param) {
    fiber_base* me = static_cast<fiber_base*>(reinterpret_cast<notify*>(param));
    me->entry_func();
}

void
fiber_base::entry_func() {
    try {
        BOOST_ASSERT( is_running() );
        run();
        BOOST_ASSERT( is_running() );
    }
/*    catch ( coro::detail::forced_unwind const&)
    {
        set_terminated_();
        release();
        throw;
    }
*/    catch( fiber_interrupted const& ) {
        except_ = current_exception();
    }
    catch( ... ) {
        std::terminate();
    }
    set_terminated_();
    release();

    fibers::detail::scheduler::instance()->run();

    BOOST_ASSERT_MSG( false, "fiber already terminated");
}

fiber_base::~fiber_base()
{
    BOOST_ASSERT( is_terminated() );
    BOOST_ASSERT( waiting_.empty() );
}

void
fiber_base::release()
{
    BOOST_ASSERT( is_terminated() );

    std::vector< notify::ptr_t > waiting;

    // get all waiting fibers
    splk_.lock();
    waiting.swap( waiting_);
    splk_.unlock();

    // notify all waiting fibers
    BOOST_FOREACH( notify::ptr_t p, waiting)
    { p->set_ready(); }

    // release all fiber-specific-pointers
    BOOST_FOREACH( fss_data_t::value_type & data, fss_data_)
    { data.second.do_cleanup(); }
}

bool
fiber_base::join( notify::ptr_t const& p)
{
    unique_lock< spinlock > lk( splk_);
    if ( is_terminated() ) return false;
    waiting_.push_back( p);
    return true;
}

void
fiber_base::rethrow() const
{
    BOOST_ASSERT( has_exception() );

    rethrow_exception( except_);
}

}}}

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif
