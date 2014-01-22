//          Copyright Oliver Kowalke 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_DETAIL_MAIN_NOTIFIER_H
#define BOOST_FIBERS_DETAIL_MAIN_NOTIFIER_H

#include <boost/atomic.hpp>
#include <boost/config.hpp>
#include <boost/foreach.hpp>

#include <boost/fiber/detail/config.hpp>
#include <boost/fiber/detail/notify.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace boost {
namespace fibers {
namespace detail {

class main_notifier : private context::fcontext_t, public notify
{
public:
    static ptr_t make_pointer( main_notifier & n) {
        ptr_t p( & n);
        intrusive_ptr_add_ref( p.get() );
        return p;
    }

    main_notifier() :
        notify(static_cast<context::fcontext_t*>(this))
    {
		thread_affinity(true);
		set_running();
	}

	~main_notifier()
	{
		BOOST_FOREACH( fss_data_t::value_type & data, fss_data_)
		{ data.second.do_cleanup(); }
	}

    void deallocate_object()
    {}

private:
    main_notifier( main_notifier const&);
    main_notifier & operator=( main_notifier const&);
};

}}}

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif // BOOST_FIBERS_DETAIL_MAIN_NOTIFIER_H
