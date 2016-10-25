#include <string>
#include <iostream>

#ifdef vxWorks
#  include <boost/tr1/memory.hpp>

#elif defined(__clang__)
#  include <memory>
namespace std {
    namespace tr1 {
        using std::shared_ptr;
    }
}

#elif defined(_MSC_VER) && (_MSC_VER>=1600)
#  include <memory>

#else
#  include <tr1/memory>
#endif

#include <testMain.h>

MAIN(testShared)
{
    std::tr1::shared_ptr<double> spd(new double);
    *spd = 1.23456789;

    std::cout << "Apparently it works..." << std::endl;

    return 0;
}
