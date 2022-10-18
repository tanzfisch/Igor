#include <iaux/test/iaTest.h>

#include <iaux/iaDefines.h>

#include <sstream>

namespace iaux
{

    std::map<std::string, std::vector<iaTest *>> iaTest::_tests;
    bool iaTest::_stopOnError = false;
    bool iaTest::_useFilter = false;
    std::string iaTest::_filter;
    bool iaTest::_verbose = false;

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

            if (value == "--verbose")
            {
                _verbose = true;
            }
            if (value == "--stop-on-error")
            {
                _stopOnError = true;
            }
            else if (value == "--filter")
            {
                _useFilter = true;
            }
            else if (_useFilter && _filter.empty())
            {
                _filter = value;
            }
        }
    }

    void iaTest::runTests()
    {
        bool ok = true;
        for (auto groupPair : _tests)
        {
            for (auto test : groupPair.second)
            {
                std::stringstream testID;
                testID << test->getGroupName() << "." << test->getName();

                if (testID.str().find(_filter) == std::string::npos)
                {
                    continue;
                }

                if (_verbose)
                {
                    iaConsole::getInstance() << iaForegroundColor::Gray << "RUNNING " << testID.str().c_str() << " @ " << test->getLocation() << endl;
                }

                test->run();

                if (!test->success())
                {
                    ok = false;
                    iaConsole::getInstance() << iaForegroundColor::Red << "TEST FAILED " << iaForegroundColor::Gray << testID.str().c_str() << " @ " << test->getLocation() << endl << endl;
                }
                else
                {
                    if (_verbose)
                    {
                        iaConsole::getInstance() << iaForegroundColor::Green << "TEST OK" << iaForegroundColor::Gray << endl << endl;
                    }
                }

                if (_stopOnError && !ok)
                {
                    return;
                }
            }
        }

        iaConsole::getInstance() << "over all result " << (ok ? iaForegroundColor::Green : iaForegroundColor::Red) << (ok ? "OK" : "FAILED") << iaForegroundColor::Gray << endl;
    }

} // namespace iaux