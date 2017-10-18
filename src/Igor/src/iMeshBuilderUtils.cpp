// Igor game engine
// (c) Copyright 2012-2017 by Martin Loga
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

            for (uint32 i = 0; i < segments; ++i)
            {
                meshBuilder.addVertex(iaVector3f(sin(i*step) * radius, 0, cos(i*step) * radius));
                meshBuilder.addVertex(iaVector3f(sin(i*step) * radius, height, cos(i*step) * radius));
            }

            if (hasCaps)
            {
                meshBuilder.addVertex(iaVector3f(0, 0, 0));
                meshBuilder.addVertex(iaVector3f(0, height, 0));
            }

            const uint32 segmentVertices = 2;
            const uint32 moduloValue = segments * segmentVertices;

            for (uint32 segment = 0; segment < segments; ++segment)
            {
                uint32 segmentIndex = segment * segmentVertices;
                meshBuilder.addTriangle((segmentIndex + 2) % moduloValue, (segmentIndex + 1) % moduloValue, (segmentIndex + 0) % moduloValue);
                meshBuilder.addTriangle((segmentIndex + 2) % moduloValue, (segmentIndex + 3) % moduloValue, (segmentIndex + 1) % moduloValue);

                if (hasCaps)
                {
                    meshBuilder.addTriangle((segmentIndex + 0) % moduloValue, meshBuilder.getVertexCount() - 2, (segmentIndex + 2) % moduloValue);
                    meshBuilder.addTriangle((segmentIndex + 3) % moduloValue, meshBuilder.getVertexCount() - 1, (segmentIndex + 1) % moduloValue);
                }
            }
        }

        void addBox(iMeshBuilder& meshBuilder, float32 width, float32 height, float32 depth)
        {
            con_assert(width > 0.0f, "parameters out of range");
            con_assert(height > 0.0f, "parameters out of range");
            con_assert(depth > 0.0f, "parameters out of range");

            meshBuilder.addVertex(iaVector3f(-0.5, 0, 0.5));
            meshBuilder.addVertex(iaVector3f(-0.5, 1, 0.5));
            meshBuilder.addVertex(iaVector3f(0.5, 1, 0.5));
            meshBuilder.addVertex(iaVector3f(0.5, 0, 0.5));
            meshBuilder.addVertex(iaVector3f(-0.5, 0, -0.5));
            meshBuilder.addVertex(iaVector3f(-0.5, 1, -0.5));
            meshBuilder.addVertex(iaVector3f(0.5, 1, -0.5));
            meshBuilder.addVertex(iaVector3f(0.5, 0, -0.5));

            meshBuilder.addTriangle(2, 1, 0);
            meshBuilder.addTriangle(3, 2, 0);
            meshBuilder.addTriangle(7, 4, 5);
            meshBuilder.addTriangle(6, 7, 5);
            meshBuilder.addTriangle(2, 5, 1);
            meshBuilder.addTriangle(2, 6, 5);
            meshBuilder.addTriangle(3, 0, 4);
            meshBuilder.addTriangle(7, 3, 4);
            meshBuilder.addTriangle(7, 2, 3);
            meshBuilder.addTriangle(6, 2, 7);
            meshBuilder.addTriangle(4, 0, 1);
            meshBuilder.addTriangle(5, 4, 1);
        }

        void addCone(iMeshBuilder& meshBuilder, float32 radius, float32 height, uint32 segments)
        {
            con_assert(height > 0.0f, "parameters out of range");
            con_assert(radius > 0.0f, "parameters out of range");
            con_assert(segments >= 3, "parameters out of range");

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
                meshBuilder.addTriangle((segmentIndex + 1) % moduloValue, meshBuilder.getVertexCount() - 1, (segmentIndex + 0) % moduloValue);
                meshBuilder.addTriangle((segmentIndex + 0) % moduloValue, meshBuilder.getVertexCount() - 2, (segmentIndex + 1) % moduloValue);
            }
        }

        void addCircle(iMeshBuilder& meshBuilder, float32 radius, uint32 segments)
        {
            con_assert(radius > 0.0f, "parameters out of range");
            con_assert(segments >= 3, "parameters out of range");

            const float32 step = (2.0f * M_PI) / segments;

            for (uint32 i = 0; i < segments; ++i)
            {
                meshBuilder.addVertex(iaVector3f(sin(i*step), 0, cos(i*step)));
            }

            meshBuilder.addVertex(iaVector3f(0, 0, 0));

            const uint32 moduloValue = segments;

            for (uint32 segmentIndex = 0; segmentIndex < segments; ++segmentIndex)
            {
                meshBuilder.addTriangle((segmentIndex + 1) % moduloValue, meshBuilder.getVertexCount() - 1, (segmentIndex + 0) % moduloValue);
            }
        }

        void addRing(iMeshBuilder& meshBuilder, float32 innerRadius, float32 outerRadius, uint32 segments)
        {
            con_assert(innerRadius < outerRadius, "parameters out of range");
            con_assert(innerRadius > 0.0f, "parameters out of range");
            con_assert(segments >= 3, "parameters out of range");

            const float32 step = (2.0f * M_PI) / segments;

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
                meshBuilder.addTriangle((segmentIndex + 0) % moduloValue, (segmentIndex + 1) % moduloValue, (segmentIndex + 2) % moduloValue);
                meshBuilder.addTriangle((segmentIndex + 1) % moduloValue, (segmentIndex + 3) % moduloValue, (segmentIndex + 2) % moduloValue);
            }
        }
    };
};
