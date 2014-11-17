#include <boost/atomic.hpp>
#include <boost/tr1/memory.hpp>
#include <boost/tr1/unordered_map.hpp>
#include <string>

#include <iostream>

typedef std::tr1::unordered_map<int, std::string> test_map;

int main()
{
    boost::atomic<int> ai;
    ai = 3;
    ai = 4;

    std::tr1::shared_ptr<test_map> tm(new test_map);
    (*tm)[1208] = "test string";

    std::cout << "Apparently it works..." << std::endl;

    return 0;
}
