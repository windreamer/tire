#include "algorithm/consthash.hpp"
#include "tut/tut.hpp"
#include "tut/tut_macros.hpp"

namespace
{
    struct data
    {
    };
    typedef tut::test_group<data> group;
    group g("const_hash");

    typedef group::object fixture;
}

namespace tut
{
    template<>
    template<>
    void fixture::test<1>()
    {
        set_test_name("construct object");
        algorithm::const_hash hash;
        ensure("default hash empty", hash.empty());
        ensure("default alive_set empty", hash.alive_set().empty());
        ensure_equals("default resource empty", hash.count(1), 0);
    }
    
    template<>
    template<>
    void fixture::test<2>()
    {
        set_test_name("throw domain_error when hash using empty object");
        algorithm::const_hash hash;
        ensure("default hash empty", hash.empty());
        ensure_THROW(hash.hash(10), std::domain_error);
    }
}

