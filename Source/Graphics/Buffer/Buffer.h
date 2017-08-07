//
//  Buffer.h
//
//
//  Created by Narendra Umate on 8/21/16.
//
//

#pragma once

#include <vector>

#include "Mesh.h"

class VertexBuffer {
public:
	VertexBuffer(const std::vector<P2T2>& vertices);
	VertexBuffer(const std::vector<P4N4T4B4T2P2>& vertices);
	virtual ~VertexBuffer() = 0;

	virtual void bind();
	virtual void unbind();
	virtual void initialize();
	virtual void deinitialize();

	float* getVertices();
	unsigned long getVertexCount();

protected:
	enum Format {
		FormatP2T2,
		FormatP4N4T4B4T2P2
	};
	Format m_vertexFormat;
	unsigned int m_vertexCount;
	float* m_vertices;
};

class IndexBuffer {
public:
	enum Type {
		Point,
		Line,
		Triangle,
	};

	IndexBuffer(const Type& type, const std::vector<unsigned int>& indices, const std::vector<unsigned int>& counts);
	virtual ~IndexBuffer() = 0;

	virtual void bind();
	virtual void unbind();
	virtual void initialize();
	virtual void deinitialize();

	Type getType();
	unsigned int* getIndices();
	unsigned long getIndexCount();

protected:
	Type m_type;
	std::vector<unsigned int> m_indices;
	std::vector<unsigned int> m_counts;
};

// TODO Move this to a better location.
struct Effect {
	std::string vertexShader;
	std::string fragmentShader;
};

class VisualEffect {
public:
	VisualEffect(const Effect& effect);
	virtual ~VisualEffect() = 0;

	virtual void set1iv(const char* name, unsigned int count, const int* v0);
	virtual void set1fv(const char* name, unsigned int count, const float* v0);
	virtual void set2iv(const char* name, unsigned int count, const int* v0);
	virtual void set2fv(const char* name, unsigned int count, const float* v0);
	virtual void set3iv(const char* name, unsigned int count, const int* v0);
	virtual void set3fv(const char* name, unsigned int count, const float* v0);
	virtual void set4iv(const char* name, unsigned int count, const int* v0);
	virtual void set4fv(const char* name, unsigned int count, const float* v0);
	virtual void setMatrix3fv(const char* name, unsigned int count, bool transpose, const float* v0);
	virtual void setMatrix4fv(const char* name, unsigned int count, bool transpose, const float* v0);

	virtual void set1iv(int location, unsigned int count, const int* v0);
	virtual void set1fv(int location, unsigned int count, const float* v0);
	virtual void set2iv(int location, unsigned int count, const int* v0);
	virtual void set2fv(int location, unsigned int count, const float* v0);
	virtual void set3iv(int location, unsigned int count, const int* v0);
	virtual void set3fv(int location, unsigned int count, const float* v0);
	virtual void set4iv(int location, unsigned int count, const int* v0);
	virtual void set4fv(int location, unsigned int count, const float* v0);
	virtual void setMatrix3fv(int location, unsigned int count, bool transpose, const float* v0);
	virtual void setMatrix4fv(int location, unsigned int count, bool transpose, const float* v0);

	virtual void setSampler(const char* name, int location, const unsigned int& texture);
};

class Framebuffer {
public:
	enum Target {
		Draw,
		Read,
		Both
	};

	enum Format {
		Color,
		Depth,
		Stencil
	};

	struct Descriptor {
		std::vector<Format> color;
		std::vector<Format> depth;
		std::vector<Format> stencil;
	};

	Framebuffer(const int& width, const int& height, const Descriptor& descriptor);
	virtual ~Framebuffer() = 0;

	virtual void bind(const Target& target);
	virtual void unbind(const Target& target);

	int getWidth();
	int getHeight();

public:
	void* m_colorTexture;
	void* m_depthTexture;
	void* m_stencilTexture;
	Descriptor m_descriptor;

protected:
	void* m_framebuffer;

private:
	int m_width;
	int m_height;
};

class Texture2D {
public:
	Texture2D();
	virtual ~Texture2D() = 0;

	virtual void bind();
	virtual void unbind();

protected:
	unsigned int m_texture; // This should probably move to derived classes.
};

class VisualMaterial {
public:
	VisualMaterial(const Material& material);
	virtual ~VisualMaterial() = 0;

	void enable();
	void disable();

	float ambient[3];
	float diffuse[3];
	float specular[3];
	float transmittance[3];
	float emission[3];
	float shininess;
	float ior; // index of refraction
	float dissolve; // 1 == opaque; 0 == fully transparent
	int illum; // illumination model (see http://www.fileformat.info/format/material/)

protected:
	std::string ambient_texname; // map_Ka
	std::string diffuse_texname; // map_Kd
	std::string specular_texname; // map_Ks
	std::string specular_highlight_texname; // map_Ns
	std::string bump_texname; // map_bump, bump
	std::string displacement_texname; // disp
	std::string alpha_texname; // map_d

	Texture2D* ambient_texture; // map_Ka
	Texture2D* diffuse_texture; // map_Kd
	Texture2D* specular_texture; // map_Ks
	Texture2D* specular_highlight_texture; // map_Ns
	Texture2D* bump_texture; // map_bump, bump
	Texture2D* displacement_texture; // disp
	Texture2D* alpha_texture; // map_d
};
