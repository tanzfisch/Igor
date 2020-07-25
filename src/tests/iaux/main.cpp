#include <iaux/test/iaTest.h>

int main(int argc, char** argv) {
    iaux::iaTest::initTests(argc, argv);
    iaux::iaTest::runTests();
    return 0;
}