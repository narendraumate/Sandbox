//
//  Mesh.h
//
//
//  Created by Narendra Umate on 8/6/16.
//
//

#pragma once

#include "Vector.h"
#include <cstdint>
#include <unordered_map>
#include <vector>

// TODO Move this to a better location.
struct P2T2 {
	Vec2 position;
	Vec2 texcoord;
	P2T2(const Vec2& p, const Vec2& t)
	: position(p)
	, texcoord(t) {
	}
	P2T2()
	: position(Vec2::zero)
	, texcoord(Vec2::zero) {
	}
};

// TODO Move this to a better location.
struct P4N4T4B4T2P2 {
	Vec4 position;
	Vec4 normal;
	Vec4 tangent;
	Vec4 bitangent;
	Vec2 texcoord;
	Vec2 pad; // Padding is needed for Metal. Works with and without for OpenGl.
	P4N4T4B4T2P2(const Vec3& p, const Vec3& n, const Vec2& t)
	: position(p.x, p.y, p.z, 1.0f)
	, normal(n.x, n.y, n.z, 0.0f)
	, tangent(Vec4::zero)
	, bitangent(Vec4::zero)
	, texcoord(t)
	, pad(Vec2::zero) {
	}
	P4N4T4B4T2P2()
	: position(0.0f, 0.0f, 0.0f, 1.0f) // Metal requires w as 1.
	, normal(Vec4::zero)
	, tangent(Vec4::zero)
	, bitangent(Vec4::zero)
	, texcoord(Vec2::zero)
	, pad(Vec2::zero) {
	}
};

// TODO Move this to a better location.
struct Material {
	float ambient[3];
	float diffuse[3];
	float specular[3];
	float transmittance[3];
	float emission[3];
	float shininess;
	float ior; // index of refraction
	float dissolve; // 1 == opaque; 0 == fully transparent
	int illum; // illumination model (see http://www.fileformat.info/format/material/)

	std::string ambient_texname; // map_Ka
	std::string diffuse_texname; // map_Kd
	std::string specular_texname; // map_Ks
	std::string specular_highlight_texname; // map_Ns
	std::string bump_texname; // map_bump, bump
	std::string displacement_texname; // disp
	std::string alpha_texname; // map_d
};

struct Mesh {
	void getVertexBuffer(unsigned long& size, float*& buffer, uint32_t material = 0) {
		size = vertices.size() * sizeof(P4N4T4B4T2P2);
		buffer = &vertices[0].position[0];
	}

	void getPointIndexBuffer(unsigned long& size, uint32_t*& buffer, uint32_t material = 0) {
		size = points[material].indices.size();
		buffer = &points[material].indices[0];
	}

	void getLineIndexBuffer(unsigned long& size, uint32_t*& buffer, uint32_t material = 0) {
		size = lines[material].indices.size();
		buffer = &lines[material].indices[0];
	}

	void getTriangleIndexBuffer(unsigned long& size, uint32_t*& buffer, uint32_t material = 0) {
		size = triangles[material].indices.size();
		buffer = &triangles[material].indices[0];
	}

	std::vector<P4N4T4B4T2P2> vertices;

	std::vector<Material> materials;

	struct Points {
		std::vector<uint32_t> indices;
	};

	struct Lines {
		std::vector<uint32_t> indices;
	};

	struct Triangles {
		std::vector<uint32_t> indices;
	};

	std::unordered_map<uint32_t, Points> points;
	std::unordered_map<uint32_t, Lines> lines;
	std::unordered_map<uint32_t, Triangles> triangles;
};

bool readMesh(const std::string& filepath, std::vector<Mesh>& mesh, std::vector<Material>& material);

void printMesh(Mesh* mesh, Material* Material);
