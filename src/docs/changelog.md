Changelog                           {#changelog}
=========

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