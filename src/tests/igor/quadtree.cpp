#include <iaux/iaux.h>
#include <iaux/test/iaTest.h>

#include <igor/data/iQuadtree.h>
using namespace igor;

using Quadtree = iQuadtree<float32, int>;
using QuadtreeObject = iQuadtreeObject<float32, int>;
using QuadtreeObjectPtr = std::shared_ptr<iQuadtreeObject<float32, int>>;
using QuadtreeObjects = std::vector<std::shared_ptr<iQuadtreeObject<float32, int>>>;

static const iaRectanglef testRect1(-100, -100, 200, 200);
static const iaRectanglef testRect2(20, 0, 50, 50);

IAUX_TEST(QuadtreeTests, EmptyTree)
{
    Quadtree tree(testRect1);

    IAUX_EXPECT_EQUAL(tree.getRootBox().getX(), testRect1.getX());
    IAUX_EXPECT_EQUAL(tree.getRootBox().getY(), testRect1.getY());
    IAUX_EXPECT_EQUAL(tree.getRootBox().getWidth(), testRect1.getWidth());
    IAUX_EXPECT_EQUAL(tree.getRootBox().getHeight(), testRect1.getHeight());

    IAUX_EXPECT_TRUE(tree.getRoot()->_children[0] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[1] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[2] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[3] == nullptr);

    IAUX_EXPECT_TRUE(tree.getRoot()->_objects.empty());

    IAUX_EXPECT_TRUE(tree.getRoot()->_parent.lock() == nullptr);
}

IAUX_TEST(QuadtreeTests, AddObjectsAndClear)
{
    Quadtree tree(testRect1, 3);

    QuadtreeObjectPtr object1 = std::make_shared<QuadtreeObject>(iaCirclef(10, 10, 10), 10);
    QuadtreeObjectPtr object2 = std::make_shared<QuadtreeObject>(iaCirclef(40, 10, 10), 10);
    QuadtreeObjectPtr object3 = std::make_shared<QuadtreeObject>(iaCirclef(60, 10, 10), 10);
    QuadtreeObjectPtr object4 = std::make_shared<QuadtreeObject>(iaCirclef(90, 10, 10), 10);

    tree.insert(object1);
    tree.insert(object2);

    IAUX_EXPECT_EQUAL(object1.use_count(), 2);
    IAUX_EXPECT_EQUAL(object2.use_count(), 2);

    IAUX_EXPECT_TRUE(tree.getRoot()->_children[0] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[1] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[2] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[3] == nullptr);

    IAUX_EXPECT_EQUAL(tree.getRoot()->_objects.size(), 2);

    tree.insert(object3);
    tree.insert(object4);

    IAUX_EXPECT_EQUAL(object3.use_count(), 2);
    IAUX_EXPECT_EQUAL(object2.use_count(), 2);

    IAUX_EXPECT_TRUE(tree.getRoot()->_children[0] != nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[1] != nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[2] != nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[3] != nullptr);

    IAUX_EXPECT_EQUAL(tree.getRoot()->_objects.size(), 0);

    IAUX_EXPECT_TRUE(tree.getRoot()->_children[3]->_children[0] != nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[3]->_children[1] != nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[3]->_children[2] != nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[3]->_children[3] != nullptr);

    IAUX_EXPECT_EQUAL(tree.getRoot()->_children[3]->_children[0]->_objects.size(), 2);
    IAUX_EXPECT_EQUAL(tree.getRoot()->_children[3]->_children[1]->_objects.size(), 2);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[3]->_children[2]->_objects.empty());
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[3]->_children[3]->_objects.empty());

    tree.clear();

    IAUX_EXPECT_EQUAL(object1.use_count(), 1);
    IAUX_EXPECT_EQUAL(object2.use_count(), 1);
    IAUX_EXPECT_EQUAL(object3.use_count(), 1);
    IAUX_EXPECT_EQUAL(object4.use_count(), 1);

    IAUX_EXPECT_TRUE(tree.getRoot()->_children[0] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[1] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[2] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[3] == nullptr);

    IAUX_EXPECT_TRUE(tree.getRoot()->_objects.empty());    
}

IAUX_TEST(QuadtreeTests, AddObjectsAndRemove)
{
    Quadtree tree(testRect1, 3);

    QuadtreeObjectPtr object1 = std::make_shared<QuadtreeObject>(iaCirclef(10, 10, 10), 1);
    QuadtreeObjectPtr object2 = std::make_shared<QuadtreeObject>(iaCirclef(40, 10, 10), 2);
    QuadtreeObjectPtr object3 = std::make_shared<QuadtreeObject>(iaCirclef(60, 10, 10), 3);
    QuadtreeObjectPtr object4 = std::make_shared<QuadtreeObject>(iaCirclef(90, 10, 10), 4);

    tree.insert(object1);
    tree.insert(object2);
    tree.insert(object3);
    tree.insert(object4);

    tree.remove(object3);

    IAUX_EXPECT_EQUAL(object3.use_count(), 1);
}

IAUX_TEST(QuadtreeTests, AddObjectsAndQueryRectangle)
{
    Quadtree tree(testRect1, 3);

    QuadtreeObjectPtr object1 = std::make_shared<QuadtreeObject>(iaCirclef(10, 10, 5), 1);
    QuadtreeObjectPtr object2 = std::make_shared<QuadtreeObject>(iaCirclef(40, 10, 10), 2);
    QuadtreeObjectPtr object3 = std::make_shared<QuadtreeObject>(iaCirclef(60, 10, 10), 3);
    QuadtreeObjectPtr object4 = std::make_shared<QuadtreeObject>(iaCirclef(90, 10, 10), 4);

    tree.insert(object1);
    tree.insert(object2);
    tree.insert(object3);
    tree.insert(object4);

    QuadtreeObjects objects;
    tree.query(testRect2, objects);

    IAUX_EXPECT_EQUAL(objects.size(), 2);

    IAUX_EXPECT_TRUE(objects[0]->_userData== 2 || objects[0]->_userData == 3);
    IAUX_EXPECT_TRUE(objects[1]->_userData== 2 || objects[1]->_userData == 3);
}