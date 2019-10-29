Changelog                           {#changelog}
=========

0.25.0
------

TODO

0.24.0
------

- Basic iWidgetMenuBar and iWidgetMenu implementation
  ![menu demonstration](/images/menu.gif)
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
  ![Scroll bars](/images/scrollbars.gif)
- New interface to create and destroy widgets. Basically we can use new and delete on widgets now but the engine will keep track of them to prevent mem leaks.
- Added colored axis indication to orientation grid in ModelViewer
- Made iaMatrix2D more usefull
- Made SpriteAnimation example more pretty (thank you opengameart.org)

0.22.0
------

- iSprite is now iAtlas and can read frames from file
- Created sprite animation example
  ![sprite animation](/images/SpriteAnimation.gif)
- Ownership of widgets transfers to the engine the moment they are children of a dialog
- Sdded mesh to world bake functionality
- Removed all using namesapce std;
- Sdded some keyboard shortcuts to ModelViewer
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
  ![pretty tree printing](/images/pretty_tree_print.png)
- Added vsync setting on window
- Fixed another bug regarding texture units in ompf

0.20.0
------

- Replaced vector and matrix conversions in iaConvert with conversion methods directly on the classes.
- Added log level to console. Controlable by config file and during runtime.

0.19.1
------

- Fixed cursor position in text edit field when text was not aligned left

0.19.0
------

- ModelViewer: UserControlTransform now handles components instead of a matrix and improved Manipulator
  ![New transform UI and improved scale modifier](/images/TransformUI.png)
- Implemented matrix decomposition
- Fixed iRenderer::project
- Added some google tests for matrix functions
- Introduced iaTransform that potentially will replace matrixes in scene graph

0.18.0
------

- Improved OBJ loader. Now meshs get generated based on combinations of groups/objects and materials.
  ![Improved mesh nodes structure from obj files](/images/OBJLoaderImprovements.png)
- ModelViewer: pan camera arround like in Maya (LeftAlt + middle mouse button)
- ModelViewer: fixed problem when loading unknown file format
- Improved iaString to be able to be used as key in std maps
- When loading models not to join vertices is now the default behavior
- Material "IgorTexture" always available. Same as "IgorDefault" just for textured materials.

0.17.0
------

- Introduced tooltips to widgets
- Used tooltips in ModelViewer
  ![Introduction of tooltips](/images/Tooltips.png)

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
  ![Visualized emitter sphere](/images/ModelViewer_Emitter_Visible.png)
- Added sphere to iMeshBuilderUtils
- Added alpha to target material
- Support alpha in target material in OMPF
- Updated the OMPF example models with new format