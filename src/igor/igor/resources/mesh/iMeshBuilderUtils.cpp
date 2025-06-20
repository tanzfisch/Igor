// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/resources/mesh/iMeshBuilderUtils.h>

namespace igor
{
    namespace iMeshBuilderUtils
    {
        void addPlane(iMeshBuilder &meshBuilder, float32 width, float32 depth, uint32 segmentsX, uint32 segmentsZ, bool normals, bool texCoords)
        {
            uint32 offsetIndex = meshBuilder.getVertexCount();
            const float32 xPos = -width * 0.5f;
            const float32 zPos = -depth * 0.5f;
            const float32 xStepping = width / segmentsX;
            const float32 zStepping = depth / segmentsZ;

            // Create vertices with texture coordinates
            for (uint32 z = 0; z <= segmentsZ; ++z)
            {
                float v = float32(z) / segmentsZ;

                for (uint32 x = 0; x <= segmentsX; ++x)
                {
                    float u = float32(x) / segmentsX;

                    iaVector3f pos(xPos + x * xStepping, 0.0f, zPos + z * zStepping);
                    uint32 index = meshBuilder.addVertex(pos);

                    if (normals)
                    {
                        meshBuilder.setNormal(index, iaVector3f(0.0f, 1.0f, 0.0f));
                    }

                    if (texCoords)
                    {
                        meshBuilder.setTexCoord(index, iaVector2f(u, v), 0);
                    }
                }
            }

            // Add triangles
            for (uint32 z = 0; z < segmentsZ; ++z)
            {
                for (uint32 x = 0; x < segmentsX; ++x)
                {
                    uint32 i0 = offsetIndex + z * (segmentsX + 1) + x;
                    uint32 i1 = i0 + 1;
                    uint32 i2 = i0 + (segmentsX + 1);
                    uint32 i3 = i2 + 1;

                    // Triangle 1: i0, i2, i1
                    meshBuilder.addTriangle(i0, i2, i1, 0);
                    // Triangle 2: i1, i2, i3
                    meshBuilder.addTriangle(i1, i2, i3, 0);
                }
            }
        }

        void addSphere(iMeshBuilder &meshBuilder, float32 radius, uint32 segments, bool texCoords)
        {
            con_assert(segments >= 4, "parameters out of range");
            con_assert(radius > 0.0f, "parameters out of range");

            const float32 stepLon = (2.0f * M_PI) / segments;
            const float32 stepLat = M_PI / segments;

            const uint32 offsetIndex = meshBuilder.getVertexCount();

            // generate vertices
            for (uint32 level = 1; level < segments; ++level)
            {
                float v = level * stepLat;
                float sinvr = sin(v) * radius;
                float texV = 1.0f - (float32(level) / segments); // flip V to match top-down texture mapping

                for (uint32 segment = 0; segment < segments; ++segment)
                {
                    float u = segment * stepLon;

                    iaVector3f vec(cos(u) * sinvr, cos(v) * radius, sin(u) * sinvr);
                    uint32 index = meshBuilder.addVertex(vec);
                    vec.normalize();
                    meshBuilder.setNormal(index, vec);
                    float texU = float32(segment) / segments;

                    if (texCoords)
                    {
                        meshBuilder.setTexCoord(index, iaVector2f(texU, texV), 0);
                    }
                }
            }

            // bottom pole
            uint32 bottomIndex = meshBuilder.addVertex(iaVector3f(0, -radius, 0));
            meshBuilder.setNormal(bottomIndex, iaVector3f(0, -1, 0));
            if (texCoords)
            {
                meshBuilder.setTexCoord(bottomIndex, iaVector2f(0.5f, 0.0f), 0); // bottom center
            }

            // top pole
            uint32 topIndex = meshBuilder.addVertex(iaVector3f(0, radius, 0));
            meshBuilder.setNormal(topIndex, iaVector3f(0, 1, 0));
            if (texCoords)
            {
                meshBuilder.setTexCoord(topIndex, iaVector2f(0.5f, 1.0f), 0); // top center
            }

            // top and bottom triangles
            uint32 topOffset = bottomIndex - segments;
            for (uint32 segment = 0; segment < segments; ++segment)
            {
                meshBuilder.addTriangle(segment, topIndex, (segment + 1) % segments, offsetIndex);
                meshBuilder.addTriangle(topOffset + (segment + 1) % segments, bottomIndex, topOffset + segment, offsetIndex);
            }

            // belly triangles
            for (uint32 level = 0; level < segments - 2; ++level)
            {
                uint32 levelOffset = level * segments;

                for (uint32 segment = 0; segment < segments; ++segment)
                {
                    uint32 i0 = offsetIndex + levelOffset + segment;
                    uint32 i1 = offsetIndex + levelOffset + (segment + 1) % segments;
                    uint32 i2 = i0 + segments;
                    uint32 i3 = i1 + segments;

                    meshBuilder.addTriangle(i0, i1, i2, 0);
                    meshBuilder.addTriangle(i2, i1, i3, 0);
                }
            }
        }

        void addCylinder(iMeshBuilder &meshBuilder, float32 radius, float32 height, uint32 segments, bool hasCaps)
        {
            con_assert(segments >= 3, "parameters out of range");
            con_assert(radius > 0.0f, "parameters out of range");
            con_assert(height > 0.0f, "parameters out of range");

            const float32 step = (2.0f * M_PI) / segments;

            const uint32 offsetIndex = meshBuilder.getVertexCount();

            for (uint32 i = 0; i < segments; ++i)
            {
                meshBuilder.addVertex(iaVector3f(sin(i * step) * radius, 0, cos(i * step) * radius));
                meshBuilder.addVertex(iaVector3f(sin(i * step) * radius, height, cos(i * step) * radius));
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
                meshBuilder.addTriangle((segmentIndex + 2) % moduloValue, (segmentIndex + 1) % moduloValue, (segmentIndex + 0) % moduloValue, offsetIndex);
                meshBuilder.addTriangle((segmentIndex + 2) % moduloValue, (segmentIndex + 3) % moduloValue, (segmentIndex + 1) % moduloValue, offsetIndex);

                if (hasCaps)
                {
                    meshBuilder.addTriangle(((segmentIndex + 0) % moduloValue) + offsetIndex, bottomIndex, ((segmentIndex + 2) % moduloValue) + offsetIndex);
                    meshBuilder.addTriangle(((segmentIndex + 3) % moduloValue) + offsetIndex, topIndex, ((segmentIndex + 1) % moduloValue) + offsetIndex);
                }
            }
        }

        void addBox(iMeshBuilder &meshBuilder, float32 width, float32 height, float32 depth)
        {
            con_assert(width > 0.0f, "parameters out of range");
            con_assert(height > 0.0f, "parameters out of range");
            con_assert(depth > 0.0f, "parameters out of range");

            const uint32 offsetIndex = meshBuilder.getVertexCount();

            float32 halfWidth = width * 0.5;
            float32 halfDepth = depth * 0.5;

            meshBuilder.addVertex(iaVector3f(-halfWidth, 0, halfDepth));
            meshBuilder.addVertex(iaVector3f(-halfWidth, height, halfDepth));
            meshBuilder.addVertex(iaVector3f(halfWidth, height, halfDepth));
            meshBuilder.addVertex(iaVector3f(halfWidth, 0, halfDepth));
            meshBuilder.addVertex(iaVector3f(-halfWidth, 0, -halfDepth));
            meshBuilder.addVertex(iaVector3f(-halfWidth, height, -halfDepth));
            meshBuilder.addVertex(iaVector3f(halfWidth, height, -halfDepth));
            meshBuilder.addVertex(iaVector3f(halfWidth, 0, -halfDepth));

            meshBuilder.addTriangle(2, 1, 0, offsetIndex);
            meshBuilder.addTriangle(3, 2, 0, offsetIndex);
            meshBuilder.addTriangle(7, 4, 5, offsetIndex);
            meshBuilder.addTriangle(6, 7, 5, offsetIndex);
            meshBuilder.addTriangle(2, 5, 1, offsetIndex);
            meshBuilder.addTriangle(2, 6, 5, offsetIndex);
            meshBuilder.addTriangle(3, 0, 4, offsetIndex);
            meshBuilder.addTriangle(7, 3, 4, offsetIndex);
            meshBuilder.addTriangle(7, 2, 3, offsetIndex);
            meshBuilder.addTriangle(6, 2, 7, offsetIndex);
            meshBuilder.addTriangle(4, 0, 1, offsetIndex);
            meshBuilder.addTriangle(5, 4, 1, offsetIndex);
        }

        void addCone(iMeshBuilder &meshBuilder, float32 radius, float32 height, uint32 segments)
        {
            con_assert(height > 0.0f, "parameters out of range");
            con_assert(radius > 0.0f, "parameters out of range");
            con_assert(segments >= 3, "parameters out of range");

            const float32 step = (2.0f * M_PI) / segments;
            const uint32 offsetIndex = meshBuilder.getVertexCount();

            for (uint32 i = 0; i < segments; ++i)
            {
                meshBuilder.addVertex(iaVector3f(sin(i * step), 0, cos(i * step)));
            }

            const uint32 bottomIndex = meshBuilder.addVertex(iaVector3f(0, 0, 0));
            const uint32 tipIndex = meshBuilder.addVertex(iaVector3f(0, height, 0));

            const uint32 moduloValue = segments;

            for (uint32 i = 0; i < segments; ++i)
            {
                uint32 segmentIndex = i;
                meshBuilder.addTriangle(((segmentIndex + 1) % moduloValue) + offsetIndex, tipIndex, ((segmentIndex + 0) % moduloValue) + offsetIndex);
                meshBuilder.addTriangle(((segmentIndex + 0) % moduloValue) + offsetIndex, bottomIndex, ((segmentIndex + 1) % moduloValue) + offsetIndex);
            }
        }

        void addCircle(iMeshBuilder &meshBuilder, float32 radius, uint32 segments)
        {
            con_assert(radius > 0.0f, "parameters out of range");
            con_assert(segments >= 3, "parameters out of range");

            const float32 step = (2.0f * M_PI) / segments;
            const uint32 offsetIndex = meshBuilder.getVertexCount();

            for (uint32 i = 0; i < segments; ++i)
            {
                meshBuilder.addVertex(iaVector3f(sin(i * step), 0, cos(i * step)));
            }

            const uint32 centerIndex = meshBuilder.addVertex(iaVector3f(0, 0, 0));
            const uint32 moduloValue = segments;

            for (uint32 segmentIndex = 0; segmentIndex < segments; ++segmentIndex)
            {
                meshBuilder.addTriangle(((segmentIndex + 1) % moduloValue) + offsetIndex, centerIndex, ((segmentIndex + 0) % moduloValue) + offsetIndex);
            }
        }

        void addRing(iMeshBuilder &meshBuilder, float32 innerRadius, float32 outerRadius, uint32 segments)
        {
            con_assert(innerRadius < outerRadius, "parameters out of range");
            con_assert(innerRadius > 0.0f, "parameters out of range");
            con_assert(segments >= 3, "parameters out of range");

            const float32 step = (2.0f * M_PI) / segments;
            const uint32 offsetIndex = meshBuilder.getVertexCount();

            for (uint32 i = 0; i < segments; ++i)
            {
                meshBuilder.addVertex(iaVector3f(sin(i * step) * innerRadius, 0, cos(i * step) * innerRadius));
                meshBuilder.addVertex(iaVector3f(sin(i * step) * outerRadius, 0, cos(i * step) * outerRadius));
            }

            const uint32 segmentVertices = 2;
            const uint32 moduloValue = segments * segmentVertices;

            for (uint32 segment = 0; segment < segments; ++segment)
            {
                uint32 segmentIndex = segment * segmentVertices;
                meshBuilder.addTriangle((segmentIndex + 0) % moduloValue, (segmentIndex + 1) % moduloValue, (segmentIndex + 2) % moduloValue, offsetIndex);
                meshBuilder.addTriangle((segmentIndex + 1) % moduloValue, (segmentIndex + 3) % moduloValue, (segmentIndex + 2) % moduloValue, offsetIndex);
            }
        }

        void addMesh(iMeshBuilder &meshBuilder, const iMeshPtr mesh)
        {
            if (!mesh->isKeepingRawData())
            {
                return;
            }

            if (meshBuilder.isJoiningVertices())
            {
                con_err("can't add mesh in join vertexes mode");
                return;
            }

            const uint32 offsetIndex = meshBuilder.getVertexCount();

            void *indexData;
            void *vertexData;
            uint32 vertexDataSize;
            uint32 indexDataSize;

            mesh->getRawData(indexData, indexDataSize, vertexData, vertexDataSize);
            const auto &layout = mesh->getLayout();

            const uint32 vertexCount = mesh->getVertexCount();

            // iterate vertices
            for (uint32 i = 0; i < vertexCount; ++i)
            {
                float32 *vertex = static_cast<float32 *>(vertexData) + (layout.getStride() * i);

                uint32 index = meshBuilder.addVertex(iaVector3f(vertex[0], vertex[1], vertex[2]));

                uint32 offset = 0;

                if (mesh->hasNormals())
                {
                    meshBuilder.setNormal(index, iaVector3f(vertex[3], vertex[4], vertex[5]));
                    offset += 3;
                }

                if (mesh->hasColors())
                {
                    meshBuilder.setColor(index, iaColor4f(vertex[offset + 3], vertex[offset + 4], vertex[offset + 5], vertex[offset + 6]));
                    offset += 4;
                }

                for (uint32 t = 0; t < 4; ++t)
                {
                    if (mesh->hasTextureUnit(t))
                    {
                        meshBuilder.setTexCoord(index, iaVector2f(vertex[offset + 3], vertex[offset + 4]), t);
                        offset += 2;
                    }
                }
            }

            const uint32 trianglesCount = mesh->getTrianglesCount();

            // iterate triangles
            for (uint32 i = 0; i < trianglesCount; ++i)
            {
                // get triangle indeces
                uint32 triA = static_cast<float32 *>(indexData)[i * 3];
                uint32 triB = static_cast<float32 *>(indexData)[i * 3 + 1];
                uint32 triC = static_cast<float32 *>(indexData)[i * 3 + 2];

                // get vertices
                meshBuilder.addTriangle(triA, triB, triC, offsetIndex);
            }
        }

        void addTorus(iMeshBuilder &meshBuilder, float32 majorRadius, float32 minorRadiusX, float32 minorRadiusY, uint32 majorSegments, uint32 minorSegments)
        {
            con_assert(majorSegments >= 3 && minorSegments >= 3, "parameters out of range");
            con_assert(majorRadius > 0.0f && minorRadiusX > 0.0f && minorRadiusY > 0.0f, "parameters out of range");

            const float32 majorStep = (2.0f * M_PI) / majorSegments;
            const float32 minorStep = (2.0f * M_PI) / minorSegments;

            const uint32 offsetIndex = meshBuilder.getVertexCount();

            // Generate vertices and normals
            for (uint32 i = 0; i < majorSegments; ++i)
            {
                float theta = i * majorStep;
                float cosTheta = cos(theta);
                float sinTheta = sin(theta);

                for (uint32 j = 0; j < minorSegments; ++j)
                {
                    float phi = j * minorStep;
                    float cosPhi = cos(phi);
                    float sinPhi = sin(phi);

                    // Elliptical offsets
                    float dx = minorRadiusX * cosPhi;
                    float dy = minorRadiusY * sinPhi;

                    float x = (majorRadius + dx) * cosTheta;
                    float y = dy;
                    float z = (majorRadius + dx) * sinTheta;

                    iaVector3f pos(x, y, z);
                    meshBuilder.addVertex(pos);

                    // Normal (from torus ring center to ellipse surface point)
                    iaVector3f centerOnRing(majorRadius * cosTheta, 0, majorRadius * sinTheta);
                    iaVector3f normal = pos - centerOnRing;
                    normal.normalize();
                    meshBuilder.setNormal(meshBuilder.getVertexCount() - 1, normal);
                }
            }

            // Generate triangles with proper winding
            for (uint32 i = 0; i < majorSegments; ++i)
            {
                for (uint32 j = 0; j < minorSegments; ++j)
                {
                    uint32 iNext = (i + 1) % majorSegments;
                    uint32 jNext = (j + 1) % minorSegments;

                    uint32 a = offsetIndex + i * minorSegments + j;
                    uint32 b = offsetIndex + iNext * minorSegments + j;
                    uint32 c = offsetIndex + iNext * minorSegments + jNext;
                    uint32 d = offsetIndex + i * minorSegments + jNext;

                    meshBuilder.addTriangle(a, c, b);
                    meshBuilder.addTriangle(a, d, c);
                }
            }
        }

    };
};
