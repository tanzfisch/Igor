#include <iaux/test/iaTest.h>

namespace iaux
{

    std::map<std::string, std::vector<iaTest *>> iaTest::_tests;

    void iaTest::registerTest(iaTest *test)
    {
        _tests[test->getGroupName()].push_back(test);
    }

    void iaTest::runAllTests()
    {
        std::cout << "runAllTests" << std::endl;
        bool ok = true;
        for (auto groupPair : _tests)
        {
            bool groupOK = true;
            std::cout << "run test group: " << groupPair.first << std::endl;
            for (auto test : groupPair.second)
            {
                test->run();
                if (!test->success())
                {
                    groupOK = ok = false;

                    std::cout << "FAILED " << test->getGroupName() << ":" << test->getName() << " @ " << test->getLocation() << std::endl;
                }
            }
            std::cout << "end test group: " << groupPair.first << " " << (groupOK ? "OK" : "FAILED") << std::endl;
        }

        std::cout << "over all result " << (ok ? "OK" : "FAILED") << std::endl;
    }

} // namespace iaux