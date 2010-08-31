#include "thread/pthreadxx.hpp"
#include "tut/tut.hpp"
#include "tut/tut_macros.hpp"

namespace 
{ 
    struct data
    { 
    };

    typedef tut::test_group<data> group;
    group g("thread_attribute");

    typedef group::object fixture;
}

namespace tut
{
    template<> 
    template<> 
    void fixture::test<1>() 
    {
        set_test_name("detach_state");

        pthreadxx::thread_attribute attribute;
        ensure_equals("default state", attribute.detach_state(),
                PTHREAD_CREATE_JOINABLE);

        attribute.detach_state(PTHREAD_CREATE_DETACHED);
        ensure_equals("set detached state", attribute.detach_state(),
                PTHREAD_CREATE_DETACHED);

        attribute.detach_state(PTHREAD_CREATE_JOINABLE);
        ensure_equals("set joinable state", attribute.detach_state(),
                PTHREAD_CREATE_JOINABLE);
    }

    template<>
    template<>
    void fixture::test<2>()
    {
        set_test_name("guard_size");

        pthreadxx::thread_attribute attribute;

        attribute.guard_size(0);
        ensure_equals("no guard", attribute.guard_size(), 0);

        attribute.guard_size(4*1024);
        ensure_equals("set 4K guard_size", attribute.guard_size(), 4*1024);
    }

    template<>
    template<>
    void fixture::test<3>()
    {
        set_test_name("stack_size");
        pthreadxx::thread_attribute attribute;

        attribute.stack_size(PTHREAD_STACK_MIN*10);
        ensure_equals("set 10 times minimal stack size", attribute.stack_size(), PTHREAD_STACK_MIN*10);
        
        ensure_THROW(attribute.stack_size(PTHREAD_STACK_MIN-1), std::invalid_argument);
    }

}; 

