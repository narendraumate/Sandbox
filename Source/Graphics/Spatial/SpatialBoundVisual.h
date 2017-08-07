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
	SpatialBoundVisual3(const size_t& hash, const Mesh& mesh, const Material& material);
	virtual ~SpatialBoundVisual3();

	virtual void setTranslate(const Vec3& translate);
	virtual void setRotate(const Quat& rotate);
	virtual void setScale(const float& scale);

	void setViewProjectionMatrix(const Mat4& viewProjectionMatrix, const Mat4& viewMatrix);

	Mat3& getWorldNormalMatrix() {
		return m_worldNormalMatrix;
	}

	Mat4& getStraightViewMatrix() {
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
	void updateWorldNormalMatrix();
	void updateWorldViewProjectionMatrix();

	VisualMaterial* m_visualMaterial;

	VertexBuffer* m_vertexBuffer;
	IndexBuffer* m_indexBuffers[5];

private:
	Mat3 m_worldNormalMatrix;

	Mat4 m_viewMatrix;
	Mat4 m_viewProjectionMatrix;
	Mat4 m_worldViewProjectionMatrix;
};
