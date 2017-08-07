//
//  MeshObj.cpp
//
//
//  Created by Narendra Umate on 8/6/16.
//
//

#include "MeshObj.h"
#include "Platform.h"

#include "tiny_obj_loader.h"

bool readObj(const std::string& filepath, std::vector<Mesh>& meshes, std::vector<Material>& materials) {
	std::string filebasepath = findBasePath(filepath);
	tinyobj::ObjReader objReader;
	bool result = objReader.ParseFromFile(filepath);
	if (result && objReader.Valid()) {
		tinyobj::attrib_t attrib = objReader.GetAttrib();
		std::vector<tinyobj::shape_t> oldTshapes = objReader.GetShapes();
		std::vector<tinyobj::material_t> tmaterials = objReader.GetMaterials();

		std::cout << "OldT-Shapes size: " << oldTshapes.size() << std::endl;
		std::cout << "T-Materials size: " << tmaterials.size() << std::endl;

		std::vector<tinyobj::shape_t> newTshapes;

		if (oldTshapes.size() == tmaterials.size()) {
			// TODO do the checks.
			newTshapes = oldTshapes;
		} else {
			for (size_t oldTshapeIndex = 0; oldTshapeIndex < oldTshapes.size(); oldTshapeIndex++) {
				tinyobj::shape_t& curOldTshape = oldTshapes[oldTshapeIndex];

				std::vector<tinyobj::shape_t> curNewTshapes;
				curNewTshapes.resize(tmaterials.size());
				std::cout << "curNewTshapes: " << curNewTshapes.size() << std::endl;

				std::cout << "indices: " << curOldTshape.mesh.indices.size() << std::endl;
				std::cout << "nmfvert: " << curOldTshape.mesh.num_face_vertices.size() << std::endl;
				std::cout << "materia: " << curOldTshape.mesh.material_ids.size() << std::endl;
				std::cout << "smoothi: " << curOldTshape.mesh.smoothing_group_ids.size() << std::endl;
				std::cout << "tagstag: " << curOldTshape.mesh.tags.size() << std::endl;

				/*std::cout << "i" << std::endl;
				for (int i = 0; i < curOldTshape.mesh.indices.size(); ++i) {
					std::cout << curOldTshape.mesh.indices[i].vertex_index << std::endl;
				}*/

				/*std::cout << "n" << std::endl;
				for (int i = 0; i < curOldTshape.mesh.num_face_vertices.size(); ++i) {
					std::cout << curOldTshape.mesh.num_face_vertices[i] << std::endl;
				}*/

				/*std::cout << "m" << std::endl;
				for (int i = 0; i < curOldTshape.mesh.material_ids.size(); ++i) {
					std::cout << curOldTshape.mesh.material_ids[i] << std::endl;
				}*/

				/*std::cout << "s" << std::endl;
				for (int i = 0; i < curOldTshape.mesh.smoothing_group_ids.size(); ++i) {
					std::cout << curOldTshape.mesh.smoothing_group_ids[i] << std::endl;
				}*/

				std::map<int, int> map;
				for (int i = 0; i < curOldTshape.mesh.material_ids.size(); ++i) {
					map[curOldTshape.mesh.material_ids[i]]++;
				}

				for (int i = 0; i < curOldTshape.mesh.material_ids.size(); ++i) {
					curNewTshapes[curOldTshape.mesh.material_ids[i]].mesh.indices.push_back(curOldTshape.mesh.indices[3 * i + 0]);
					curNewTshapes[curOldTshape.mesh.material_ids[i]].mesh.indices.push_back(curOldTshape.mesh.indices[3 * i + 1]);
					curNewTshapes[curOldTshape.mesh.material_ids[i]].mesh.indices.push_back(curOldTshape.mesh.indices[3 * i + 2]);

					curNewTshapes[curOldTshape.mesh.material_ids[i]].mesh.num_face_vertices.push_back(curOldTshape.mesh.num_face_vertices[i]);
					curNewTshapes[curOldTshape.mesh.material_ids[i]].mesh.material_ids.push_back(curOldTshape.mesh.material_ids[i]);
					curNewTshapes[curOldTshape.mesh.material_ids[i]].mesh.smoothing_group_ids.push_back(curOldTshape.mesh.smoothing_group_ids[i]);
				}

				for (int i = 0; i < curNewTshapes.size(); ++i) {
					// std::cout << i << " " << map[i] << std::endl;
					curNewTshapes[i].name = curOldTshape.name;
					newTshapes.push_back(curNewTshapes[i]);
				}
			}
		}

		std::cout << "NewT-Shapes size: " << newTshapes.size() << std::endl;

		//..
		bool normalsPresent = attrib.normals.size() > 0;
		bool texcoordsPresent = attrib.texcoords.size() > 0;
		for (int tshapeindex = 0; tshapeindex < newTshapes.size(); ++tshapeindex) {
			auto& tshape = newTshapes[tshapeindex];

			Mesh mesh;
			Material material;

			// Fill out Mesh.
			std::vector<unsigned int>& indices = mesh.triangles[0].indices;
			indices.resize(tshape.mesh.indices.size());
			std::vector<P4N4T4B4T2P2>& vertices = mesh.vertices;
			vertices.resize(tshape.mesh.indices.size());
			for (unsigned int i = 0; i < tshape.mesh.indices.size(); ++i) {
				indices[i] = (i);
				tinyobj::index_t indicesTuple = tshape.mesh.indices[i];
				vertices[i].position.x = (attrib.vertices[indicesTuple.vertex_index * 3 + 0]);
				vertices[i].position.y = (attrib.vertices[indicesTuple.vertex_index * 3 + 1]);
				vertices[i].position.z = (attrib.vertices[indicesTuple.vertex_index * 3 + 2]);
				vertices[i].position.w = 1.0f;
				if (normalsPresent) {
					vertices[i].normal.x = (attrib.normals[indicesTuple.normal_index * 3 + 0]);
					vertices[i].normal.y = (attrib.normals[indicesTuple.normal_index * 3 + 1]);
					vertices[i].normal.z = (attrib.normals[indicesTuple.normal_index * 3 + 2]);
					vertices[i].normal.w = 0.0f;
					float length = sqrtf(vertices[i].normal[0] * vertices[i].normal[0] + vertices[i].normal[1] * vertices[i].normal[1] + vertices[i].normal[2] * vertices[i].normal[2]);
					vertices[i].normal[0] = vertices[i].normal[0] / length;
					vertices[i].normal[1] = vertices[i].normal[1] / length;
					vertices[i].normal[2] = vertices[i].normal[2] / length;
				}
				if (texcoordsPresent) {
					vertices[i].texcoord.x = attrib.texcoords[indicesTuple.texcoord_index * 2 + 0];
					vertices[i].texcoord.y = attrib.texcoords[indicesTuple.texcoord_index * 2 + 1];
				}
			}

			if (normalsPresent && texcoordsPresent) {
				// Calculate tangent and bitangent. This math can be improved upon a lot.
				for (unsigned int k = 0; k < indices.size(); k += 3) {
					// Shortcuts for vertices
					Vec4* v0 = &vertices[k].position;
					Vec4* v1 = &vertices[k + 1].position;
					Vec4* v2 = &vertices[k + 2].position;

					// Shortcuts for texcoords
					Vec2* uv0 = &vertices[k].texcoord;
					Vec2* uv1 = &vertices[k + 1].texcoord;
					Vec2* uv2 = &vertices[k + 2].texcoord;

					// position delta
					Vec4 v4deltaPos1 = *v1 - *v0;
					Vec4 v4deltaPos2 = *v2 - *v0;
					Vec3 deltaPos1 = Vec3(v4deltaPos1.x, v4deltaPos1.y, v4deltaPos1.z);
					Vec3 deltaPos2 = Vec3(v4deltaPos2.x, v4deltaPos2.y, v4deltaPos2.z);

					// texcoord delta
					Vec2 deltaUV1 = *uv1 - *uv0;
					Vec2 deltaUV2 = *uv2 - *uv0;

					float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
					Vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
					Vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;
					Vec3 normal = cross(deltaPos1, deltaPos2);

					vertices[k].normal += Vec4(normal.x, normal.y, normal.z, 0.0f);
					vertices[k + 1].normal += Vec4(normal.x, normal.y, normal.z, 0.0f);
					vertices[k + 2].normal += Vec4(normal.x, normal.y, normal.z, 0.0f);

					vertices[k].tangent += Vec4(tangent.x, tangent.y, tangent.z, 0.0f);
					vertices[k + 1].tangent += Vec4(tangent.x, tangent.y, tangent.z, 0.0f);
					vertices[k + 2].tangent += Vec4(tangent.x, tangent.y, tangent.z, 0.0f);

					vertices[k].bitangent += Vec4(bitangent.x, bitangent.y, bitangent.z, 0.0f);
					vertices[k + 1].bitangent += Vec4(bitangent.x, bitangent.y, bitangent.z, 0.0f);
					vertices[k + 2].bitangent += Vec4(bitangent.x, bitangent.y, bitangent.z, 0.0f);
				}

				for (unsigned int k = 0; k < vertices.size(); k += 1) {
					vertices[k].normal.normalize();
					vertices[k].tangent.normalize();
					vertices[k].bitangent.normalize();
				}
			}

			meshes.push_back(mesh);

			// Fill out Material.
			if (tmaterials.size() > 0) {

				// Note that every shape has only one material because the material split is complete.
				// We have already accounted for the per face materials and split meshes per material.

				int material_id = tshape.mesh.material_ids[0];

				// Scoped for single memcpy.
				{
					struct Copy {
						float ambient[3];
						float diffuse[3];
						float specular[3];
						float transmittance[3];
						float emission[3];
						float shininess;
						float ior; // index of refraction
						float dissolve; // 1 == opaque; 0 == fully transparent
						int illum; // illumination model (see http://www.fileformat.info/format/material/)
					};
					std::memcpy(material.ambient, tmaterials[material_id].ambient, sizeof(Copy));
				}

				if (!tmaterials[material_id].ambient_texname.empty()) {
					material.ambient_texname = filebasepath + tmaterials[material_id].ambient_texname;
				}
				if (!tmaterials[material_id].diffuse_texname.empty()) {
					material.diffuse_texname = filebasepath + tmaterials[material_id].diffuse_texname;
				}
				if (!tmaterials[material_id].specular_texname.empty()) {
					material.specular_texname = filebasepath + tmaterials[material_id].specular_texname;
				}
				if (!tmaterials[material_id].specular_highlight_texname.empty()) {
					material.specular_highlight_texname = filebasepath + tmaterials[material_id].specular_highlight_texname;
				}
				if (!tmaterials[material_id].bump_texname.empty()) {
					material.bump_texname = filebasepath + tmaterials[material_id].bump_texname;
				}
				if (!tmaterials[material_id].displacement_texname.empty()) {
					material.displacement_texname = filebasepath + tmaterials[material_id].displacement_texname;
				}
				if (!tmaterials[material_id].alpha_texname.empty()) {
					material.alpha_texname = filebasepath + tmaterials[material_id].alpha_texname;
				}
			}

			materials.push_back(material);

			/*std::cout << "i" << std::endl;
			for (int i = 0; i < tshape.mesh.indices.size(); ++i) {
				std::cout << tshape.mesh.indices[i].vertex_index << std::endl;
			}*/

			/*std::cout << "n" << std::endl;
			for (int i = 0; i < tshape.mesh.num_face_vertices.size(); ++i) {
				std::cout << tshape.mesh.num_face_vertices[i] << std::endl;
			}*/

			/*std::cout << "m" << std::endl;
			for (int i = 0; i < tshape.mesh.material_ids.size(); ++i) {
				std::cout << tshape.mesh.material_ids[i] << std::endl;
			}*/

			/*std::cout << "s" << std::endl;
			for (int i = 0; i < tshape.mesh.smoothing_group_ids.size(); ++i) {
				std::cout << tshape.mesh.smoothing_group_ids[i] << std::endl;
			}*/
		}

		return true;
	} else {
		// printf("%s", error.c_str());
		return false;
	}
}
