#include <iaux/test/iaTest.h>

#include <iaux/iaDefines.h>

namespace iaux
{

    std::map<std::string, std::vector<iaTest *>> iaTest::_tests;
    static bool s_stopOnError = false;
    static bool s_useFilter = false;
    static std::string s_filter;

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
            else if (value == "--filter")
            {
                s_useFilter = true;
            }
            else if (s_useFilter && s_filter.empty())
            {
                s_filter = value;
            }
        }
    }

    void iaTest::runTests()
    {
        std::cout << "runTests" << std::endl;
        bool ok = true;
        for (auto groupPair : _tests)
        {
            for (auto test : groupPair.second)
            {
                std::stringstream testID;
                testID << test->getGroupName() << "." << test->getName();

                if (testID.str().find(s_filter) == std::string::npos)
                {
                    continue;
                }

                std::cout << "RUNNING " << testID.str() << " @ " << test->getLocation() << std::endl;
                test->run();
                if (!test->success())
                {
                    ok = false;

                    iaConsole::getInstance() << iaForegroundColor::Red << "TEST FAILED" << iaForegroundColor::Gray << endl
                                             << endl;
                }
                else
                {
                    iaConsole::getInstance() << iaForegroundColor::Green << "TEST OK" << iaForegroundColor::Gray << endl
                                             << endl;
                }

                if (s_stopOnError && !ok)
                {
                    return;
                }
            }
        }

        iaConsole::getInstance() << "over all result " << (ok ? iaForegroundColor::Green : iaForegroundColor::Red) << (ok ? "OK" : "FAILED") << iaForegroundColor::Gray << endl;
    }

} // namespace iaux