Contribute to Igor                   {#contribute}
==================
 
Get Igor
--------

Get the Igor sources from [github](https://github.com/tanzfisch/Igor.git).
 
Submodules
----------

Igor currently links to the following libraries as sub modules.
 
- newton-dynamics
 
dont forget to
 
`git submodule init`

and

`git submodule update`

Build
-----

Igor is using cmake. It is configured to work with Debian/g++ (Windows/MSVC is currently not supported).

To build go to your working directory and run:

`mkdir build`
`cd build`
`cmake ..`
`make`

Build Debug
^^^^^^^^^^^
`cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=DEBUG`
`cmake --build build/debug`

Build Release
^^^^^^^^^^^^^
`cmake -S . -B build/release -DCMAKE_BUILD_TYPE=RELEASE`
`cmake --build build/release`

Note! Under Linux you might need to install some additional dependencies

`sudo apt-get install libx11-dev libgl1-mesa-dev libglu1-mesa-dev libglfw3-dev libalsa-ocaml-dev libxxf86vm-dev`

Git Flow
--------

Use git flow for branching and merging etc. Please find [here](https://danielkummer.github.io/git-flow-cheatsheet/) a very usefull cheat sheet.

Maybe get a git client that supports git flow naturally (e.g. [gitkraken](https://www.gitkraken.com/)).

Make a release
--------------

For making a release do the following steps.

1. create a release branch from develop named by the next release version (e.g. 0.23.0).

2. Update the [Changelog](@ref changelog)

3. Update doxygen config (src/docs/Doxyfile.doxy) using the new version of the release. And generate the doxygen docs.

4. Run some smoke tests (e.g. run all the examples once) and when satisfied merge in to master and develop. 

5. Create a tag with the new version number on master and push master, develop and the new tag to github.

Make a Patch Release 
--------------------

When happy with your hotfix update the [Changelog](@ref changelog) before mergin back in to master and develop.

Crate patch release version tag on master branch.

Doxygen
-------

Install [doxygen](http://www.doxygen.nl/download.html) (ver. 1.8.8) than run 
doxygen using the configuration file located at src/docs/Doxyfile.doxy