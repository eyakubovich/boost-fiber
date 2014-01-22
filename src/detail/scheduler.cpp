//          Copyright Oliver Kowalke 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/fiber/detail/scheduler.hpp"

#include <boost/assert.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace boost {
namespace fibers {
namespace detail {

static void deleter_fn( fiber_manager* mgr) { delete mgr; }

thread_specific_ptr< fiber_manager> scheduler::instance_( deleter_fn);

fiber_manager*
scheduler::instance()
{
    if ( ! instance_.get() )
    {
        instance_.reset( new fiber_manager() );
    }
    return instance_.get();
}

void
scheduler::replace( sched_algorithm * other)
{
    BOOST_ASSERT( other);

    instance()->set_sched_algo( other);
}

}}}

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif
