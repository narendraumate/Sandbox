//
//  BufferVk.h
//
//
//  Created by Narendra Umate on 8/21/16.
//
//

#pragma once

#define BUFFER_OFFSET(offset) (void*)((char*)nullptr + offset)

#include "Buffer.h"

class VertexBufferVk : public VertexBuffer {
public:
	VertexBufferVk(const std::vector<P2T2>& vertices);
	VertexBufferVk(const std::vector<P4N4T4B4T2P2>& vertices);
	virtual ~VertexBufferVk();

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

class IndexBufferVk : public IndexBuffer {
public:
	IndexBufferVk(const Type& type, const std::vector<unsigned int>& indices, const std::vector<unsigned int>& counts = std::vector<unsigned int>());
	virtual ~IndexBufferVk();

	virtual void bind();
	virtual void unbind();
	virtual void initialize();
	virtual void deinitialize();

private:
	unsigned int m_buffer;
	unsigned long m_count;
	unsigned long m_size;
};

class VisualEffectVk : public VisualEffect {
public:
	VisualEffectVk(const Effect& effect);
	virtual ~VisualEffectVk();

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

private:
	unsigned int m_program;
};

class FramebufferVk : public Framebuffer {
public:
	FramebufferVk(const int& width, const int& height, const Framebuffer::Descriptor& descriptor);
	virtual ~FramebufferVk();

	virtual void bind(const Target& target);
	virtual void unbind(const Target& target);
};

class Texture2DVk : public Texture2D {
public:
	Texture2DVk(const std::string& type, const std::string& filepath);
	virtual ~Texture2DVk();

	virtual void bind();
	virtual void unbind();

	static Texture2DVk* allocate(const std::string& type, const std::string& filepath);
	static void deallocate(const std::string& type, const std::string& filepath);
	struct Container {
		unsigned int count = 0;
		Texture2DVk* pointer = nullptr;
	};
	static std::unordered_map<std::string, Container> s_map;
};

class VisualMaterialVk : public VisualMaterial {
public:
	VisualMaterialVk(const Material& material);
	virtual ~VisualMaterialVk();

	void enable();
	void disable();

	void initialize();
	void deinitialize();
};
