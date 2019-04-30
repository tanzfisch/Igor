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

Use git flow for branching and merging. Please find [here](https://danielkummer.github.io/git-flow-cheatsheet/) a very usefull cheat sheet.

Release
-------

Make sure to update the [Changelog](@ref changelog) before creating a tag for a new release version.

Good place to do that is while still working on the release branch.

Doxygen
-------

Install [doxygen](http://www.doxygen.nl/download.html) and [Graphviz](https://graphviz.gitlab.io/download/) than run 
doxygen using the configuration file located at src/docs/Doxyfile.doxy