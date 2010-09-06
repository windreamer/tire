#include "algorithm/consthash.hpp"

#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace algorithm;
using namespace std;
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
    return (int)(rand()/(RAND_MAX+1.0)*(b-a)+a);
}

double frandom ()
{
    double r = rand();
    return r/RAND_MAX;
}

int main()
{
    srand(time(NULL));
    const_hash hash;
    const int node_num = 26;
    char name[node_num];
    int count[node_num];
    for (int i=0; i<node_num; ++i)
    {
        name[i]='a'+i;
        hash.add(i, random(100, 200));
    }
    
    const int loop = 10000000;
    int i = 0;
    while (true)
    {
        memset(count, 0, sizeof(count));
        clock_t begin=clock();
        for(int j=0; j<loop; ++j)
        {
            int index = hash.hash(frandom());
            ++count[index];
        }
        int elasped = (int)((clock()-begin)*1000.0/CLOCKS_PER_SEC);
        double average = ((double)elasped)/loop;
        int count_num = (int) (1000 / average);
        cout<< endl <<"process time: total="<< elasped<<
            "ms average="<< average
            << "ms speed=" << count_num  << " per second" <<endl;
        cout<<"node weight: ";
        for(int j=0; j<node_num; ++j)
        {
            cout<<name[j]<<"="<<hash.weight(j)<<" ";
        }
        cout << endl;

        cout<<"node counter: ";
        for(int j=0; j<node_num; ++j)
        {
            cout<<name[j]<<"="<<count[j]<<" ";
        }
        cout << endl;

        cout<<"hit ratio: ";
        for(int j=0; j<node_num; ++j)
        {
            double ratio = (count[j]==0?0:((double)count[j]/hash.weight(j)));
            cout<<name[j]<<"="<<ratio <<" ";
        }
        cout << endl;

        switch(i)
        {
        case 0:
            {
                for(int j=0; j<node_num; ++j)
                {
                    hash.add(random(0,node_num), random(0,50));
                }
            }
            break;
        case 1:
            {
                int loop = random(0, node_num);
                for(int j=0; j<node_num; ++j)
                {
                    hash.remove(random(0,node_num), random(0,50));
                }
            }
            break;
        case 2:
            {
                int index = random(0, node_num);
                hash.erase(index);
            }
            break;
        }
        ++i;
        i%=3;
    }
}
