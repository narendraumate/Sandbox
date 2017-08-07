//
//  MeshPly.cpp
//
//
//  Created by Narendra Umate on 8/6/16.
//
//

#include <list>
#include <queue>
#include <sstream>

#include "MeshPly.h"
#include "Platform.h"

// Reference http://paulbourke.net/dataformats/ply/

bool readPly(const std::string& filepath, std::vector<Mesh>& meshes, std::vector<Material>& materials) {
	// TODO Support Multiple Meshes.
	Mesh mesh;
	Material material;

	std::ifstream filehandle;
	if (fileOpen(filepath, filehandle)) {
		std::string line;
		std::string word;
		std::istringstream iss;
		std::queue<std::list<std::string>> cqm;

		unsigned int vertexCount = 0;
		unsigned int faceCount = 0;
		unsigned int edgeCount = 0;
		unsigned int materialCount = 0;

		bool endHeader = false;
		while (fileReadLine(filehandle, line)) {
			if (line.find("ply") != std::string::npos
				|| line.find("format") != std::string::npos
				|| line.find("comment") != std::string::npos
				|| line.find("obj_info") != std::string::npos) {
				continue;
			}
			if (!endHeader) {
				if (line.find("element vertex") != std::string::npos) {
					// Vertex Count
					// Vertex Format
					iss.str(line);
					iss >> word;
					iss >> word;
					iss >> vertexCount;
					iss.clear();
					std::list<std::string> cqs;
					while (fileReadLine(filehandle, line) && line.find("property") != std::string::npos) {
						iss.str(line);
						iss >> word;
						std::string datatype;
						iss >> datatype;
						std::string variable;
						iss >> variable;
						iss.clear();
						if (variable.compare("x") == 0 || variable.compare("X") == 0) {
							fileReadLine(filehandle, line);
							fileReadLine(filehandle, line);
							cqs.push_back("vertex.position");
						}
						if (variable.compare("nx") == 0 || variable.compare("NX") == 0) {
							fileReadLine(filehandle, line);
							fileReadLine(filehandle, line);
							cqs.push_back("vertex.normal");
						}
						if (variable.compare("u") == 0 || variable.compare("U") == 0) {
							fileReadLine(filehandle, line);
							cqs.push_back("vertex.texcoord");
						}
						if (variable.compare("red") == 0 || variable.compare("RED") == 0) {
							fileReadLine(filehandle, line);
							fileReadLine(filehandle, line);
							cqs.push_back("vertex.color");
						}
						if (variable.compare("material_index") == 0 || variable.compare("MATERIAL_INDEX") == 0) {
							cqs.push_back("vertex.material");
						}
					}
					for (unsigned int i = 0; i < vertexCount; ++i) {
						cqm.push(cqs);
					}
				}
				if (line.find("end_header") != std::string::npos) {
					endHeader = true;
					continue;
				}
				if (line.find("element face") != std::string::npos || line.find("element range_grid") != std::string::npos) {
					// Face Count
					// Face Format
					iss.str(line);
					iss >> word;
					iss >> word;
					iss >> faceCount;
					iss.clear();
					std::list<std::string> cqs;
					if (fileReadLine(filehandle, line) && line.find("property") != std::string::npos) {
						iss.str(line);
						iss >> word;
						iss >> word;
						std::string counttype;
						iss >> counttype;
						std::string indextype;
						iss >> indextype;
						iss >> word;
						iss.clear();
						cqs.push_back("face.indices");
					}
					for (unsigned int i = 0; i < faceCount; ++i) {
						cqm.push(cqs);
					}
				}
				if (line.find("end_header") != std::string::npos) {
					endHeader = true;
					continue;
				}
				if (line.find("element edge") != std::string::npos) {
					// Edge Count
					// Edge Format
					iss.str(line);
					iss >> word;
					iss >> word;
					iss >> edgeCount;
					iss.clear();
					std::list<std::string> cqs;
					while (fileReadLine(filehandle, line) && line.find("property") != std::string::npos) {
						iss.str(line);
						iss >> word;
						std::string datatype;
						iss >> datatype;
						std::string variable;
						iss >> variable;
						iss.clear();
						if (variable.compare("vertex1") == 0 || variable.compare("VERTEX1") == 0) {
							fileReadLine(filehandle, line);
							cqs.push_back("edge.indices");
						}
						if (variable.compare("red") == 0 || variable.compare("RED") == 0) {
							fileReadLine(filehandle, line);
							fileReadLine(filehandle, line);
							cqs.push_back("edge.color");
						}
					}
					for (unsigned int i = 0; i < edgeCount; ++i) {
						cqm.push(cqs);
					}
				}
				if (line.find("end_header") != std::string::npos) {
					endHeader = true;
					continue;
				}
				if (line.find("element material") != std::string::npos) {
					// Material Count
					// Material Format
					iss.str(line);
					iss >> word;
					iss >> word;
					iss >> materialCount;
					iss.clear();
					std::list<std::string> cqs;
					fileReadLine(filehandle, line);
					fileReadLine(filehandle, line);
					fileReadLine(filehandle, line);
					fileReadLine(filehandle, line);
					fileReadLine(filehandle, line);
					fileReadLine(filehandle, line);
					fileReadLine(filehandle, line);
					fileReadLine(filehandle, line);
					fileReadLine(filehandle, line);
					fileReadLine(filehandle, line);
					fileReadLine(filehandle, line);
					fileReadLine(filehandle, line);
					fileReadLine(filehandle, line);
					cqs.push_back("material");
					for (unsigned int i = 0; i < materialCount; ++i) {
						cqm.push(cqs);
					}
				}
				if (line.find("end_header") != std::string::npos) {
					endHeader = true;
					continue;
				}
			}
			if (endHeader) {
				unsigned int vertexIndex = 0;

				mesh.vertices.resize(vertexCount);

				while (!cqm.empty()) {
					bool vertexRow = false;
					////bool faceRow = false;
					////bool edgeRow = false;
					////bool materialRow = false;

					iss.str(line);
					for (std::list<std::string>::iterator cqi = cqm.front().begin(); cqi != cqm.front().end(); ++cqi) {
						if (cqi->compare("vertex.position") == 0) {
							vertexRow = true;
							iss >> mesh.vertices[vertexIndex].position.x;
							iss >> mesh.vertices[vertexIndex].position.y;
							iss >> mesh.vertices[vertexIndex].position.z;
						} else if (cqi->compare("vertex.normal") == 0) {
							vertexRow = true;
							iss >> mesh.vertices[vertexIndex].normal.x;
							iss >> mesh.vertices[vertexIndex].normal.y;
							iss >> mesh.vertices[vertexIndex].normal.z;
						} else if (cqi->compare("vertex.texcoord") == 0) {
							vertexRow = true;
							iss >> mesh.vertices[vertexIndex].texcoord.x;
							iss >> mesh.vertices[vertexIndex].texcoord.y;
						} else if (cqi->compare("vertex.color") == 0) {
							vertexRow = true;
							iss >> word;
							iss >> word;
							iss >> word;
						} else if (cqi->compare("face.indices") == 0) {
							////faceRow = true;
							unsigned int size = 0;
							iss >> size;
							switch (size) {
								case 1: {
									unsigned int point0 = 0;
									iss >> point0;
									mesh.primitives[0].points.indices.push_back(point0);
								} break;
								case 3: {
									unsigned int triangle0 = 0;
									iss >> triangle0;
									mesh.primitives[0].triangles.indices.push_back(triangle0);
									unsigned int triangle1 = 0;
									iss >> triangle1;
									mesh.primitives[0].triangles.indices.push_back(triangle1);
									unsigned int triangle2 = 0;
									iss >> triangle2;
									mesh.primitives[0].triangles.indices.push_back(triangle2);
								} break;
								case 4: {
									unsigned int quadrilateral0 = 0;
									iss >> quadrilateral0;
									mesh.primitives[0].quadrilaterals.indices.push_back(quadrilateral0);
									unsigned int quadrilateral1 = 0;
									iss >> quadrilateral1;
									mesh.primitives[0].quadrilaterals.indices.push_back(quadrilateral1);
									unsigned int quadrilateral2 = 0;
									iss >> quadrilateral2;
									mesh.primitives[0].quadrilaterals.indices.push_back(quadrilateral2);
									unsigned int quadrilateral3 = 0;
									iss >> quadrilateral3;
									mesh.primitives[0].quadrilaterals.indices.push_back(quadrilateral3);
								} break;
								default: {
									mesh.primitives[0].faces.counts.push_back(size);
									for (unsigned int i = 0; i < size; ++i) {
										unsigned int value = 0;
										iss >> value;
										mesh.primitives[0].faces.indices.push_back(value);
									}
								} break;
							}

						} else if (cqi->compare("edge.indices") == 0) {
							////edgeRow = true;
							unsigned int edge0 = 0;
							iss >> edge0;
							mesh.primitives[0].edges.indices.push_back(edge0);
							unsigned int edge1 = 0;
							iss >> edge1;
							mesh.primitives[0].edges.indices.push_back(edge1);
						} else if (cqi->compare("edge.color") == 0) {
							////edgeRow = true;
							iss >> word;
							iss >> word;
							iss >> word;
						} else if (cqi->compare("material") == 0) {
							////materialRow = true;
							// TODO Implement this
						}
					}
					iss.clear();

					if (vertexRow) {
						vertexIndex++;
					}

					fileReadLine(filehandle, line);
					cqm.pop();
				}
			}
		}
		fileClose(filehandle);
		meshes.push_back(mesh);
		materials.push_back(material);
		return true;
	}
	return false;
}
