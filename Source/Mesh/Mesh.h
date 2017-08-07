//
//  Mesh.h
//
//
//  Created by Narendra Umate on 8/6/16.
//
//

#pragma once

#include "Vector.h"
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
struct P2N2T2P2 {
	Vec2 position;
	Vec2 normal;
	Vec2 texcoord;
	Vec2 pad;
	P2N2T2P2(const Vec2& p, const Vec2& n, const Vec2& t)
	: position(p)
	, normal(n)
	, texcoord(t)
	, pad(Vec2::zero) {
	}
	P2N2T2P2()
	: position(Vec2::zero)
	, normal(Vec2::zero)
	, texcoord(Vec2::zero)
	, pad(Vec2::zero) {
	}
};

// TODO Move this to a better location.
struct P4N4T4B4T2P2 {
	Vec4 position;
	Vec4 normal;
	Vec4 tangent;
	Vec4 bitangent;
	Vec2 texcoord;
	Vec2 pad; // TODO Evaluate need for Metal. Works with and without for OpenGl.
	P4N4T4B4T2P2(const Vec3& position, const Vec3& normal, const Vec2& texcoord)
	: position(position.x, position.y, position.z, 1.0f)
	, normal(normal.x, normal.y, normal.z, 0.0f)
	, tangent(0.0f, 0.0f, 0.0f, 0.0f)
	, bitangent(0.0f, 0.0f, 0.0f, 0.0f)
	, texcoord(texcoord)
	, pad(Vec2::zero) {
	}
	P4N4T4B4T2P2()
	: position(position.x, position.y, position.z, 1.0f)
	, normal(0.0f, 0.0f, 0.0f, 0.0f)
	, tangent(0.0f, 0.0f, 0.0f, 0.0f)
	, bitangent(0.0f, 0.0f, 0.0f, 0.0f)
	, texcoord(0.0f, 0.0f)
	, pad(0.0f, 0.0f) {
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

// TODO Move this to a better location.
struct Effect {
	std::string vertexShader;
	std::string fragmentShader;
};

struct Mesh {
	void getVertexBuffer(unsigned long& size, float*& buffer, unsigned int material = 0) {
		size = vertices.size() * sizeof(P4N4T4B4T2P2);
		buffer = &vertices[0].position[0];
	}

	void getPointIndexBuffer(unsigned long& size, unsigned int*& buffer, unsigned int material = 0) {
		size = primitives[material].points.indices.size();
		buffer = &primitives[material].points.indices[0];
	}

	void getEdgeIndexBuffer(unsigned long& size, unsigned int*& buffer, unsigned int material = 0) {
		size = primitives[material].edges.indices.size();
		buffer = &primitives[material].edges.indices[0];
	}

	void getTriangleIndexBuffer(unsigned long& size, unsigned int*& buffer, unsigned int material = 0) {
		size = primitives[material].triangles.indices.size();
		buffer = &primitives[material].triangles.indices[0];
	}

	void getQuadrilateralIndexBuffer(unsigned long& size, unsigned int*& buffer, unsigned int material = 0) {
		size = primitives[material].quadrilaterals.indices.size();
		buffer = &primitives[material].quadrilaterals.indices[0];
	}

	void getFaceIndexBuffer(unsigned long& size, unsigned int*& counts, unsigned int*& indices, unsigned int material = 0) {
		size = primitives[material].faces.counts.size();
		counts = &primitives[material].faces.counts[0];
		indices = &primitives[material].faces.indices[0];
	}

	std::vector<P4N4T4B4T2P2> vertices;

	std::vector<Material> materials;

	struct Points {
		std::vector<unsigned int> indices;
	};

	struct Edges {
		std::vector<unsigned int> indices;
	};

	struct Triangles {
		std::vector<unsigned int> indices;
	};

	struct Quadrilaterals {
		std::vector<unsigned int> indices;
	};

	struct Faces {
		std::vector<unsigned int> indices;
		std::vector<unsigned int> counts;
	};

	struct Primitives {
		Points points;
		Edges edges;
		Triangles triangles;
		Quadrilaterals quadrilaterals;
		Faces faces;
	};

	std::unordered_map<unsigned int, Primitives> primitives;
};

bool readMesh(const std::string& filepath, std::vector<Mesh>& mesh, std::vector<Material>& material);

void printMesh(Mesh* mesh, Material* Material);
