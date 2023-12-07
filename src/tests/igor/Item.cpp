#include <iaux/test/iaTest.h>

#include <igor/data/iItem.h>
using namespace igor;

IAUX_TEST(ItemTests, SetGetString)
{
    iItem item;

    item.setValue<iaString>("foo", "bar");
    iaString bar = item.getValue<iaString>("foo");

    IAUX_EXPECT_EQUAL(bar, "bar");
}