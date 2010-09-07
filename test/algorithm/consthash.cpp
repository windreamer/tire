#include "algorithm/consthash.hpp"
#include "tut/tut.hpp"
#include "tut/tut_macros.hpp"

namespace
{
    struct data
    {
        int random (int a, int b)
        {
            if (b < a)
            {
                throw std::invalid_argument("b is less than a");
            }
            if ((b-a) > RAND_MAX)
            {
                throw std::range_error("b-a larger than RAND_MAX");
            }
            if(b == a)
            {
                return a;
            }
            double r = rand();
            return (int)(r / RAND_MAX * (b-a) + a);
        }
        
        double random()
        {
            double r = rand();
            return r/RAND_MAX;
        }
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
        ensure_equals("default resource empty", hash.weight(1), 0);
    }
    
    template<>
    template<>
    void fixture::test<2>()
    {
        set_test_name("throw domain_error when hash using empty object");
        algorithm::const_hash hash;
        ensure("default hash empty", hash.empty());
        ensure_THROW(hash.hash(0.5), std::domain_error);
    }

    template<>
    template<>
    void fixture::test<3>()
    {
        set_test_name("test add one node");
        int loop =0;
        algorithm::const_hash hash;
        ensure("default hash empty", hash.empty());
        hash.add(0, 1);
        ensure_not("hash not empty", hash.empty());
        ensure_not("hash alive_set not empty", hash.alive_set().empty());
        ensure_equals("hash alive_set has node 0", 
                hash.alive_set().count(0), 1);
        ensure_equals("hash node 0 weight", hash.weight(0), 1);
        loop = random(1, 100);
        for (int i = 0; i < loop; ++i)
        {
            ensure_equals("hash resource to node 0", 
                    hash.hash(random()), 0);
        }
        hash.add(0, 99);
        ensure_not("hash not empty", hash.empty());
        ensure_not("hash alive_set not empty", hash.alive_set().empty());
        ensure_equals("hash alive_set has node 0", 
                hash.alive_set().count(0), 1);
        ensure_equals("hash node 0 weight", hash.weight(0), 100);
        loop = random(1, 100);
        for (int i = 0; i < loop; ++i)
        {
            ensure_equals("hash resource to node 0", 
                    hash.hash(random()), 0);
        }
        ensure_THROW(hash.hash(2), std::range_error);
        ensure_THROW(hash.add(1, algorithm::const_hash::MAX_NODES-99), std::range_error);
        ensure_equals("node 1 has no weight", hash.weight(1), 0);
        ensure_equals("node 1 not in alive_set", hash.alive_set().count(1), 0);
    }
    
    template<>
    template<>
    void fixture::test<4>()
    {
        set_test_name("test add multiple node");
        algorithm::const_hash hash;
        int count = random (1, 20);
        for (int i = 0; i < count; ++i)
        {
            int weight = random(100, 200);
            hash.add(i, weight);
            ensure_equals("hash weight", hash.weight(i), weight);
        }
        ensure_equals("hash count", hash.alive_set().size(), count);
        int loop = random (200, 300);
        for (int i = 0; i < loop; ++i)
        {
            ensure("node in alive_set", 
                    hash.alive_set().count(hash.hash(random())) == 1); 
        }
    }

    template<>
    template<>
    void fixture::test<5>()
    {
        set_test_name("test remove weight");
        algorithm::const_hash hash;
        hash.add(0, 100);
        ensure_not("hash not empty", hash.empty());
        ensure_not("hash alive_set not empty", hash.alive_set().empty());
        ensure_equals("hash alive_set has one node", 
                hash.alive_set().size(), 1);
        ensure_equals("hash alive_set has node 0", 
                hash.alive_set().count(0), 1);
        ensure_equals("hash node 0 weight", hash.weight(0), 100);

        hash.remove(1, 100);
        ensure_not("hash not empty", hash.empty());
        ensure_not("hash alive_set not empty", hash.alive_set().empty());
        ensure_equals("hash alive_set has one node", 
                hash.alive_set().size(), 1);
        ensure_equals("hash alive_set has node 0", 
                hash.alive_set().count(0), 1);
        ensure_equals("hash node 0 weight", hash.weight(0), 100);

        hash.remove(0, 99);
        ensure_not("hash not empty", hash.empty());
        ensure_not("hash alive_set not empty", hash.alive_set().empty());
        ensure_equals("hash alive_set has one node", 
                hash.alive_set().size(), 1);
        ensure_equals("hash alive_set has node 0", 
                hash.alive_set().count(0), 1);
        ensure_equals("hash node 0 weight", hash.weight(0), 1);

        hash.remove(0, 1);
        ensure("hash empty", hash.empty());
        ensure("hash alive_set empty", hash.alive_set().empty());
        ensure_equals("hash alive_set has no node 0", 
                hash.alive_set().count(0), 0);
        ensure_equals("hash node 0 weight", hash.weight(0), 0);

        hash.add(1, 50);
        hash.add(2, 50);
        ensure_equals("hash alive_set has 2 nodes", 
                hash.alive_set().size(), 2);
        ensure_equals("hash alive_set has no node 0", 
                hash.alive_set().count(0), 0);
        ensure_equals("hash alive_set has node 1", 
                hash.alive_set().count(1), 1);
        ensure_equals("hash alive_set has node 2", 
                hash.alive_set().count(2), 1);
        ensure_equals("hash node 0 weight", hash.weight(0), 0);
        ensure_equals("hash node 1 weight", hash.weight(1), 50);
        ensure_equals("hash node 2 weight", hash.weight(2), 50);

        hash.remove(2, 100);
        ensure_equals("hash alive_set has 1 nodes", 
                hash.alive_set().size(), 1);
        ensure_equals("hash alive_set has no node 0", 
                hash.alive_set().count(0), 0);
        ensure_equals("hash alive_set has node 1", 
                hash.alive_set().count(1), 1);
        ensure_equals("hash alive_set has no node 2", 
                hash.alive_set().count(2), 0);
        ensure_equals("hash node 0 weight", hash.weight(0), 0);
        ensure_equals("hash node 1 weight", hash.weight(1), 50);
        ensure_equals("hash node 2 weight", hash.weight(2), 0);
    }
    
    template<>
    template<>
    void fixture::test<6>()
    {
        set_test_name("test erase");
        algorithm::const_hash hash;
        hash.add(0, 100);
        ensure_not("hash not empty", hash.empty());
        ensure_not("hash alive_set not empty", hash.alive_set().empty());
        ensure_equals("hash alive_set has one node", 
                hash.alive_set().size(), 1);
        ensure_equals("hash alive_set has node 0", 
                hash.alive_set().count(0), 1);
        ensure_equals("hash node 0 weight", hash.weight(0), 100);

        hash.erase(1);
        ensure_not("hash not empty", hash.empty());
        ensure_not("hash alive_set not empty", hash.alive_set().empty());
        ensure_equals("hash alive_set has one node", 
                hash.alive_set().size(), 1);
        ensure_equals("hash alive_set has node 0", 
                hash.alive_set().count(0), 1);

        hash.erase(0);
        ensure("hash empty", hash.empty());
        ensure("hash alive_set empty", hash.alive_set().empty());
        ensure_equals("hash alive_set has no node", 
                hash.alive_set().size(), 0);
        ensure_equals("hash alive_set has no node 0", 
                hash.alive_set().count(0), 0);

        hash.add(1, 50);
        hash.add(2, 50);
        ensure_equals("hash alive_set has 2 nodes", 
                hash.alive_set().size(), 2);
        ensure_equals("hash alive_set has no node 0", 
                hash.alive_set().count(0), 0);
        ensure_equals("hash alive_set has node 1", 
                hash.alive_set().count(1), 1);
        ensure_equals("hash alive_set has node 2", 
                hash.alive_set().count(2), 1);
        ensure_equals("hash node 0 weight", hash.weight(0), 0);
        ensure_equals("hash node 1 weight", hash.weight(1), 50);
        ensure_equals("hash node 2 weight", hash.weight(2), 50);

        hash.erase(2);
        ensure_equals("hash alive_set has 1 nodes", 
                hash.alive_set().size(), 1);
        ensure_equals("hash alive_set has no node 0", 
                hash.alive_set().count(0), 0);
        ensure_equals("hash alive_set has node 1", 
                hash.alive_set().count(1), 1);
        ensure_equals("hash alive_set has node 2", 
                hash.alive_set().count(2), 0);
        ensure_equals("hash node 0 weight", hash.weight(0), 0);
        ensure_equals("hash node 1 weight", hash.weight(1), 50);
        ensure_equals("hash node 2 weight", hash.weight(2), 0);
    }

    template<>
    template<>
    void fixture::test<7>()
    {
        set_test_name("hash algorithm deterministic");
        algorithm::const_hash hash1, hash2;
        int nodes = 100;
        for(int i = 0; i < nodes; ++i)
        {
            hash1.add(i, 200);
        }
        for(int i = 0; i < nodes; ++i)
        {
            hash2.add(i, 200);
        }

        int loop = 10000;
        for(int i= 0; i < loop; ++i)
        {
            double r = random();
            ensure_equals(hash1.hash(r), hash2.hash(r));
        }
    }
}

