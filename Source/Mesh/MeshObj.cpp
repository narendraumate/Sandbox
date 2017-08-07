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
	tinyobj::attrib_t attribs;
	std::vector<tinyobj::shape_t> tshapes;
	std::vector<tinyobj::material_t> tmaterials;
	std::string warning;
	std::string error;
	std::string filebasepath = findBasePath(filepath);
	bool result = LoadObj(&attribs, &tshapes, &tmaterials, &warning, &error, filepath.c_str(), filebasepath.c_str(), true, true);
	if (result && error.empty()) {
		bool normalsPresent = attribs.normals.size() > 0;
		bool texcoordsPresent = attribs.texcoords.size() > 0;
		int primitiveIndex = 0;
		for (tinyobj::shape_t tshape : tshapes) {
			Mesh mesh;
			Material material;

			// Fill out Mesh.
			std::vector<unsigned int>& indices = mesh.primitives[primitiveIndex].triangles.indices;
			indices.resize(tshape.mesh.indices.size());
			std::vector<P4N4T4B4T2P2>& vertices = mesh.vertices;
			vertices.resize(tshape.mesh.indices.size());
			for (unsigned int i = 0; i < tshape.mesh.indices.size(); ++i) {
				indices[i] = (i);
				tinyobj::index_t indicesTuple = tshape.mesh.indices[i];
				vertices[i].position.x = (attribs.vertices[indicesTuple.vertex_index * 3 + 0]);
				vertices[i].position.y = (attribs.vertices[indicesTuple.vertex_index * 3 + 1]);
				vertices[i].position.z = (attribs.vertices[indicesTuple.vertex_index * 3 + 2]);
				if (normalsPresent) {
					vertices[i].normal.x = (attribs.normals[indicesTuple.normal_index * 3 + 0]);
					vertices[i].normal.y = (attribs.normals[indicesTuple.normal_index * 3 + 1]);
					vertices[i].normal.z = (attribs.normals[indicesTuple.normal_index * 3 + 2]);
				}
				if (texcoordsPresent) {
					vertices[i].texcoord.x = attribs.texcoords[indicesTuple.texcoord_index * 2 + 0];
					vertices[i].texcoord.y = attribs.texcoords[indicesTuple.texcoord_index * 2 + 1];
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

				// Assume that every shape has only one material because per face materials is a myth.
				// To account for per face materials the shape will need to be split here accordingly.
				int material_id = tshape.mesh.material_ids[0];

				std::memcpy(material.ambient, tmaterials[material_id].ambient, sizeof(float) * 3);
				std::memcpy(material.diffuse, tmaterials[material_id].diffuse, sizeof(float) * 3);
				std::memcpy(material.specular, tmaterials[material_id].specular, sizeof(float) * 3);
				std::memcpy(material.transmittance, tmaterials[material_id].transmittance, sizeof(float) * 3);
				std::memcpy(material.emission, tmaterials[material_id].emission, sizeof(float) * 3);
				material.shininess = tmaterials[material_id].shininess;
				material.ior = tmaterials[material_id].ior;
				material.dissolve = tmaterials[material_id].dissolve;
				material.illum = tmaterials[material_id].illum;

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
		}
		return true;
	} else {
		printf("%s", error.c_str());
		return false;
	}
}
