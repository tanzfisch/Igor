#include <iaux/test/iaTest.h>

#include <iaux/iaDefines.h>

namespace iaux
{

    std::map<std::string, std::vector<iaTest *>> iaTest::_tests;
    static bool s_stopOnError = true;

    void iaTest::registerTest(iaTest *test)
    {
        _tests[test->getGroupName()].push_back(test);
    }

    void iaTest::initTests(int argc, char **argv)
    {
        if (argc <= 1)
        {
            return;
        }

        for (uint32 i = 1; i < argc; ++i)
        {
            std::string value(argv[i]);

            if (value == "--stop-on-error")
            {
                s_stopOnError = true;
            }
        }
    }

    void iaTest::runTests()
    {
        std::cout << "runTests" << std::endl;
        bool ok = true;
        for (auto groupPair : _tests)
        {
            bool groupOK = true;
            std::cout << "BEGIN GROUP " << groupPair.first << std::endl;
            for (auto test : groupPair.second)
            {
                std::cout << "RUNNING " << test->getGroupName() << "." << test->getName() << " @ " << test->getLocation() << std::endl;
                test->run();
                if (!test->success())
                {
                    groupOK = ok = false;

                    std::cout << "FAILED" << std::endl;
                }
                else
                {
                    std::cout << "OK" << std::endl;
                }

                if(s_stopOnError && !ok)
                {
                    return;
                }
                
            }
            std::cout << "END GROUP " << groupPair.first << " " << (groupOK ? "OK" : "FAILED") << std::endl;
        }

        std::cout << "over all result " << (ok ? "OK" : "FAILED") << std::endl;
    }

} // namespace iaux