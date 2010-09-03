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

        virtual void add (int id, int weight)
        {
            if(weight >0)
            {
                id_set.insert(id);
            }

            for (int counter=0; counter<weight;)
            {
                int index = rand();
                if (ring.insert(std::make_pair(index, id)).second)
                {
                    counter++;
                }
            }
        }

        virtual int remove (int id, int weight)
        {
            int size = count(id);

            weight = std::min(weight, size);

            for (int counter = 0; counter< weight;)
            {
                int index = rand();
                ring_type::iterator 
                    begin = ring.lower_bound(index),
                          end = ring.end();
                if (begin == end)
                {
                    begin = end;
                }

                for(ring_type::iterator it = begin; it!=end;
                        ++it)
                {
                    if(it->second == id)
                    {
                        ring.erase(it);
                        --size;
                        break;
                    }
                }
            }
            if(size == 0)
            {
                id_set.erase(id);
            }
            return size;
        }

        virtual void erase (int id)
        {
            for(ring_type::iterator it = ring.begin();
                    it != ring.end();)
            {
                if (it->second == id)
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

        virtual int count (int id) const
        {
            int size = 0;
            for(ring_type::const_iterator it = ring.begin(),
                    end = ring.end(); it != end; ++it)
            {
                if (it->second == id)
                {
                    ++size;
                }
            }
            return size;
        }

        virtual int hash (int resource) const
        {
            if (empty())
            {
                throw std::domain_error ("empty ring.");
            }

            ring_type::const_iterator it = ring.lower_bound(resource);
            if (it == ring.end())
            {
                it == ring.begin();
            }
        }

        virtual bool empty() const
        {
            return ring.empty();
        }

        virtual std::set<int> alive_set () const
        {
            return id_set;
        }
    private:
        typedef std::map<int ,int> ring_type;
        ring_type ring;

        std::set<int> id_set;
    };
}
#endif //__CONST_HASH_H__
