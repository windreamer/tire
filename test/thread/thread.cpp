#include "thread/pthreadxx.hpp"
#include "tut/tut.hpp"
#include "tut/tut_macros.hpp"

namespace
{
    struct data
    {
        void* ret; 
        data(void* ret_= NULL):
            ret(ret_)
        {
        }

        void* operator () () 
        {
            if (ret == reinterpret_cast<void*>(-2))
            {
                ret = reinterpret_cast<void*>(
                        pthreadxx::thread::current().id());
            }
            return ret;
        }
    };
    typedef tut::test_group<data> group;
    group g("thread");

    typedef group::object fixture;
}

namespace tut
{
    template<>
    template<>
    void fixture::test<1>()
    {
        set_test_name("default constructor");
        pthreadxx::thread t;
        ensure_not("default thread is invalid", t.valid());
    }

    template<>
    template<>
    void fixture::test<2>()
    {
        data d;
        set_test_name("create thread");
        ensure("thread create successful", 
                pthreadxx::thread::create(d).valid());
    }

    template<>
    template<>
    void fixture::test<3>()
    {
        data d;
        set_test_name("equal operator");
        pthreadxx::thread t1, t2;
        ensure("invalid threads are equal", t1 == t2);
        ensure_not("invalid threads are not unequal", t1 != t2);
        ensure("self compare are equal", t1 == t1);
        ensure_not("self compare are not unequal", t1 != t1);
        t1=t1;
        ensure("self compare are equal", t1 == t1);
        ensure_not("self compare are not unequal", t1 != t1);
        t1 = pthreadxx::thread::create(d);
        ensure_not("threads are unequal", t1 == t2);
        ensure("invalid threads are not equal", t1 != t2);
        t2 = pthreadxx::thread::create(d);
        ensure_not("threads are unequal", t1 == t2);
        ensure("invalid threads are not equal", t1 != t2);
    }

    template<>
    template<>
    void fixture::test<4>()
    {
        void* result = reinterpret_cast<void*>(-1);
        data d(result);
        set_test_name("join thread");
        pthreadxx::thread t = pthreadxx::thread::create(d);
        ensure_equals("join return", t.join(), result);
        pthreadxx::thread t1;
        ensure_THROW(t1.join(), pthreadxx::invalid_state);
        ensure_THROW(t.join(), std::invalid_argument);
    }

    template<>
    template<>
    void fixture::test<5>()
    {
        data d(reinterpret_cast<void*>(-2));
        set_test_name("get current");
        pthreadxx::thread t = pthreadxx::thread::create(d);
        ensure_equals("same id", 
                reinterpret_cast<pthread_t>(t.join()), t.id());
    }

    template<>
    template<>
    void fixture::test<6>()
    {
        set_test_name("detach thread");
        pthreadxx::thread t = pthreadxx::thread::create(data());
        ensure_NO_THROW(t.detach());
        pthreadxx::thread t1;
        ensure_THROW(t.detach(), pthreadxx::invalid_state);
    }
}

