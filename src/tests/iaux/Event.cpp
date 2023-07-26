#include <iaux/iaux.h>
#include <iaux/test/iaTest.h>

#include <iaux/system/iaEvent.h>
using namespace iaux;

IGOR_EVENT_DEFINITION(SimpleVoid, int);

int simpleFunction1()
{
    return 1;
}

int simpleFunction2()
{
    return 2;
}

class SimpleClass
{
    public:
        int simpleMethod()
        {
            return 100;
        }
};

IAUX_TEST(EventTests, EmptyDelegate)
{
    SimpleVoidDelegate delegate1;
    SimpleVoidDelegate delegate2;
    IAUX_EXPECT_FALSE(delegate1.isValid());
    IAUX_EXPECT_FALSE(delegate2.isValid());
    IAUX_EXPECT_NOT_EQUAL(delegate1, delegate2);
}

IAUX_TEST(EventTests, FunctionDelegate)
{
    SimpleVoidDelegate delegate(simpleFunction1);
    IAUX_EXPECT_TRUE(delegate.isValid());

    IAUX_EXPECT_EQUAL(delegate(), 1);
}

IAUX_TEST(EventTests, ClearDelegate)
{
    SimpleVoidDelegate delegate(simpleFunction1);
    IAUX_EXPECT_TRUE(delegate.isValid());
    IAUX_EXPECT_EQUAL(delegate(), 1);
    delegate.clear();
    IAUX_EXPECT_FALSE(delegate.isValid());
}

IAUX_TEST(EventTests, MethodDelegate)
{
    SimpleClass foo;
    SimpleVoidDelegate delegate(&foo, &SimpleClass::simpleMethod);
    IAUX_EXPECT_TRUE(delegate.isValid());

    IAUX_EXPECT_EQUAL(delegate(), 100);
}

IAUX_TEST(EventTests, FunctionDelegateOverride1)
{
    SimpleVoidDelegate delegate1(simpleFunction1);
    SimpleVoidDelegate delegate2;
    IAUX_EXPECT_TRUE(delegate1.isValid());
    IAUX_EXPECT_FALSE(delegate2.isValid());
    IAUX_EXPECT_NOT_EQUAL(delegate1, delegate2);
    IAUX_EXPECT_EQUAL(delegate1(), 1);

    delegate2 = delegate1;

    IAUX_EXPECT_EQUAL(delegate1, delegate2);
    IAUX_EXPECT_EQUAL(delegate1(), 1);
    IAUX_EXPECT_EQUAL(delegate2(), 1);
}

IAUX_TEST(EventTests, FunctionDelegateOverride2)
{
    SimpleVoidDelegate delegate1(simpleFunction1);
    SimpleVoidDelegate delegate2(simpleFunction2);
    IAUX_EXPECT_TRUE(delegate1.isValid());
    IAUX_EXPECT_TRUE(delegate2.isValid());
    IAUX_EXPECT_NOT_EQUAL(delegate1, delegate2);
    IAUX_EXPECT_EQUAL(delegate1(), 1);
    IAUX_EXPECT_EQUAL(delegate2(), 2);

    delegate1 = delegate2;

    IAUX_EXPECT_EQUAL(delegate1, delegate2);
    IAUX_EXPECT_EQUAL(delegate1(), 2);
    IAUX_EXPECT_EQUAL(delegate2(), 2);
}

IAUX_TEST(EventTests, CopyCtorDelegateOverrideFunction)
{
    SimpleVoidDelegate delegate1(simpleFunction1);
    SimpleVoidDelegate delegate2(delegate1);
    IAUX_EXPECT_TRUE(delegate1.isValid());
    IAUX_EXPECT_TRUE(delegate2.isValid());
    IAUX_EXPECT_EQUAL(delegate1, delegate2);
    IAUX_EXPECT_EQUAL(delegate1(), 1);
    IAUX_EXPECT_EQUAL(delegate2(), 1);
}

IAUX_TEST(EventTests, CopyCtorDelegateOverrideMethod)
{
    SimpleClass foo;
    SimpleVoidDelegate delegate1(&foo, &SimpleClass::simpleMethod);
    SimpleVoidDelegate delegate2(delegate1);
    IAUX_EXPECT_TRUE(delegate1.isValid());
    IAUX_EXPECT_TRUE(delegate2.isValid());
    IAUX_EXPECT_EQUAL(delegate1, delegate2);
    IAUX_EXPECT_EQUAL(delegate1(), 100);
    IAUX_EXPECT_EQUAL(delegate2(), 100);
}

IAUX_TEST(EventTests, EmptyEvent)
{
    SimpleVoidEvent event;
    IAUX_EXPECT_FALSE(event.hasDelegates());
    IAUX_EXPECT_FALSE(event.isBlocked());
}

IAUX_TEST(EventTests, RunEvent)
{
    SimpleVoidEvent event;

    SimpleClass foo;

    event.add(SimpleVoidDelegate(simpleFunction1));
    event.add(SimpleVoidDelegate(simpleFunction2));
    event.add(SimpleVoidDelegate(&foo, &SimpleClass::simpleMethod));

    IAUX_EXPECT_TRUE(event.hasDelegates());
    auto values = event();

    IAUX_EXPECT_EQUAL(values.size(), 3);
    IAUX_EXPECT_EQUAL(values[0], 1);
    IAUX_EXPECT_EQUAL(values[1], 2);
    IAUX_EXPECT_EQUAL(values[2], 100);
}

IAUX_TEST(EventTests, RemoveDelegates)
{
    SimpleVoidEvent event;
    SimpleClass foo;

    event.add(SimpleVoidDelegate(simpleFunction1));
    event.add(SimpleVoidDelegate(simpleFunction2));
    event.add(SimpleVoidDelegate(&foo, &SimpleClass::simpleMethod));

    event.remove(SimpleVoidDelegate(simpleFunction2));
    event.remove(SimpleVoidDelegate(&foo, &SimpleClass::simpleMethod));

    IAUX_EXPECT_TRUE(event.hasDelegates());
    auto values = event();

    IAUX_EXPECT_EQUAL(values.size(), 1);
    IAUX_EXPECT_EQUAL(values[0], 1);
}

IAUX_TEST(EventTests, BlockEvent)
{
    SimpleVoidEvent event;
    SimpleClass foo;

    event.add(SimpleVoidDelegate(simpleFunction1));
    event.add(SimpleVoidDelegate(simpleFunction2));
    event.add(SimpleVoidDelegate(&foo, &SimpleClass::simpleMethod));

    event.block();
    IAUX_EXPECT_TRUE(event.isBlocked());

    auto values = event();
    IAUX_EXPECT_TRUE(values.empty());

    event.unblock();

    values = event();
    IAUX_EXPECT_EQUAL(values.size(), 3);
}

IAUX_TEST(EventTests, ClearEvent)
{
    SimpleVoidEvent event;
    SimpleClass foo;

    event.add(SimpleVoidDelegate(simpleFunction1));
    event.add(SimpleVoidDelegate(simpleFunction2));
    event.add(SimpleVoidDelegate(&foo, &SimpleClass::simpleMethod));

    IAUX_EXPECT_TRUE(event.hasDelegates());
    event.clear();
    IAUX_EXPECT_FALSE(event.hasDelegates());
    auto values = event();

    IAUX_EXPECT_TRUE(values.empty());
}