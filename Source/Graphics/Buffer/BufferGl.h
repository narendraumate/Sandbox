//
//  BufferGl.h
//
//
//  Created by Narendra Umate on 8/21/16.
//
//

#pragma once

#define BUFFER_OFFSET(offset) (void*)((char*)nullptr + offset)

#include "Buffer.h"

class VertexBufferGl : public VertexBuffer {
public:
	VertexBufferGl(const std::vector<P2T2>& vertices);
	VertexBufferGl(const std::vector<P4N4T4B4T2P2>& vertices);
	virtual ~VertexBufferGl();

	virtual void bind();
	virtual void unbind();
	virtual void initialize();
	virtual void deinitialize();

private:
	unsigned int m_vertexArray;
	unsigned int m_buffer;
	unsigned int m_count;
	unsigned int m_size;
};

class IndexBufferGl : public IndexBuffer {
public:
	IndexBufferGl(const Type& type, const std::vector<unsigned int>& indices, const std::vector<unsigned int>& counts = std::vector<unsigned int>());
	virtual ~IndexBufferGl();

	virtual void bind();
	virtual void unbind();
	virtual void initialize();
	virtual void deinitialize();

private:
	unsigned int m_buffer;
	unsigned long m_count;
	unsigned long m_size;
};

class VisualEffectGl : public VisualEffect {
public:
	VisualEffectGl(const Effect& effect);
	virtual ~VisualEffectGl();

	virtual void set1iv(const char* name, unsigned int count, const int* v0);
	virtual void set1fv(const char* name, unsigned int count, const float* v0);
	virtual void set2iv(const char* name, unsigned int count, const int* v0);
	virtual void set2fv(const char* name, unsigned int count, const float* v0);
	virtual void set3iv(const char* name, unsigned int count, const int* value);
	virtual void set3fv(const char* name, unsigned int count, const float* value);
	virtual void set4iv(const char* name, unsigned int count, const int* v0);
	virtual void set4fv(const char* name, unsigned int count, const float* v0);
	virtual void setMatrix3fv(const char* name, unsigned int count, bool transpose, const float* value);
	virtual void setMatrix4fv(const char* name, unsigned int count, bool transpose, const float* value);

	virtual void set1iv(int location, unsigned int count, const int* v0);
	virtual void set1fv(int location, unsigned int count, const float* v0);
	virtual void set2iv(int location, unsigned int count, const int* v0);
	virtual void set2fv(int location, unsigned int count, const float* v0);
	virtual void set3iv(int location, unsigned int count, const int* value);
	virtual void set3fv(int location, unsigned int count, const float* value);
	virtual void set4iv(int location, unsigned int count, const int* v0);
	virtual void set4fv(int location, unsigned int count, const float* v0);
	virtual void setMatrix3fv(int location, unsigned int count, bool transpose, const float* value);
	virtual void setMatrix4fv(int location, unsigned int count, bool transpose, const float* value);

	virtual void setSampler(const char* name, int location, const unsigned int& texture);

public:
	unsigned int m_program;
};

class FramebufferGl : public Framebuffer {
public:
	FramebufferGl(const int& width, const int& height, const Framebuffer::Descriptor& descriptor);
	virtual ~FramebufferGl();

	virtual void bind(const Target& target);
	virtual void unbind(const Target& target);
};

class Texture2DGl : public Texture2D {
public:
	Texture2DGl(const std::string& type, const std::string& filepath);
	virtual ~Texture2DGl();

	virtual void bind();
	virtual void unbind();

	static Texture2DGl* allocate(const std::string& type, const std::string& filepath);
	static void deallocate(const std::string& type, const std::string& filepath);
	struct Container {
		unsigned int count = 0;
		Texture2DGl* pointer = nullptr;
	};
	static std::unordered_map<std::string, Container> s_map;
};

class VisualMaterialGl : public VisualMaterial {
public:
	VisualMaterialGl(const Material& material);
	virtual ~VisualMaterialGl();

	void enable();
	void disable();

	void initialize();
	void deinitialize();
};
