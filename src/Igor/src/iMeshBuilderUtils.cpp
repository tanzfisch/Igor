// Igor game engine
// (c) Copyright 2012-2018 by Martin Loga
// see copyright notice in corresponding header file

#include <iMeshBuilderUtils.h>

namespace Igor
{
    namespace iMeshBuilderUtils
    {

        void addCylinder(iMeshBuilder& meshBuilder, float32 radius, float32 height, uint32 segments, bool hasCaps)
        {
            con_assert(segments >= 3, "parameters out of range");
            con_assert(radius > 0.0f, "parameters out of range");
            con_assert(height > 0.0f, "parameters out of range");

            const float32 step = (2.0f * M_PI) / segments;

            const uint32 firstIndex = meshBuilder.getVertexCount();

            for (uint32 i = 0; i < segments; ++i)
            {
                meshBuilder.addVertex(iaVector3f(sin(i*step) * radius, 0, cos(i*step) * radius));
                meshBuilder.addVertex(iaVector3f(sin(i*step) * radius, height, cos(i*step) * radius));
            }

            uint32 bottomIndex = 0;
            uint32 topIndex = 0;

            if (hasCaps)
            {
                bottomIndex = meshBuilder.addVertex(iaVector3f(0, 0, 0));
                topIndex = meshBuilder.addVertex(iaVector3f(0, height, 0));
            }

            const uint32 segmentVertices = 2;
            const uint32 moduloValue = segments * segmentVertices;

            for (uint32 segment = 0; segment < segments; ++segment)
            {
                uint32 segmentIndex = (segment * segmentVertices);
                meshBuilder.addTriangle((segmentIndex + 2) % moduloValue, (segmentIndex + 1) % moduloValue, (segmentIndex + 0) % moduloValue, firstIndex);
                meshBuilder.addTriangle((segmentIndex + 2) % moduloValue, (segmentIndex + 3) % moduloValue, (segmentIndex + 1) % moduloValue, firstIndex);

                if (hasCaps)
                {
                    meshBuilder.addTriangle(((segmentIndex + 0) % moduloValue) + firstIndex, bottomIndex, ((segmentIndex + 2) % moduloValue) + firstIndex);
                    meshBuilder.addTriangle(((segmentIndex + 3) % moduloValue) + firstIndex, topIndex, ((segmentIndex + 1) % moduloValue) + firstIndex);
                }
            }
        }

        void addBox(iMeshBuilder& meshBuilder, float32 width, float32 height, float32 depth)
        {
            con_assert(width > 0.0f, "parameters out of range");
            con_assert(height > 0.0f, "parameters out of range");
            con_assert(depth > 0.0f, "parameters out of range");

            const uint32 firstIndex = meshBuilder.getVertexCount();

            meshBuilder.addVertex(iaVector3f(-0.5, 0, 0.5));
            meshBuilder.addVertex(iaVector3f(-0.5, 1, 0.5));
            meshBuilder.addVertex(iaVector3f(0.5, 1, 0.5));
            meshBuilder.addVertex(iaVector3f(0.5, 0, 0.5));
            meshBuilder.addVertex(iaVector3f(-0.5, 0, -0.5));
            meshBuilder.addVertex(iaVector3f(-0.5, 1, -0.5));
            meshBuilder.addVertex(iaVector3f(0.5, 1, -0.5));
            meshBuilder.addVertex(iaVector3f(0.5, 0, -0.5));

            meshBuilder.addTriangle(2, 1, 0, firstIndex);
            meshBuilder.addTriangle(3, 2, 0, firstIndex);
            meshBuilder.addTriangle(7, 4, 5, firstIndex);
            meshBuilder.addTriangle(6, 7, 5, firstIndex);
            meshBuilder.addTriangle(2, 5, 1, firstIndex);
            meshBuilder.addTriangle(2, 6, 5, firstIndex);
            meshBuilder.addTriangle(3, 0, 4, firstIndex);
            meshBuilder.addTriangle(7, 3, 4, firstIndex);
            meshBuilder.addTriangle(7, 2, 3, firstIndex);
            meshBuilder.addTriangle(6, 2, 7, firstIndex);
            meshBuilder.addTriangle(4, 0, 1, firstIndex);
            meshBuilder.addTriangle(5, 4, 1, firstIndex);
        }

        void addCone(iMeshBuilder& meshBuilder, float32 radius, float32 height, uint32 segments)
        {
            con_assert(height > 0.0f, "parameters out of range");
            con_assert(radius > 0.0f, "parameters out of range");
            con_assert(segments >= 3, "parameters out of range");

            const float32 step = (2.0f * M_PI) / segments;
            const uint32 firstIndex = meshBuilder.getVertexCount();

            for (uint32 i = 0; i < segments; ++i)
            {
                meshBuilder.addVertex(iaVector3f(sin(i*step), 0, cos(i*step)));
            }

            const uint32 bottomIndex = meshBuilder.addVertex(iaVector3f(0, 0, 0));
            const uint32 tipIndex = meshBuilder.addVertex(iaVector3f(0, height, 0));

            const uint32 moduloValue = segments;

            for (uint32 i = 0; i < segments; ++i)
            {
                uint32 segmentIndex = i;
                meshBuilder.addTriangle(((segmentIndex + 1) % moduloValue) + firstIndex, tipIndex, ((segmentIndex + 0) % moduloValue) + firstIndex);
                meshBuilder.addTriangle(((segmentIndex + 0) % moduloValue) + firstIndex, bottomIndex, ((segmentIndex + 1) % moduloValue) + firstIndex);
            }
        }

        void addCircle(iMeshBuilder& meshBuilder, float32 radius, uint32 segments)
        {
            con_assert(radius > 0.0f, "parameters out of range");
            con_assert(segments >= 3, "parameters out of range");

            const float32 step = (2.0f * M_PI) / segments;
            const uint32 firstIndex = meshBuilder.getVertexCount();

            for (uint32 i = 0; i < segments; ++i)
            {
                meshBuilder.addVertex(iaVector3f(sin(i*step), 0, cos(i*step)));
            }

            const uint32 centerIndex = meshBuilder.addVertex(iaVector3f(0, 0, 0));
            const uint32 moduloValue = segments;

            for (uint32 segmentIndex = 0; segmentIndex < segments; ++segmentIndex)
            {
                meshBuilder.addTriangle(((segmentIndex + 1) % moduloValue) + firstIndex, centerIndex, ((segmentIndex + 0) % moduloValue) + firstIndex);
            }
        }

        void addRing(iMeshBuilder& meshBuilder, float32 innerRadius, float32 outerRadius, uint32 segments)
        {
            con_assert(innerRadius < outerRadius, "parameters out of range");
            con_assert(innerRadius > 0.0f, "parameters out of range");
            con_assert(segments >= 3, "parameters out of range");

            const float32 step = (2.0f * M_PI) / segments;
            const uint32 firstIndex = meshBuilder.getVertexCount();

            for (uint32 i = 0; i < segments; ++i)
            {
                meshBuilder.addVertex(iaVector3f(sin(i*step) * innerRadius, 0, cos(i*step) * innerRadius));
                meshBuilder.addVertex(iaVector3f(sin(i*step) * outerRadius, 0, cos(i*step) * outerRadius));
            }

            const uint32 segmentVertices = 2;
            const uint32 moduloValue = segments * segmentVertices;

            for (uint32 segment = 0; segment < segments; ++segment)
            {
                uint32 segmentIndex = segment * segmentVertices;
                meshBuilder.addTriangle((segmentIndex + 0) % moduloValue, (segmentIndex + 1) % moduloValue, (segmentIndex + 2) % moduloValue, firstIndex);
                meshBuilder.addTriangle((segmentIndex + 1) % moduloValue, (segmentIndex + 3) % moduloValue, (segmentIndex + 2) % moduloValue, firstIndex);
            }
        }
    };
};
