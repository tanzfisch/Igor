#include <iaux/test/iaTest.h>

#include <igor/data/iMimeData.h>
using namespace igor;

IAUX_TEST(MimeDataTests, ArbitratyData)
{
    iMimeData mimeData;

    uint8 srcData[] = {1,2,3,4,5,6,7,8,9,0};
    mimeData.setData("mytype", srcData, 10);

    uint8 *data = nullptr;
    uint32 dataSize = 0;
    mimeData.getData("mytype", &data, dataSize);

    IAUX_EXPECT_EQUAL(data[3], srcData[3]);
    IAUX_EXPECT_EQUAL(dataSize, 10);
}

IAUX_TEST(MimeDataTests, TextData)
{
    iMimeData mimeData;
    mimeData.setText("my text");
    iaString text = mimeData.getText();

    IAUX_EXPECT_EQUAL(text, "my text");
    IAUX_EXPECT_TRUE(mimeData.hasText());
}

IAUX_TEST(MimeDataTests, WidgetIDData)
{
    iMimeData mimeData;
    mimeData.setWidgetID(1337);
    iWidgetID id = mimeData.getWidgetID();

    IAUX_EXPECT_EQUAL(id, 1337);
    IAUX_EXPECT_TRUE(mimeData.hasWidgetID());
}