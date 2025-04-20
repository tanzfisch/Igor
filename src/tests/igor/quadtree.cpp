#include <iaux/iaux.h>
#include <iaux/test/iaTest.h>

#include <igor/data/iQuadtree.h>
using namespace igor;

static const iaRectanglef testRect1(-100, -100, 200, 200);
static const iaRectanglef testRect2(20, 0, 50, 50);
static const iaCirclef testCircle1(20, 10, 15);

IAUX_TEST(QuadtreeTests, EmptyTree)
{
    iQuadtreef tree(testRect1);

    IAUX_EXPECT_EQUAL(tree.getArea().getX(), testRect1.getX());
    IAUX_EXPECT_EQUAL(tree.getArea().getY(), testRect1.getY());
    IAUX_EXPECT_EQUAL(tree.getArea().getWidth(), testRect1.getWidth());
    IAUX_EXPECT_EQUAL(tree.getArea().getHeight(), testRect1.getHeight());

    IAUX_EXPECT_TRUE(tree.getRoot()->_children[0] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[1] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[2] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[3] == nullptr);

    IAUX_EXPECT_TRUE(tree.getRoot()->_objects.empty());

    IAUX_EXPECT_TRUE(tree.getRoot()->_parent.lock() == nullptr);
}

IAUX_TEST(QuadtreeTests, AddObjectsAndClear)
{
    iQuadtreef tree(testRect1, 3);

    auto object1 = std::make_shared<iQuadtreef::Object>(iaCirclef(10, 10, 10), 10);
    auto object2 = std::make_shared<iQuadtreef::Object>(iaCirclef(40, 10, 10), 10);
    auto object3 = std::make_shared<iQuadtreef::Object>(iaCirclef(60, 10, 10), 10);
    auto object4 = std::make_shared<iQuadtreef::Object>(iaCirclef(90, 10, 10), 10);

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
    iQuadtreef tree(testRect1, 3);

    auto object1 = std::make_shared<iQuadtreef::Object>(iaCirclef(10, 10, 10), 1);
    auto object2 = std::make_shared<iQuadtreef::Object>(iaCirclef(40, 10, 10), 2);
    auto object3 = std::make_shared<iQuadtreef::Object>(iaCirclef(60, 10, 10), 3);
    auto object4 = std::make_shared<iQuadtreef::Object>(iaCirclef(90, 10, 10), 4);

    tree.insert(object1);
    tree.insert(object2);
    tree.insert(object3);
    tree.insert(object4);

    tree.remove(object3);

    IAUX_EXPECT_EQUAL(object3.use_count(), 1);
}

IAUX_TEST(QuadtreeTests, AddObjectsAndQueryRectangle)
{
    iQuadtreef tree(testRect1, 3);

    auto object1 = std::make_shared<iQuadtreef::Object>(iaCirclef(10, 10, 5), 1);
    auto object2 = std::make_shared<iQuadtreef::Object>(iaCirclef(40, 10, 10), 2);
    auto object3 = std::make_shared<iQuadtreef::Object>(iaCirclef(60, 10, 10), 3);
    auto object4 = std::make_shared<iQuadtreef::Object>(iaCirclef(90, 10, 10), 4);

    tree.insert(object1);
    tree.insert(object2);
    tree.insert(object3);
    tree.insert(object4);

    iQuadtreef::Objects objects;
    tree.query(testRect2, objects);

    IAUX_EXPECT_EQUAL(objects.size(), 2);

    IAUX_EXPECT_TRUE(std::any_cast<int>(objects[0]->_userData) == 2 || std::any_cast<int>(objects[0]->_userData) == 3);
    IAUX_EXPECT_TRUE(std::any_cast<int>(objects[1]->_userData) == 2 || std::any_cast<int>(objects[1]->_userData) == 3);
}

IAUX_TEST(QuadtreeTests, AddObjectsAndQueryCircle)
{
    iQuadtreef tree(testRect1, 3);

    auto object1 = std::make_shared<iQuadtreef::Object>(iaCirclef(10, 10, 10), 1);
    auto object2 = std::make_shared<iQuadtreef::Object>(iaCirclef(30, 10, 10), 2);
    auto object3 = std::make_shared<iQuadtreef::Object>(iaCirclef(60, 10, 10), 3);
    auto object4 = std::make_shared<iQuadtreef::Object>(iaCirclef(90, 10, 10), 4);

    tree.insert(object1);
    tree.insert(object2);
    tree.insert(object3);
    tree.insert(object4);

    iQuadtreef::Objects objects;
    tree.query(testCircle1, objects);

    IAUX_EXPECT_EQUAL(objects.size(), 2);

    IAUX_EXPECT_TRUE(std::any_cast<int>(objects[0]->_userData) == 1 || std::any_cast<int>(objects[0]->_userData) == 2);
    IAUX_EXPECT_TRUE(std::any_cast<int>(objects[1]->_userData) == 1 || std::any_cast<int>(objects[1]->_userData) == 2);
}

IAUX_TEST(QuadtreeTests, MoveObjects)
{
    iQuadtreef tree(testRect1, 3);

    auto object1 = std::make_shared<iQuadtreef::Object>(iaCirclef(10, 10, 10), 1);
    auto object2 = std::make_shared<iQuadtreef::Object>(iaCirclef(30, 10, 10), 2);
    auto object3 = std::make_shared<iQuadtreef::Object>(iaCirclef(60, 10, 10), 3);
    auto object4 = std::make_shared<iQuadtreef::Object>(iaCirclef(90, 10, 10), 4);

    tree.insert(object1);
    tree.insert(object2);
    tree.insert(object3);
    tree.insert(object4);

    iQuadtreef::Objects objects;
    tree.query(testCircle1, objects);

    IAUX_EXPECT_EQUAL(objects.size(), 2);

    IAUX_EXPECT_TRUE(std::any_cast<int>(objects[0]->_userData) == 1 || std::any_cast<int>(objects[0]->_userData) == 2);
    IAUX_EXPECT_TRUE(std::any_cast<int>(objects[1]->_userData) == 1 || std::any_cast<int>(objects[1]->_userData) == 2);

    tree.update(object2, iaVector2f(50,50));
    tree.update(object3, iaVector2f(25,12));

    objects.clear();
    tree.query(testCircle1, objects);

    IAUX_EXPECT_EQUAL(objects.size(), 2);

    IAUX_EXPECT_TRUE(std::any_cast<int>(objects[0]->_userData) == 1 || std::any_cast<int>(objects[0]->_userData) == 3);
    IAUX_EXPECT_TRUE(std::any_cast<int>(objects[1]->_userData) == 1 || std::any_cast<int>(objects[1]->_userData) == 3);

}