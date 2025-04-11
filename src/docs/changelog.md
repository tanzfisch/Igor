Changelog                           {#changelog}
=========

0.45.0
------

!!! MSVC build fails. no time to fix !!!

- #422 new entity based render engine (will eventually replace the scene graph based renderer)
- #414 reintroducing home brew entity system (it's something like a game object component thingy)
- IGOR_MAX_ENTITY_COMPONENT_TYPES is 64 but can be overridden 
- added random interface to iaColor3 and iaColor4
- iaDirectory and iaFile do not automatically fix a paths anymore
- #407 more renaming iShaderMaterial to iShader and related
- removed widget example since Mica covers all you need to know about widgets in Igor
- #374 using float32 for mouse position to reduce conversions
- #408 cleaned up ompf from legacy data and broke compatibility with older ompf (set ompf to 2.0.0)
- #404 made project a singleton and put project and user search paths on top in iResourceManager
- igor data folder now installed under data
- iProfiler now also tracks key value pairs per frame ie "draw calls", "triangles" etc
- iEntityScene is registered with iView which eventually will replace iScene
- #424 added sky box compatible to new render engine
- #420 integrated json and replaced all xml files except for shaders
- introduced prefab as resource
- iaEvent now allows to add a delegate that only get's triggered once
- introduced iaEventPool to handle events that only get called in main thread
- fixed del key issue in iWidgetLineTextEdit
- modal dialogs are allowed to be nested using a modal stack. The top one is the current modal one
- added iClipboard copy pasting text and entity IDs
- #412 bring entity system to use in Mica 

0.44.0
------

!!! MSVC build fails. no time to fix !!!

- #388 allow file select dialogs to be closed using ESC
- #387 improved render of drag operation
- #395 all examples to use now projects
- if no config file (igor.xml) is found one will be created in the user folder with default values
- changed up cmake for igor subfolder
- moved igor data files in to src/igor to make clear they are part of the engine it self
- added interface to save resources (implemented for materials only so far)
- fixed an issue with checker board backdrop of iWidgetPicture
- creating a default material from asset browser context menu in Mica
- #401 iMaterial -> iShaderMaterial and iTargetMaterial is not iMaterial
- removed some examples. Want to reduce further to have less maintenance in future

0.43.1
------

!!! MSVC build fails. no time to fix !!!

- #393 fixed invalid node info issue
- #391 adding more docs to examples
- #383 thumbnails now respect aspect ratio

0.43.0
------

!!! MSVC build fails. no time to fix !!!

- reimplemented iMaterial and make it a iResource
- replaced iMaterialResourceFactory with iMaterialFactory which is now integrated in the iResourceManager
- some spring cleaning around plenty of TODOs
- improved random number generator using STL
- introducing resource dictionaries and using 64bit ids to primarily identify resources
  (resource ids are not generated from the resource name or filename anymore)s
- introducing projects
- added interface to create empty/default resources (depends on what the factories do with it)
- added iWidgetBoxLayout
- renamed iWidgetGrid to iWidgetGridLayout
- replaced iUserControlAction with iWidgetButton
- dialogs can now be moved, resized and docked
- added interface to change mouse cursor see iMouseCursorType for supported types
- added iWidgetSplitter, iMimeData
- added drag&drop
- added iWidgetViewport
- widget event handling now uses iEvent
- added iUserControlTreeView and iUserControlGridView and iItem for generic data
- added AssetBrowser to Mica
- fixed texture font vertical alignment
- added new widget selection interfaces
- Mica drag drop assets in viewport (models and textures on models)
- added iUserControlTexture
- added background color and background checkerboard option to iWidgetPicture

Known issues:

- data not updated -> missing textures, invisible models -> need to do some work on ompf to get this all working again

0.42.0
------

- fixed threading sync in iResourceManager preventing resource being loaded multiple times
- reimplemented iModel and make it a iResource
- replaced iModelResourceFactory with iModelFactory which is now integrated in the iResourceManager
- removed iTaskFlushModels and iTaskLoadModel since this is now handled in iResourceManager
- auto start iTaskFlushResources after window is opened so the application does not need to do it anymore
- added quiet flag to resources

0.41.1
------

- added missing vsync control (Windows)
- fixed MSVC template specialization build issues

0.41.0
------

- improved iNodeTransform info string
- improved load time of instancing example
- lazy load audio device
- fixed mem leaks in iaDelegate, iWindow, iOctree
- going full DSA in iTexture
- refactored iTextureFactory
- added exit code to iApplication
- introduced load mode for resource manager
- fixed shutdown of voxel terrain
- added more settings to config file (min max threads and synchronous loading for resource manager)
- fixed black textures using mipmaps
- fixed physics integration
- added missing vsync control (Linux)

0.40.0
------

- improved renderer interface and fleshed out the docs
- fixed issue around texture fonts rendering and measuring size
- fixed more typos
- added more tests for iaux
- remove iTextureResourceFactory and have textures now handled by the iResourceManager
- new resources iAnimation, iSprite and iTexture (replaced previous iTexture)
- added state machine back in
- added pixmap to texture back in
- added new animation system running on top of the entity system
- replaced iAtlas with iSprite

0.39.0
------

!This release does not build with MSVC!

- wrapped entt. Let's see if this was so smart later
- implemented a whole bunch of components and systems
- added simple and probably slow home brewed components interface for custom user components (see iComponentMap)
- removed old entity system completely
- used Supremacy to make use of changed in ECS interfaces
- introduced application wide simulation pause/continue
- fixed lots of typos
- removed multi window support since it never worked anyway
- fixed issue in iaCircle
- added gray as standard color
- rewrote signal/slot aka event/delegate
- improved iAABox and iQuadtree interface
- changed profiler sections
- improved profiler visualization

0.38.0
------

- sorting files and directories alphabetically
- fixed scrolling of iWidgetScroll
- removed decompose/recompose matrix
- removed clipboard
- removed evaluation
- removed state machine
- removed memblock
- renamed iWidget setActive/isActive to setEnabled/isEnabled
- added align function to iaString
- improved profiler visualizer
- fixed selection sync issue in Mica

0.37.0
------

- allowing custom config file names
- fallback to default material when loading material fails
- moved all igor specific data under data/igor
- moved all mica specific data under data/mica
- removed unused shader folder
- added addPlane to iMeshBuilderUtils
- brought emitter node overlay back in mica

0.36.0
------

- minor improvements to entt wrapper interfaces
- cleaned up logging verbose modes
- removed console tests10

0.35.0
------

- worked on supremacy demo
- added multiline text widget (output only for now)
- changed iaEvent/iaDelegate naming to iaSignal/iaSlot
- iQuadtree uses std::any for user data instead of void*
- refactored iQuadtree
- introduced class iaRandom for cases where one does not need a unique random number generator instance
- added iaColor4::transparent
- added scalar operators for iaTime
- added more interfaces to the renderer
- updated and added some tests
- added non linear random numbers
- added triggerNow to iTimerHandle to allow to run a timed handle outside of the configured interval

0.34.6
------

- added missing texture tiling for particles
- added new tile index increment
- added missing multi texture support for particles
- fixed change of particle size over time

0.34.5
------

- introduced a particle system wrapper for easy use of 2D particle systems
- fixed iMaterialIO relative path to shader

0.34.4
------

- fixed text line edit clamping
- made iRenderer::flush public so we can force draw calls when needed for stencil testing
- activated 8bit stencil buffer for linux implementation

0.34.3
------

- Mica now can load and safe .mat files (material files)
- Mica is only displaying materials with public visibility
- ompf has a new chunk type which stores a reference to a material file

0.34.2
------

- Mica generally works again. But there is still a lot to do around materials ompf and mica
- fixed color id
- fixed selection highlighting

0.34.1 
------

- fixed instancing

0.34.0 (unstable/incomplete)
----------------------------

!This version is incomplete. See below!

- wrote entirely new Renderer using batch rendering where ever possible
- replaced Glee with Glad
- switched to Direct State Access
- improved testing framework and added some more tests
- added some missing functions in iQuadtree and iIntersection
- improved top down 2d shooter example
- added start/stop to iTimer to start/stop "game time" which essentially can be used to pause a game
- introduced new material file format .mat
- set minimum OpenGL version to 4.5
- iNodeSkyBox now takes a cube map texture 
- improved cmake configuration. Though there is more todo here
- new particle system replacing 2D and 3D ones
- support for geometry shaders

Known Missing features are:
- Instancing
- new particle system does not support all previous features yet
- Mica does not build
- missing world grid resolution feature
- missing color ID render mode feature used to select objects in the scene by Mica

0.33.0
------

- brushed up examples base
- implemented simple top down 2d shooter example
- using EnTT for the entity component system
- improved profiler interfaces

0.32.0
------

!Decided to drop MSVC support until I feel like it again!

- improved slider widget behavior
- log level DebugInfo is now called User
- default log level is now User
- improved error handling in iAudio
- textures are loaded cached by default now and do not get release immediately after nothing is referencing them anymore
- improved some debug output

0.31.0
------

!This release does not build with MSVC!

- improved rendering performance (still a long way to go though)
- replaced gtest with igorTest including writing more unit tests
- fixed material initialization problem in Ascent

0.30.0
------

!This release does not build with MSVC!

- introduced iModule and switched most singletons to modules
- removed iaSingleton class

0.29.0
------

- removed most of old event handling
- using AL_DIRECT_CHANNELS_SOFT for all multi channel sounds
- Mica: fixed framing by fixing bounding boxes
- Mica: fixed bake to world action
- improved behavior of menu and sub menus
- replaced gtest with own simple implementation and adding more tests
- fixed issue in cmake file regarding building igor as submodule
- DebugInfo is the default log level again
- renamed con_debug_endl to con_debug
- con_trace now has a message parameter
- renamed original con_trace to con_trace_call
- improved profiling visualization
- fixed performance issue in iScene where a queue was not cleared
- reduced setMaterial calls
- introduced one frame verbose output and render stats (F5 in examples)
- fixed problem with showing zero instances
- added vsync control on linux side

0.28.0 (unstable)
-----------------

This is probably less stable than previous versions but 
I really needed to get this out before it get's even bigger.

- fixed how menus behave
- improved console output
- made instancing and the instancing example work again
- supressing auto key repeat under Linux
- created an example base class for quicker prototyping
- iaBSpline is a template now
- fixed problem with setting transformations dirty down the tree
- mapping thread IDs to igor internal IDs for more useful logging output
- fixed left and right audio channel being swapped
- ALSA required on Linux
- added regex to iaString
- introduced layer and layer-stack
- introduced new queue-able events and replaced some of the old ones using event/delegate
- killed the state machine example
- some improvements around mouse handling in widgets

0.27.0
------

- sound support using OpenAL Soft (www.openal-soft.org/)
  including audio listener and audio source nodes (see usage in sound example)
- introduced ExampleBase class for prototyping
- changed namespaces from Igor and IgorAux to igor and iaux

0.26.0
------

- Linux and Windows support using cmake

0.25.0
------

- added basic evaluation functionality to control animations on transform nodes
- added iaFile::getStem

0.24.1
------

- fixed iDialogColorGradient
- fixed crash in Mica related to widget ownership problems
- fixed exception in Mica related to wrong type cast of std::any
- did some maintenance

0.24.0
------

- Basic iWidgetMenuBar and iWidgetMenu implementation
- Added z value order for widgets. For now only used for dialog order
- Introduced event blocking (used in widgets)
- Introduced Actions
- Using std::any for user data instead of void* where possible
- Improved dialog close handling
- Improved node creation interface
- Exposed iRenderStateSet interface in iMaterial to reduce writing effort when using it
- Renamed ModelViewer in to Mica

0.23.0
------

- Scroll bars are now fully functional
- New interface to create and destroy widgets. Basically we can use new and delete on widgets now but the engine will keep track of them to prevent mem leaks.
- Added colored axis indication to orientation grid in ModelViewer
- Made iaMatrix2D more useful
- Made SpriteAnimation example more pretty (thank you opengameart.org)

0.22.0
------

- iSprite is now iAtlas and can read frames from file
- Created sprite animation example
- Ownership of widgets transfers to the engine the moment they are children of a dialog
- added mesh to world bake functionality
- Removed all using namespace std;
- Added some keyboard shortcuts to ModelViewer
  - Ctrl + N - clear scene
  - Del - deletes current selection
  - Ctrl + D - duplicates current selection
  - Ctrl + C - copies current selection
  - Ctrl + X - cuts current selection
  - Ctrl + V - pastes on current selection
  - Ctrl + S - opens save dialog
  - Ctrl + O - opens load dialog
- Improved getInfo implementation and pretty print node tree
- Renamed iStatistic* to iProfiler*

0.21.0
------

- Increased unit testing efforts
- Worked on iaQuaternion class
- Turned iaRandomNumberGenerator in to a template
- Improved character controller example
- Improved mouse handling in all examples
- Added interface to control vsync setting
- Improved pretty printing for scene graph
- Added vsync setting on window
- Fixed another bug regarding texture units in ompf

0.20.0
------

- Replaced vector and matrix conversions in iaConvert with conversion methods directly on the classes.
- Added log level to console. Controllable by config file and during runtime.

0.19.1
------

- Fixed cursor position in text edit field when text was not aligned left

0.19.0
------

- ModelViewer: UserControlTransform now handles components instead of a matrix and improved Manipulator
- Implemented matrix decomposition
- Fixed iRenderer::project
- Added some google tests for matrix functions
- Introduced iaTransform that potentially will replace matrixes in scene graph

0.18.0
------

- Improved OBJ loader. Now meshs get generated based on combinations of groups/objects and materials.
- ModelViewer: pan camera around like in Maya (LeftAlt + middle mouse button)
- ModelViewer: fixed problem when loading unknown file format
- Improved iaString to be able to be used as key in std maps
- When loading models not to join vertices is now the default behavior
- Material "IgorTexture" always available. Same as "IgorDefault" just for textured materials.

0.17.0
------

- Introduced tooltips to widgets
- Used tooltips in ModelViewer

0.16.1
------

- Fixed problem with loading files over existing scene in ModelViewer

0.16.0
------

- Adding [tutorials](@ref tutorials) to doxygen documentation
- Changing doxygen style
- Generall update of docs

0.15.0
------

- Visualizing emitters in ModelViewer when selected
- Added sphere to iMeshBuilderUtils
- Added alpha to target material
- Support alpha in target material in OMPF
- Updated the OMPF example models with new format

pre Igor
--------

Before Igor there was openDC (Open Dirty Coding) starting in 2003.
Back then it's primary function was to load stuff and render it for projects during my Study.

As curiosity following the original header.

/*
                                              _______     ______             
                                             |       \   /                   
                                             |        | |                    
          ______   ______   ______  _ _____  |        | |                    
         /      \ |      \ /      \ |      \ |        | |                    
         |      | |      | |______  |      | |        | |                    
         |      | |      | |        |      | |        | |                    
         \______/ |______/ \_______ |      | |_______/   \______             
                  |                                                           
                  |                                                          
                     O p e n  D i r t y  C o d i n g                       
    
               Grundfunktionalitaet fuer 3D - Applikationen          

                  (c) Copyright 2003-2007 by Martin Loga               
                                                                            
  ------------------------------------------------------------------------- 
                                                                            
  This library is free software; you can redistribute it and or modify it   
  under the terms of the GNU Lesser General Public License as published by  
  the Free Software Foundation; either version 2.1 of the License, or (at   
  your option) any later version.                                           
                                                                            
  This library is distributed in the hope that it will be useful,           
  but WITHOUT ANY WARRANTY; without even the implied warranty of            
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         
  Lesser General Public License for more details.                           
                                                                            
  You should have received a copy of the GNU Lesser General Public          
  License along with this library; if not, write to the Free Software       
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA 
                                                                            
    contact: martinloga@gamedev.de                                          
    URL    : www.opendc.de                                                  
                                                                            
  ------------------------------------------------------------------------- 
  */      

pre OpenDC
----------

OpenDC came from easyGL. An OO wrapper for OpenGL. Started probably in 2001 or 2002.