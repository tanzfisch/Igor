#include <iaux/iaux.h>
#include <iaux/test/iaTest.h>

#include <igor/data/iOctree.h>
using namespace igor;

static const iAACubef testCube1(iaVector3f(100,100,100), 100);
static const iAACubef testCube2(iaVector3f(100,100,100), 50);
static const iaSpheref testSphere1(iaVector3f(100,100,100), 50);

IAUX_TEST(OctreeTests, EmptyTree)
{
    iOctreef tree(testCube1, 4);

    IAUX_EXPECT_EQUAL(tree.getRootCube().getCenter(), testCube1.getCenter());
    IAUX_EXPECT_EQUAL(tree.getRootCube().getHalfEdge(), testCube1.getHalfEdge());

    IAUX_EXPECT_TRUE(tree.getRoot()->_children[0] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[1] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[2] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[3] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[4] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[5] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[6] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[7] == nullptr);

    IAUX_EXPECT_TRUE(tree.getRoot()->_objects.empty());

    IAUX_EXPECT_TRUE(tree.getRoot()->_parent.lock() == nullptr);
}

IAUX_TEST(OctreeTests, AddObjectsAndClear)
{
    iOctreef tree(testCube1, 3);

    auto object1 = std::make_shared<iOctreef::Object>(iaSpheref(iaVector3f(30,30,30), 10), 1);
    auto object2 = std::make_shared<iOctreef::Object>(iaSpheref(iaVector3f(90,150,30), 10), 2);
    auto object3 = std::make_shared<iOctreef::Object>(iaSpheref(iaVector3f(110,120,30), 10), 3);
    auto object4 = std::make_shared<iOctreef::Object>(iaSpheref(iaVector3f(150,150,130), 10), 4);

    tree.insert(object1);
    tree.insert(object2);

    IAUX_EXPECT_EQUAL(object1.use_count(), 2);
    IAUX_EXPECT_EQUAL(object2.use_count(), 2);

    IAUX_EXPECT_TRUE(tree.getRoot()->_children[0] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[1] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[2] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[3] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[4] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[5] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[6] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[7] == nullptr);

    IAUX_EXPECT_EQUAL(tree.getRoot()->_objects.size(), 2);

    tree.insert(object3);
    tree.insert(object4);

    IAUX_EXPECT_EQUAL(object3.use_count(), 2);
    IAUX_EXPECT_EQUAL(object2.use_count(), 2);

    IAUX_EXPECT_TRUE(tree.getRoot()->_children[0] != nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[1] != nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[2] != nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[3] != nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[4] != nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[5] != nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[6] != nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[7] != nullptr);

    IAUX_EXPECT_EQUAL(tree.getRoot()->_objects.size(), 0);

    IAUX_EXPECT_EQUAL(tree.getRoot()->_children[0]->_objects.size(), 1);
    IAUX_EXPECT_EQUAL(tree.getRoot()->_children[2]->_objects.size(), 1);
    IAUX_EXPECT_EQUAL(tree.getRoot()->_children[3]->_objects.size(), 1);
    IAUX_EXPECT_EQUAL(tree.getRoot()->_children[7]->_objects.size(), 1);

    IAUX_EXPECT_EQUAL(std::any_cast<int>(tree.getRoot()->_children[0]->_objects[0]->_userData), 1);
    IAUX_EXPECT_EQUAL(std::any_cast<int>(tree.getRoot()->_children[2]->_objects[0]->_userData), 2);
    IAUX_EXPECT_EQUAL(std::any_cast<int>(tree.getRoot()->_children[3]->_objects[0]->_userData), 3);
    IAUX_EXPECT_EQUAL(std::any_cast<int>(tree.getRoot()->_children[7]->_objects[0]->_userData), 4);

    tree.clear();

    IAUX_EXPECT_EQUAL(object1.use_count(), 1);
    IAUX_EXPECT_EQUAL(object2.use_count(), 1);
    IAUX_EXPECT_EQUAL(object3.use_count(), 1);
    IAUX_EXPECT_EQUAL(object4.use_count(), 1);

    IAUX_EXPECT_TRUE(tree.getRoot()->_children[0] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[1] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[2] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[3] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[4] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[5] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[6] == nullptr);
    IAUX_EXPECT_TRUE(tree.getRoot()->_children[7] == nullptr);

    IAUX_EXPECT_TRUE(tree.getRoot()->_objects.empty());
}

IAUX_TEST(OctreeTests, AddObjectsAndRemove)
{
    iOctreef tree(testCube1, 3);

    auto object1 = std::make_shared<iOctreef::Object>(iaSpheref(iaVector3f(30,30,30), 10), 1);
    auto object2 = std::make_shared<iOctreef::Object>(iaSpheref(iaVector3f(100,150,100), 10), 2);
    auto object3 = std::make_shared<iOctreef::Object>(iaSpheref(iaVector3f(100,100,150), 10), 3);
    auto object4 = std::make_shared<iOctreef::Object>(iaSpheref(iaVector3f(10,10,100), 10), 4);

    tree.insert(object1);
    tree.insert(object2);
    tree.insert(object3);
    tree.insert(object4);

    tree.remove(object3);

    IAUX_EXPECT_EQUAL(object3.use_count(), 1);
}

IAUX_TEST(OctreeTests, AddObjectsAndQueryCubeangle)
{
    iOctreef tree(testCube1, 3);

    auto object1 = std::make_shared<iOctreef::Object>(iaSpheref(iaVector3f(30,30,30), 10), 1);
    auto object2 = std::make_shared<iOctreef::Object>(iaSpheref(iaVector3f(110,90,120), 10), 2);
    auto object3 = std::make_shared<iOctreef::Object>(iaSpheref(iaVector3f(160,180,150), 10), 3);
    auto object4 = std::make_shared<iOctreef::Object>(iaSpheref(iaVector3f(10,10,100), 10), 4);

    tree.insert(object1);
    tree.insert(object2);
    tree.insert(object3);
    tree.insert(object4);

    iOctreef::Objects objects;
    tree.query(testCube2, objects);

    IAUX_EXPECT_EQUAL(objects.size(), 1);
    IAUX_EXPECT_TRUE(std::any_cast<int>(objects[0]->_userData) == 2);
}

IAUX_TEST(OctreeTests, AddObjectsAndQuerySphere)
{
    iOctreef tree(testCube1, 3);

    auto object1 = std::make_shared<iOctreef::Object>(iaSpheref(iaVector3f(30,30,30), 10), 1);
    auto object2 = std::make_shared<iOctreef::Object>(iaSpheref(iaVector3f(110,90,120), 10), 2);
    auto object3 = std::make_shared<iOctreef::Object>(iaSpheref(iaVector3f(160,180,150), 10), 3);
    auto object4 = std::make_shared<iOctreef::Object>(iaSpheref(iaVector3f(10,10,100), 10), 4);

    tree.insert(object1);
    tree.insert(object2);
    tree.insert(object3);
    tree.insert(object4);

    iOctreef::Objects objects;
    tree.query(testSphere1, objects);

    IAUX_EXPECT_EQUAL(objects.size(), 1);
    IAUX_EXPECT_TRUE(std::any_cast<int>(objects[0]->_userData) == 2);
}

IAUX_TEST(OctreeTests, MoveObjects)
{
    iOctreef tree(testCube1, 3);

    auto object1 = std::make_shared<iOctreef::Object>(iaSpheref(iaVector3f(30,30,30), 10), 1);
    auto object2 = std::make_shared<iOctreef::Object>(iaSpheref(iaVector3f(110,90,120), 10), 2);
    auto object3 = std::make_shared<iOctreef::Object>(iaSpheref(iaVector3f(160,180,150), 10), 3);
    auto object4 = std::make_shared<iOctreef::Object>(iaSpheref(iaVector3f(10,10,100), 10), 4);

    tree.insert(object1);
    tree.insert(object2);
    tree.insert(object3);
    tree.insert(object4);

    iOctreef::Objects objects;
    tree.query(testSphere1, objects);

    IAUX_EXPECT_EQUAL(objects.size(), 1);
    IAUX_EXPECT_TRUE(std::any_cast<int>(objects[0]->_userData) == 2);

    tree.update(object2, iaVector3f(10,20,10));
    tree.update(object3, iaVector3f(100,100,100));

    objects.clear();
    tree.query(testSphere1, objects);

    IAUX_EXPECT_EQUAL(objects.size(), 1);
    IAUX_EXPECT_TRUE(std::any_cast<int>(objects[0]->_userData) == 3);
}