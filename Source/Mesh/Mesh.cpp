//
//  Mesh.cpp
//
//
//  Created by Narendra Umate on 8/6/16.
//
//

#include "Mesh.h"

#include "MeshObj.h"

#include "Platform.h"

bool readMesh(const std::string& filepath, std::vector<Mesh>& mesh, std::vector<Material>& material) {
	if (fileExists(filepath)) {
		std::string filename = findFileName(filepath);
		if (filename.find(".obj") != std::string::npos) {
			return readObj(filepath, mesh, material);
		}
	}
	return false;
}

void printMesh(Mesh* mesh, Material* material) {
#if defined UNOPTIMIZED
	if (mesh) {
		std::cout << "Vertex Buffer:" << std::endl;
		unsigned long vertexSize = 0;
		float* vertexBuffer = nullptr;
		mesh->getVertexBuffer(vertexSize, vertexBuffer);
		// P4 N4 T4 B4 T2 P2
		for (uint32_t i = 0; i < vertexSize / sizeof(float); i = i + 20) {
			std::cout << "I: " << i / 20 << " ";
			std::cout << "P: " << vertexBuffer[i] << " " << vertexBuffer[i + 1] << " " << vertexBuffer[i + 2] << " " << vertexBuffer[i + 3] << " ";
			std::cout << "N: " << vertexBuffer[i + 4] << " " << vertexBuffer[i + 5] << " " << vertexBuffer[i + 6] << " " << vertexBuffer[i + 7] << " ";
			std::cout << "T: " << vertexBuffer[i + 8] << " " << vertexBuffer[i + 9] << " " << vertexBuffer[i + 10] << " " << vertexBuffer[i + 11] << " ";
			std::cout << "B: " << vertexBuffer[i + 12] << " " << vertexBuffer[i + 13] << " " << vertexBuffer[i + 14] << " " << vertexBuffer[i + 15] << " ";
			std::cout << "T: " << vertexBuffer[i + 16] << " " << vertexBuffer[i + 17] << " ";
			std::cout << "P: " << vertexBuffer[i + 18] << " " << vertexBuffer[i + 19] << std::endl;
		}

		unsigned long pointSize = 0;
		uint32_t* pointIndices = nullptr;
		mesh->getPointIndexBuffer(pointSize, pointIndices);
		if (pointSize) {
			std::cout << "Point Index Buffer:" << std::endl;
			for (uint32_t i = 0; i < pointSize; i = i + 1) {
				std::cout << "Indices: " << pointIndices[i] << std::endl;
			}
		}

		unsigned long lineSize = 0;
		uint32_t* lineIndices = nullptr;
		mesh->getEdgeIndexBuffer(lineSize, lineIndices);
		if (lineSize) {
			std::cout << "Edge Index Buffer:" << std::endl;
			for (uint32_t i = 0; i < lineSize; i = i + 2) {
				std::cout << "Indices: " << lineIndices[i] << " " << lineIndices[i + 1] << std::endl;
			}
		}

		unsigned long triangleSize = 0;
		uint32_t* triangleIndices = nullptr;
		mesh->getTriangleIndexBuffer(triangleSize, triangleIndices);
		if (triangleSize) {
			std::cout << "Triangle Index Buffer:" << std::endl;
			for (uint32_t i = 0; i < triangleSize; i = i + 3) {
				std::cout << "Indices: " << triangleIndices[i] << " " << triangleIndices[i + 1] << " " << triangleIndices[i + 2] << std::endl;
			}
		}
	}
#endif // defined UNOPTIMIZED
}
