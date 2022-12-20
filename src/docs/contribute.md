Contribute to Igor                   {#contribute}
==================
 
Get Igor
--------

Get the Igor sources from [github](https://github.com/tanzfisch/Igor.git).
 
Submodules
----------

Igor currently links to the following libraries as sub modules.
 
- newton-dynamics
- openal-soft
 
dont forget to run the following unless your git client already does it for you

``` 
git submodule init
git submodule update
```

Build
-----

Igor is using cmake to build. It is tested to work on Ubuntu and Debian with GCC and Windows/MSVC.

Preffered setup is VSCode, MSVC and CMAKE extension but it should work with Visual Studio as well.

**Note!** Under Linux you might need to install some additional dependencies like the following ...

```
sudo apt-get install libx11-dev libgl1-mesa-dev libglu1-mesa-dev libglfw3-dev libalsa-ocaml-dev libxxf86vm-dev
```

Run from VSCode
---------------

Make sure when debugging from vscode to add the following to your settings.json in order for it to start from the correct working directory otherwise all relative paths to the data folder will fail.

```
    "cmake.debugConfig": {
        "cwd": "${workspaceFolder}"
    }
```

Git Flow
--------

Use git flow for branching and merging etc. Please find [here](https://danielkummer.github.io/git-flow-cheatsheet/) a very usefull cheat sheet.

Maybe get a git client that supports git flow naturally (e.g. [gitkraken](https://www.gitkraken.com/)).

Make a release
--------------

For making a release do the following steps.

1. create a release branch from develop named by the next release version (e.g. 0.23.0).

2. run smoke tests (run all the examples, tests and applications like Mica)

3. if tests fail try to fix on the release branch if possible or abbort the release and start over

4. Update the [Changelog](@ref changelog)

5. Update doxygen config file (src/docs/Doxyfile.doxy) using the new version of the release. And generate the doxygen docs.

6. when satisfied with test and docs merge the release in to master and develop. 

7. Create a tag with the new version number on master and push master, develop and the new tag

Make a Patch Release
--------------------

When happy with your hotfix update the [Changelog](@ref changelog) before mergin back in to master and develop.

Crate patch release version tag on master branch.

Doxygen
-------

Install [doxygen](http://www.doxygen.nl/download.html) (ver. 1.8.8) than run 
doxygen using the configuration file located at src/docs/Doxyfile.doxy