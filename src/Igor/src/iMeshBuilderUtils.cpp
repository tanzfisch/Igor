// Igor game engine
// (c) Copyright 2012-2017 by Martin Loga
// see copyright notice in corresponding header file

#include <iMeshBuilderUtils.h>

namespace Igor
{
    namespace iMeshBuilderUtils
    {

        void addCylinder(iMeshBuilder& meshBuilder, float32 radius, float32 height, uint32 segments)
        {
            const float32 step = (2.0f * M_PI) / segments;

            for (uint32 i = 0; i < segments; ++i)
            {
                meshBuilder.addVertex(iaVector3f(sin(i*step) * radius, 0, cos(i*step) * radius));
                meshBuilder.addVertex(iaVector3f(sin(i*step) * radius, height, cos(i*step) * radius));
            }

            meshBuilder.addVertex(iaVector3f(0, 0, 0));
            meshBuilder.addVertex(iaVector3f(0, height, 0));

            const uint32 segmentVertices = 2;
            const uint32 moduloValue = segments * segmentVertices;

            for (uint32 segment = 0; segment < segments; ++segment)
            {
                uint32 segmentIndex = segment * segmentVertices;
                meshBuilder.addTriangle((segmentIndex + 0) % moduloValue, (segmentIndex + 1) % moduloValue, (segmentIndex + 2) % moduloValue);
                meshBuilder.addTriangle((segmentIndex + 1) % moduloValue, (segmentIndex + 3) % moduloValue, (segmentIndex + 2) % moduloValue);

                meshBuilder.addTriangle((segmentIndex + 2) % moduloValue, meshBuilder.getVertexCount() - 2, (segmentIndex + 0) % moduloValue);
                meshBuilder.addTriangle((segmentIndex + 1) % moduloValue, meshBuilder.getVertexCount() - 1, (segmentIndex + 3) % moduloValue);
            }
        }

        void addBox(iMeshBuilder& meshBuilder, float32 width, float32 height, float32 depth)
        {
            meshBuilder.addVertex(iaVector3f(-0.5, 0, 0.5));
            meshBuilder.addVertex(iaVector3f(-0.5, 1, 0.5));
            meshBuilder.addVertex(iaVector3f(0.5, 1, 0.5));
            meshBuilder.addVertex(iaVector3f(0.5, 0, 0.5));
            meshBuilder.addVertex(iaVector3f(-0.5, 0, -0.5));
            meshBuilder.addVertex(iaVector3f(-0.5, 1, -0.5));
            meshBuilder.addVertex(iaVector3f(0.5, 1, -0.5));
            meshBuilder.addVertex(iaVector3f(0.5, 0, -0.5));

            meshBuilder.addTriangle(0, 1, 2);
            meshBuilder.addTriangle(0, 2, 3);
            meshBuilder.addTriangle(5, 4, 7);
            meshBuilder.addTriangle(5, 7, 6);
            meshBuilder.addTriangle(1, 5, 2);
            meshBuilder.addTriangle(5, 6, 2);
            meshBuilder.addTriangle(4, 0, 3);
            meshBuilder.addTriangle(4, 3, 7);
            meshBuilder.addTriangle(3, 2, 7);
            meshBuilder.addTriangle(7, 2, 6);
            meshBuilder.addTriangle(1, 0, 4);
            meshBuilder.addTriangle(1, 4, 5);
        }

        void addCone(iMeshBuilder& meshBuilder, float32 radius, float32 height, uint32 segments)
        {
            const float32 step = (2.0f * M_PI) / segments;

            for (uint32 i = 0; i < segments; ++i)
            {
                meshBuilder.addVertex(iaVector3f(sin(i*step), 0, cos(i*step)));
            }

            meshBuilder.addVertex(iaVector3f(0, 0, 0));
            meshBuilder.addVertex(iaVector3f(0, height, 0));

            const uint32 moduloValue = segments;

            for (uint32 segmentIndex = 0; segmentIndex < segments; ++segmentIndex)
            {
                meshBuilder.addTriangle((segmentIndex + 0) % moduloValue, meshBuilder.getVertexCount() - 1, (segmentIndex + 1) % moduloValue);
                meshBuilder.addTriangle((segmentIndex + 1) % moduloValue, meshBuilder.getVertexCount() - 2, (segmentIndex + 0) % moduloValue);
            }
        }
    };
};
