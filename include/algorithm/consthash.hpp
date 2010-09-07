#ifndef __CONST_HASH_H__
#define __CONST_HASH_H__
#include <stdexcept>
#include <map>
#include <set>
namespace algorithm
{
    class const_hash
    {
    public:
        const_hash(){}
        virtual ~const_hash(){}

        virtual void add(int id, int w)
        {
            if((w + ring.size()) >= MAX_NODES)
            {
                throw std::range_error("too many nodes");
            }

            if(w > 0)
            {
                id_set.insert(id);
            }

            int current_weight = weight(id);
            for(int counter = 0; counter < w;)
            {
                double index = random(id, current_weight + counter);
                if(ring.insert(std::make_pair(index, id)).second)
                {
                    counter++;
                }
            }
        }

        virtual int remove(int id, int w)
        {
            int current_weight = weight(id);

            w = std::min(w, current_weight);

            for(int counter = 0; counter < w;)
            {
                double index = random(id, current_weight - 1);
                ring_type::iterator 
                    begin = ring.lower_bound(index),
                          end = ring.end();
                if(begin == end)
                {
                    begin = ring.begin();
                }

                for(ring_type::iterator it = begin; it!=end;
                        ++it)
                {
                    if(it->second == id)
                    {
                        ring.erase(it);
                        --current_weight;
                        ++counter;
                        break;
                    }
                }
            }
            if(current_weight == 0)
            {
                id_set.erase(id);
            }
            return current_weight;
        }

        virtual void erase(int id)
        {
            for(ring_type::iterator it = ring.begin();
                    it != ring.end();)
            {
                if(it->second == id)
                {
                    ring_type::iterator to_erase = it++;
                    ring.erase(to_erase);
                }
                else
                {
                    ++it;
                }
            }
            id_set.erase(id);
        }

        virtual int weight(int id) const
        {
            int weight = 0;
            for(ring_type::const_iterator it = ring.begin(),
                    end = ring.end(); it != end; ++it)
            {
                if(it->second == id)
                {
                    ++weight;
                }
            }
            return weight;
        }

        virtual int hash(double resource) const
        {
            if(resource < 0 || resource > 1)
            {
                throw std::range_error("resource should be between 0" 
                        "and 1.");
            }

            if(empty())
            {
                throw std::domain_error("empty ring.");
            }

            ring_type::const_iterator it = ring.lower_bound(resource);
            if(it == ring.end())
            {
                it = ring.begin();
            }
            return it->second;
        }

        virtual bool empty() const
        {
            return ring.empty();
        }

        virtual std::set<int> alive_set() const
        {
            return id_set;
        }

        const static int MAX_NODES = 0x7FFFFFFF;

    protected:
        virtual double random(int x, int y)
        {
            unsigned int a = x * 123456789 + y;
            a -= (a<<6);
            a ^= (a>>17);
            a -= (a<<9);
            a ^= (a<<4);
            a -= (a<<3);
            a ^= (a<<10);
            a ^= (a>>15);
            double r = a % MAX_NODES;
            return r/MAX_NODES;
        }
    private:
        typedef std::map<double ,int> ring_type;
        ring_type ring;

        std::set<int> id_set;
    };
}
#endif //__CONST_HASH_H__
