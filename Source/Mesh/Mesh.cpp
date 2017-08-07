//
//  Mesh.cpp
//
//
//  Created by Narendra Umate on 8/6/16.
//
//

#include "Mesh.h"

#include "MeshPly.h"

#include "MeshObj.h"

#include "Platform.h"

bool readMesh(const std::string& filepath, std::vector<Mesh>& mesh, std::vector<Material>& material) {
	if (fileExists(filepath)) {
		std::string filename = findFileName(filepath);
		if (filename.find(".ply") != std::string::npos) {
			return readPly(filepath, mesh, material);
		} else if (filename.find(".obj") != std::string::npos) {
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
		for (unsigned int i = 0; i < vertexSize / sizeof(float); i = i + 20) {
			std::cout << "I: " << i / 20 << " ";
			std::cout << "P: " << vertexBuffer[i] << " " << vertexBuffer[i + 1] << " " << vertexBuffer[i + 2] << " " << vertexBuffer[i + 3] << " ";
			std::cout << "N: " << vertexBuffer[i + 4] << " " << vertexBuffer[i + 5] << " " << vertexBuffer[i + 6] << " " << vertexBuffer[i + 7] << " ";
			std::cout << "T: " << vertexBuffer[i + 8] << " " << vertexBuffer[i + 9] << " " << vertexBuffer[i + 10] << " " << vertexBuffer[i + 11] << " ";
			std::cout << "B: " << vertexBuffer[i + 12] << " " << vertexBuffer[i + 13] << " " << vertexBuffer[i + 14] << " " << vertexBuffer[i + 15] << " ";
			std::cout << "T: " << vertexBuffer[i + 16] << " " << vertexBuffer[i + 17] << " ";
			std::cout << "P: " << vertexBuffer[i + 18] << " " << vertexBuffer[i + 19] << std::endl;
		}

		unsigned long pointSize = 0;
		unsigned int* pointIndices = nullptr;
		mesh->getPointIndexBuffer(pointSize, pointIndices);
		if (pointSize) {
			std::cout << "Point Index Buffer:" << std::endl;
			for (unsigned int i = 0; i < pointSize; i = i + 1) {
				std::cout << "Indices: " << pointIndices[i] << std::endl;
			}
		}

		unsigned long edgeSize = 0;
		unsigned int* edgeIndices = nullptr;
		mesh->getEdgeIndexBuffer(edgeSize, edgeIndices);
		if (edgeSize) {
			std::cout << "Edge Index Buffer:" << std::endl;
			for (unsigned int i = 0; i < edgeSize; i = i + 2) {
				std::cout << "Indices: " << edgeIndices[i] << " " << edgeIndices[i + 1] << std::endl;
			}
		}

		unsigned long triangleSize = 0;
		unsigned int* triangleIndices = nullptr;
		mesh->getTriangleIndexBuffer(triangleSize, triangleIndices);
		if (triangleSize) {
			std::cout << "Triangle Index Buffer:" << std::endl;
			for (unsigned int i = 0; i < triangleSize; i = i + 3) {
				std::cout << "Indices: " << triangleIndices[i] << " " << triangleIndices[i + 1] << " " << triangleIndices[i + 2] << std::endl;
			}
		}

		unsigned long quadrilateralSize = 0;
		unsigned int* quadrilateralIndices = nullptr;
		mesh->getQuadrilateralIndexBuffer(quadrilateralSize, quadrilateralIndices);
		if (quadrilateralSize) {
			std::cout << "Quadrilateral Index Buffer:" << std::endl;
			for (unsigned int i = 0; i < quadrilateralSize; i = i + 4) {
				std::cout << "Indices: " << quadrilateralIndices[i] << " " << quadrilateralIndices[i + 1] << " " << quadrilateralIndices[i + 2] << " " << quadrilateralIndices[i + 3] << std::endl;
			}
		}

		unsigned long faceSize;
		unsigned int* faceCounts;
		unsigned int* faceIndices;
		mesh->getFaceIndexBuffer(faceSize, faceCounts, faceIndices);
		if (faceSize) {
			std::cout << "Face Index Buffer:" << std::endl;
			unsigned int k = 0;
			for (unsigned int i = 0; i < faceSize; ++i) {
				std::cout << "Indices: ";
				for (unsigned int j = 0; j < faceCounts[i]; ++j) {
					std::cout << faceIndices[k] << " ";
					++k;
				}
				std::cout << std::endl;
			}
		}
	}
#endif // defined UNOPTIMIZED
}
