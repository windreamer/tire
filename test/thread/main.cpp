#include "tut/tut_main.hpp"
#include "tut/tut_reporter.hpp"

#include <iostream>

using namespace tut;
using namespace std;

namespace tut
{
    test_runner_singleton runner;
}

int main (int argc, char** argv)
{
    tut::reporter reporter;
    tut::runner.get().set_callback(&reporter);

    if (tut_main(argc, argv))
    {
        if (reporter.all_ok())
        {
            cout << "test successful." << endl;
            return 0;
        }
        else
        {
            cout << "test failed." << endl;
            return 1;
        }
    }
    else
    {
        return -1;
    }
}
