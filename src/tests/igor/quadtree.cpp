#include <iaux/iaux.h>
#include <iaux/test/iaTest.h>

#include <igor/data/iQuadtree.h>
using namespace igor;

using Quadtree = iQuadtree<float32, int>;
using QuadtreeObject = iQuadtreeObject<float32, int>;
using QuadtreeObjectPtr = std::shared_ptr<iQuadtreeObject<float32, int>>;
using QuadtreeObjects = std::vector<std::shared_ptr<iQuadtreeObject<float32, int>>>;

static const iaRectanglef testRect(-100, -100, 200, 200);

IAUX_TEST(QuadtreeTests, EmptyTree)
{
    Quadtree tree(testRect);

    IAUX_EXPECT_EQUAL(tree.getRootBox().getX(), testRect.getX());
    IAUX_EXPECT_EQUAL(tree.getRootBox().getY(), testRect.getY());
    IAUX_EXPECT_EQUAL(tree.getRootBox().getWidth(), testRect.getWidth());
    IAUX_EXPECT_EQUAL(tree.getRootBox().getHeight(), testRect.getHeight());

    IAUX_EXPECT_TRUE(tree.getRoot()->_children[0] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[1] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[2] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[3] == nullptr);

    IAUX_EXPECT_TRUE(tree.getRoot()->_objects.empty());

    IAUX_EXPECT_TRUE(tree.getRoot()->_parent.lock() == nullptr);
}

IAUX_TEST(QuadtreeTests, AddObjectsAndClear)
{
    Quadtree tree(testRect);

    QuadtreeObjectPtr object = std::make_shared<QuadtreeObject>(iaCirclef(10, 10, 10), 10);
    IAUX_EXPECT_EQUAL(object.use_count(), 1);

    tree.insert(object);

    IAUX_EXPECT_EQUAL(object.use_count(), 2);

    IAUX_EXPECT_TRUE(tree.getRoot()->_children[0] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[1] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[2] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[3] == nullptr);

    IAUX_EXPECT_EQUAL(tree.getRoot()->_objects.size(), 1);

    tree.clear();

    IAUX_EXPECT_EQUAL(object.use_count(), 1);

    IAUX_EXPECT_TRUE(tree.getRoot()->_children[0] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[1] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[2] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[3] == nullptr);

    IAUX_EXPECT_TRUE(tree.getRoot()->_objects.empty());    
}