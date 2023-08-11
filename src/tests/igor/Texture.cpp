#include <iaux/iaux.h>
#include <iaux/test/iaTest.h>

#include <igor/resources/texture/iTexture.h>
using namespace igor;

IAUX_TEST(TextureTests, MipMapCalculation)
{
    IAUX_EXPECT_EQUAL(iTexture::calcMipMapLevels(128, 128), 8);
    IAUX_EXPECT_EQUAL(iTexture::calcMipMapLevels(128, 64), 8);
    IAUX_EXPECT_EQUAL(iTexture::calcMipMapLevels(64, 128), 8);
    IAUX_EXPECT_EQUAL(iTexture::calcMipMapLevels(1, 1), 1);
}
