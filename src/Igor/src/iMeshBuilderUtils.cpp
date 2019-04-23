// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iMeshBuilderUtils.h>

namespace Igor
{
	namespace iMeshBuilderUtils
	{

		void addRectangle(iMeshBuilder& meshBuilder, float32 x, float32 z, float32 sizeX, float32 sizeZ)
		{
			const uint32 offsetIndex = meshBuilder.getVertexCount();

			meshBuilder.addVertex(iaVector3f(x, 0, z + sizeZ));
			meshBuilder.addVertex(iaVector3f(x + sizeX, 0, z + sizeZ));
			meshBuilder.addVertex(iaVector3f(x + sizeX, 0, z));
			meshBuilder.addVertex(iaVector3f(x, 0, z));

			meshBuilder.addTriangle(0, 1, 2, offsetIndex);
			meshBuilder.addTriangle(2, 3, 0, offsetIndex);
		}

		void addSphere(iMeshBuilder & meshBuilder, float32 radius, uint32 segments)
		{
			con_assert(segments >= 4, "parameters out of range");
			con_assert(radius > 0.0f, "parameters out of range");

			const float32 stepLon = (2.0f * M_PI) / segments;
			const float32 stepLat = M_PI / segments;

			const uint32 offsetIndex = meshBuilder.getVertexCount();

			// skip first and 6th
			for (uint32 level = 1; level < segments; ++level)
			{
				float v = level * stepLat;
				float sinvr = sin(v) * radius;

				for (uint32 segment = 0; segment < segments; ++segment)
				{
					float u = segment * stepLon;
					
					iaVector3f vec(cos(u) * sinvr, cos(v) * radius, sin(u) * sinvr);
					meshBuilder.addVertex(vec);
					vec.normalize();
					meshBuilder.setNormal(meshBuilder.getVertexCount() - 1, vec);
				}
			}

			uint32 bottomIndex = meshBuilder.addVertex(iaVector3f(0, -radius, 0));
			meshBuilder.setNormal(bottomIndex, iaVector3f(0, -1, 0));
			uint32 topIndex = meshBuilder.addVertex(iaVector3f(0, radius, 0));
			meshBuilder.setNormal(topIndex, iaVector3f(0, -1, 0));

			uint32 topOffset = bottomIndex - segments;
			for (uint32 segment = 0; segment < segments; ++segment)
			{
				meshBuilder.addTriangle(segment, topIndex, (segment + 1) % segments, offsetIndex);
				meshBuilder.addTriangle(topOffset + (segment + 1) % segments, bottomIndex, topOffset + segment, offsetIndex);
			}

			for (uint32 level = 0; level < segments - 2; ++level)
			{
				uint32 levelOffset = level * segments;

				for (uint32 segment = 0; segment < segments; ++segment)
				{
					meshBuilder.addTriangle(segment, (segment + 1) % segments, segment + segments, offsetIndex + levelOffset);
					meshBuilder.addTriangle(segment + segments, (segment + 1) % segments, (segment + 1) % segments + segments, offsetIndex + levelOffset);
				}
			}
		}

		void addCylinder(iMeshBuilder & meshBuilder, float32 radius, float32 height, uint32 segments, bool hasCaps)
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

		void addBox(iMeshBuilder & meshBuilder, float32 width, float32 height, float32 depth)
		{
			con_assert(width > 0.0f, "parameters out of range");
			con_assert(height > 0.0f, "parameters out of range");
			con_assert(depth > 0.0f, "parameters out of range");

			const uint32 offsetIndex = meshBuilder.getVertexCount();

			meshBuilder.addVertex(iaVector3f(-0.5, 0, 0.5));
			meshBuilder.addVertex(iaVector3f(-0.5, 1, 0.5));
			meshBuilder.addVertex(iaVector3f(0.5, 1, 0.5));
			meshBuilder.addVertex(iaVector3f(0.5, 0, 0.5));
			meshBuilder.addVertex(iaVector3f(-0.5, 0, -0.5));
			meshBuilder.addVertex(iaVector3f(-0.5, 1, -0.5));
			meshBuilder.addVertex(iaVector3f(0.5, 1, -0.5));
			meshBuilder.addVertex(iaVector3f(0.5, 0, -0.5));

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

		void addCone(iMeshBuilder & meshBuilder, float32 radius, float32 height, uint32 segments)
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

		void addCircle(iMeshBuilder & meshBuilder, float32 radius, uint32 segments)
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

		void addRing(iMeshBuilder & meshBuilder, float32 innerRadius, float32 outerRadius, uint32 segments)
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
	};
};
