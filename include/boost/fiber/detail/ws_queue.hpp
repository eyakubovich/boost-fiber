
//          Copyright Oliver Kowalke 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_DETAIL_WS_QUEUE_H
#define BOOST_FIBERS_DETAIL_WS_QUEUE_H

#include <deque>

#include <boost/config.hpp>
#include <boost/thread/lock_types.hpp> 

#include <boost/fiber/detail/config.hpp>
#include <boost/fiber/detail/notify.hpp>
#include <boost/fiber/detail/spinlock.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace boost {
namespace fibers {
namespace detail {

class ws_queue
{
private:
    typedef std::deque< notify::ptr_t >     queue_t;

    spinlock    splk_;
    queue_t     queue_;

public:
    ws_queue() :
        splk_(),
        queue_()
    {}

    void push( notify::ptr_t const& f)
    {
        unique_lock< spinlock > lk( splk_);
        queue_.push_back( f);
    }

    bool try_pop( notify::ptr_t & f)
    {
        unique_lock< spinlock > lk( splk_);
        queue_t::iterator e = queue_.end();
        for ( queue_t::iterator i = queue_.begin(); i != e; ++i)
        {
            if ( ! ( * i)->thread_affinity() )
            {
                f.swap( * i);
                queue_.erase( i);
                return true;
            }
        }
        return false;
    }
};

}}}

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif //  BOOST_FIBERS_DETAIL_WS_QUEUE_H
