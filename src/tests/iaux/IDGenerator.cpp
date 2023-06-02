#include <iaux/iaux.h>
#include <iaux/test/iaTest.h>

#include <iaux/data/iaIDGenerator.h>
using namespace iaux;

IAUX_TEST(IDGeneratorTests, GenerateSomeIDs)
{
	iaIDGenerator64 idGenerator;

	IAUX_EXPECT_EQUAL(idGenerator.getNextID(), 1);
	IAUX_EXPECT_EQUAL(idGenerator.getNextID(), 2);
	IAUX_EXPECT_EQUAL(idGenerator.getNextID(), 3);
}
