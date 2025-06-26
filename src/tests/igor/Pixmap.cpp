#include <iaux/test/iaTest.h>

#include <igor/resources/texture/iPixmap.h>
using namespace igor;

IAUX_TEST(PixmapTests, GetSetRGB)
{
    auto pixmap = iPixmap::createPixmap(10, 10, iColorFormat::RGB);

    iaColor3c in(10, 20, 30);

    pixmap->setPixel(5, 5, in);
    auto out = pixmap->getPixelRGB(5, 5);
    IAUX_EXPECT_EQUAL(in, out);
}

IAUX_TEST(PixmapTests, GetSetRGBA)
{
    auto pixmap = iPixmap::createPixmap(10, 10, iColorFormat::RGBA);

    iaColor4c in(10, 20, 30, 40);

    pixmap->setPixel(5, 5, in);
    auto out = pixmap->getPixelRGBA(5, 5);
    IAUX_EXPECT_EQUAL(in, out);
}