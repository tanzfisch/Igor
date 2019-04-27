L-System Example                                   {#example10}
================

The L-System example shows how to set up L-Systems and generate meshs from them by using a skeleton and the iModelDataIO interface.

Build
=====

The code for this example can be found in the folder examples/10 L-Systems. In a vs[ver] sub folder you will find a visual studio solution which contains example project but also all dependent projects. So you just open this one and than build and start. If everything went right you gonna see the following screen. If not please don't hesitate to contact [tanzfisch](https://github.com/tanzfisch).

![L-System Example](/images/Example10_Pic1.png)

There is also a youtube clip [L-System Example](https://youtu.be/5qK0ee4xTug).

Interaction
===========

* Press **Left Mouse Button** and drag to rotate the camera position around center of interest.
* Use **Mouse Wheel** to zoom in and out
* Press **Space** to regenerate plants

Code
====

All the meat about how to use the iLSystem class is in the methods initStyle1, initStyle2 and initStyle3. We will only have a closer look at one of them because they are mostly the same except for the values ins use.

The first line is a typical replacement rule F will be replaced with FF. This basically makes all branches grow double each iteration.

    _lSystem.setRule('F', "FF");

In the next section we have an example for a weighted rule with 4 cases. Make sure all four probabilities add up to 1.0.

    vector<pair<float64, iaString>> weightedRule1;
    weightedRule1.push_back(pair<float64, iaString>(0.25, "F[+X]OF[-X]+X."));
    weightedRule1.push_back(pair<float64, iaString>(0.25, "FO[-X]F[+X]-X."));
    weightedRule1.push_back(pair<float64, iaString>(0.25, "F[RX]F[LX]ORX."));
    weightedRule1.push_back(pair<float64, iaString>(0.25, "F[LX]FO[RX]LX."));
    _lSystem.setRule('X', weightedRule1);

In the following section we also have a weighted rule but it also has an additional age filter. It basically says that it has only effect after the L-System age is greater 2. In our scenario the buds that are represented with a '*' can earliest appear on the 4th plant because it has the age of 4.

    vector<pair<float64, iaString>> weightedRule2;
    weightedRule2.push_back(pair<float64, iaString>(0.3, "."));
    weightedRule2.push_back(pair<float64, iaString>(0.7, "*"));
    _lSystem.setRule('.', weightedRule2);
    _lSystem.setAgeFilter('.', iLSystemAgeFunction::Greater, 2);

The next one works exactly the same but instead of turning the '.' placeholder in to buds it turns buds in to flowers.

    vector<pair<float64, iaString>> weightedRule3;
    weightedRule3.push_back(pair<float64, iaString>(0.3, "*"));
    weightedRule3.push_back(pair<float64, iaString>(0.7, "o"));
    _lSystem.setRule('*', weightedRule3);
    _lSystem.setAgeFilter('*', iLSystemAgeFunction::Greater, 4);

The following definitions have nothing to do with the L-System it self. They are only important for the later generation of meshs and representation of the L-Systems.

    _segmentLength = 0.25;
    _angle = 0.3;
    _trunkColor.set(0, 0.8, 0);
    _budColor.set(0.8, 0.7, 0.0);
    _flowerColor.set(1, 0, 0);
    _leafColor.set(0, 0.7, 0);

Inside the PlantMeshGenerator class we do basically three things.
1. we run the L-System generation to create a new sentence that describes our plant
2. we generate a skeleton based on the L-System
3. we generate some mehs basxed on the skeleton

For generating the L-System sentence we just call the following function. The first parameter is the starting sentence that should contain something the L-System is able to replace. In our case just "X". The second parameter stands for how many iterations the L-System should run. Iterations in our case equals age. And last but not lease we git the l-System a random seed so we can actually generate different ages of the exact same plant and make it grow.

    iaString sentence = lSystem->generate(plantInformation->_axiom, plantInformation->_iterations, _rand.getNext());

Next we generate a skeleton based on the generated sentence. We basically iterate through the sentence and based on the symbols we find we create bones make transformations or attach custom values to the bones. All that can be found in PlantMeshGenerator::generateSkeleton but will not discussed in this tutorial in detail. And last we traverse through the skeleton tree and generate the mesh by adding stems, buds or flowers.

All this can be done of course more efficient and better looking but the tutorial was meant to show primarily the iLSystem class. Feel free to change it as you wish and if you have further questions please contact [tanzfisch](https://github.com/tanzfisch).