
//          Copyright Oliver Kowalke 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FIBERS_DETAIL_FIBER_BASE_H
#define BOOST_FIBERS_DETAIL_FIBER_BASE_H

#include <cstddef>
#include <map>
#include <vector>

#include <boost/assert.hpp>
#include <boost/atomic.hpp>
#include <boost/bind.hpp>
#include <boost/config.hpp>
#include <boost/coroutine/all.hpp>
#include <boost/cstdint.hpp>
#include <boost/exception_ptr.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/move/move.hpp>
#include <boost/utility.hpp>

#include <boost/fiber/attributes.hpp>
#include <boost/fiber/detail/config.hpp>
#include <boost/fiber/detail/notify.hpp>
#include <boost/fiber/detail/spinlock.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

# if defined(BOOST_MSVC)
# pragma warning(push)
# pragma warning(disable:4251)
# endif

namespace boost {
namespace fibers {
namespace detail {

namespace coro = boost::coroutines;

class BOOST_FIBERS_DECL fiber_base : public notify
{
public:
    typedef intrusive_ptr< fiber_base >     ptr_t;

private:
    // set terminate is only set inside fiber_base::trampoline_()
    void set_terminated_() BOOST_NOEXCEPT;

	static void entry_thunk(intptr_t param);
	void entry_func();

protected:
    exception_ptr                           except_;
    spinlock                                splk_;
    std::vector< notify::ptr_t >            waiting_;

    void release();

    virtual void run() = 0;

public:
    fiber_base( coro::stack_context const& sctx) :
        notify(context::make_fcontext(sctx.sp, sctx.size, &fiber_base::entry_thunk)),
        except_(),
        waiting_()
    {
        set_ready(); // fiber is setup and now ready to run
    }

    virtual ~fiber_base();

    bool join( notify::ptr_t const&);

    bool has_exception() const BOOST_NOEXCEPT
    { return except_; }

    void rethrow() const;
};

}}}

# if defined(BOOST_MSVC)
# pragma warning(pop)
# endif

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif // BOOST_FIBERS_DETAIL_FIBER_BASE_H
