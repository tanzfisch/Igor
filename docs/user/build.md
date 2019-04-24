 Building Igor                   {#build}
 =============
 
 Get Igor
 --------
 You can get Igor at github with following link
 
 https://github.com/tanzfisch/Igor.git
 
 Submodules
 ----------
 Igor currently links to the following libraries as sub modules.
 
 - newton-dynamics
 
 dont forget to
 
 git submodule init
 
 and
 
 git submodule update
 
 Build
 -----
 To build Igor open the solution at src\Igor\vs\Igor.sln set the target platform to x64 (32bit is no longer supported) and build.
 
 Only ever the latest Visual Studio project files are included. There is no time to maintain more.