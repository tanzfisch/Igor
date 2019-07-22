Contribute to Igor                   {#contribute}
==================
 
Get Igor
--------

You can get the Igor sources from [github](https://github.com/tanzfisch/Igor.git).
 
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
To build Igor open the solution at src\Igor\vs\Igor.sln set the target platform to x64 (32bit is no longer supported) and build.

Only ever the latest Visual Studio project files are included. There is no time to maintain more.

Git Flow
--------

Use git flow for branching and merging etc. Please find [here](https://danielkummer.github.io/git-flow-cheatsheet/) a very usefull cheat sheet.

Make a release
--------------

For making a release do the following steps.

1. create a release branch from develop named by the next release version.

2. Update the [Changelog](@ref changelog)

3. Update doxygen config (src/docs/Doxyfile.doxy) using the new version of the release. And generate the doxygen docs.

4. Run some smoke tests (ig run all the examples once) and when satisfied merge in to master and develop. 

5. Create a tag with the new version number on master and push master, devlop and the new tag to github.

Make a Patch Release 
--------------------

When happy with the hotfix update the [Changelog](@ref changelog) before mergin back in to master and develop.

Crate patch release version tag on master branch.

Doxygen
-------

Install [doxygen](http://www.doxygen.nl/download.html) (ver. 1.8.8) than run 
doxygen using the configuration file located at src/docs/Doxyfile.doxy