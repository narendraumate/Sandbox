//
//  SpatialBoundVisual.h
//
//
//  Created by Narendra Umate on 8/28/15.
//
//

#pragma once

#include "Buffer.h"
#include "Intersect.h"
#include "SpatialBound.h"
#include <cstring>
#include <list>
#include <vector>

class SpatialBoundVisual3 : public SpatialBound3 {
public:
	SpatialBoundVisual3(const Mesh& mesh, const Material& material);
	virtual ~SpatialBoundVisual3();

	virtual void setTranslate(const Vec3& translate);
	virtual void setRotate(const Quat& rotate);
	virtual void setScale(const Vec3& scale);

	void setViewProjectionMatrix(const Mat4& projectionMatrix, const Mat4& viewMatrix);

	void setModelBoundingBox(const std::vector<float>& positions) {
		Vec3 bbmin = Vec3::max;
		Vec3 bbmax = Vec3::min;
		unsigned int j = 0;
		unsigned long positionsCount = positions.size();
		for (j = 0; j < positionsCount;) {
			if (bbmin.x > positions[j]) {
				bbmin.x = positions[j];
			}
			if (bbmax.x < positions[j]) {
				bbmax.x = positions[j];
			}
			j++;
			if (bbmin.y > positions[j]) {
				bbmin.y = positions[j];
			}
			if (bbmax.y < positions[j]) {
				bbmax.y = positions[j];
			}
			j++;
			if (bbmin.z > positions[j]) {
				bbmin.z = positions[j];
			}
			if (bbmax.z < positions[j]) {
				bbmax.z = positions[j];
			}
			j++;
		}
		m_modelBoundingBox.updateMinMax(bbmin, bbmax);
		updateWorldBoundingBox();
	}

	Mat3& getWorldViewNormMatrix() {
		return m_worldViewNormMatrix;
	}

	Mat4& getViewMatrix() {
		return m_viewMatrix;
	}

	Mat4& getWorldViewProjectionMatrix() {
		return m_worldViewProjectionMatrix;
	}

	VisualMaterial* getVisualMaterial() {
		return m_visualMaterial;
	}

	VertexBuffer* getVertexBuffer() {
		return m_vertexBuffer;
	}

	IndexBuffer* getIndexBuffer(const int& index) {
		return m_indexBuffers[index];
	}

	int getIndexBufferCount() {
		int count = 0;
		count += m_indexBuffers[0] != nullptr ? 1 : 0;
		count += m_indexBuffers[1] != nullptr ? 1 : 0;
		count += m_indexBuffers[2] != nullptr ? 1 : 0;
		count += m_indexBuffers[3] != nullptr ? 1 : 0;
		count += m_indexBuffers[4] != nullptr ? 1 : 0;
		return count;
	}

protected:
	void updateWorldViewNormMatrix();
	void updateWorldViewProjectionMatrix();

	VisualMaterial* m_visualMaterial;

	VertexBuffer* m_vertexBuffer;
	IndexBuffer* m_indexBuffers[5];

private:
	Mat3 m_worldViewNormMatrix;

	Mat4 m_viewMatrix;
	Mat4 m_viewProjectionMatrix;
	Mat4 m_worldViewProjectionMatrix;
};
