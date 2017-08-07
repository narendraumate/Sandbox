//
//  BufferMl.h
//
//
//  Created by Narendra Umate on 8/21/16.
//
//

#pragma once

#include "Buffer.h"

#if defined(__APPLE__)
#include "mtlpp.hpp"
#endif // defined(__APPLE__)

class VertexBufferMl : public VertexBuffer {
public:
	VertexBufferMl(const std::vector<P2T2>& vertices);
	VertexBufferMl(const std::vector<P4N4T4B4T2P2>& vertices);
	virtual ~VertexBufferMl();

	virtual void bind();
	virtual void unbind();
	virtual void initialize();
	virtual void deinitialize();

private:
	unsigned int m_vertexArray;
	unsigned int m_buffer;
	unsigned int m_count;
	unsigned int m_size;

public:
#if defined(__APPLE__)
	mtlpp::Buffer m_vertexBuffer;
#endif // defined(__APPLE__)
};

class IndexBufferMl : public IndexBuffer {
public:
	IndexBufferMl(const Type& type, const std::vector<unsigned int>& indices, const std::vector<unsigned int>& counts = std::vector<unsigned int>());
	virtual ~IndexBufferMl();

	virtual void bind();
	virtual void unbind();
	virtual void initialize();
	virtual void deinitialize();

private:
	unsigned int m_buffer;
	unsigned long m_count;
	unsigned long m_size;

public:
#if defined(__APPLE__)
	mtlpp::Buffer m_indexBuffer;
#endif // defined(__APPLE__)
};

class VisualEffectMl : public VisualEffect {
public:
	VisualEffectMl(const Effect& effect);
	virtual ~VisualEffectMl();

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

public:
#if defined(__APPLE__)
	mtlpp::Function m_vertexFunction;
	mtlpp::Function m_fragmentFunction;
#endif // defined(__APPLE__)
};

class FramebufferMl : public Framebuffer {
public:
	FramebufferMl(const int& width, const int& height, const Framebuffer::Descriptor& descriptor);
	virtual ~FramebufferMl();

	virtual void bind(const Target& target);
	virtual void unbind(const Target& target);
	// TODO Consider making this private
public:
#if defined(__APPLE__)
	mtlpp::RenderPassDescriptor m_renderPassDescriptor;
#endif // defined(__APPLE__)
};

class Texture2DMl : public Texture2D {
public:
	Texture2DMl(const std::string& type, const std::string& filepath);
	virtual ~Texture2DMl();

	void bind();
	void unbind();

	static Texture2DMl* allocate(const std::string& type, const std::string& filepath);
	static void deallocate(const std::string& type, const std::string& filepath);
	struct Container {
		unsigned int count = 0;
		Texture2DMl* pointer = nullptr;
	};
	static std::unordered_map<std::string, Container> s_map;
};

class VisualMaterialMl : public VisualMaterial {
public:
	VisualMaterialMl(const Material& material);
	virtual ~VisualMaterialMl();

	void enable();
	void disable();

	void initialize();
	void deinitialize();
};
