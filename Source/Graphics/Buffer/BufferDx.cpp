//
//  BufferDx.cpp
//
//
//  Created by Narendra Umate on 8/21/16.
//
//

#include "BufferDx.h"

#include "Platform.h"

VertexBufferDx::VertexBufferDx(const std::vector<P2T2>& vertices)
: VertexBuffer(vertices)
, m_vertexArray(0)
, m_buffer(0)
, m_count(0)
, m_size(0) {
	initialize();
}

VertexBufferDx::VertexBufferDx(const std::vector<P4N4T4B4T2P2>& vertices)
: VertexBuffer(vertices)
, m_vertexArray(0)
, m_buffer(0)
, m_count(0)
, m_size(0) {
	initialize();
}

VertexBufferDx::~VertexBufferDx() {
	deinitialize();
}

void VertexBufferDx::bind() {
}

void VertexBufferDx::unbind() {
}

void VertexBufferDx::initialize() {
	m_count = m_vertexCount * sizeof(P4N4T4B4T2P2) / sizeof(float);
	m_size = m_count * sizeof(float);
}

void VertexBufferDx::deinitialize() {
	m_count = 0;
	m_size = 0;
}

IndexBufferDx::IndexBufferDx(const Type& type, const std::vector<unsigned int>& indices, const std::vector<unsigned int>& counts)
: IndexBuffer(type, indices, counts)
, m_buffer(0)
, m_count(0)
, m_size(0) {
	initialize();
}

IndexBufferDx::~IndexBufferDx() {
	deinitialize();
}

void IndexBufferDx::bind() {
}

void IndexBufferDx::unbind() {
}

void IndexBufferDx::initialize() {
	m_count = m_indices.size();
	m_size = m_count * sizeof(unsigned int);
}

void IndexBufferDx::deinitialize() {
	m_count = 0;
	m_size = 0;
}

VisualEffectDx::VisualEffectDx(const Effect& effect)
: VisualEffect(effect) {
	std::string vertexShaderPath = getShaderDirectory() + "/" + effect.vertexShader + "Dx.vert";
	std::string fragmentShaderPath = getShaderDirectory() + "/" + effect.fragmentShader + "Dx.frag";
	if (fileExists(vertexShaderPath) && fileExists(fragmentShaderPath)) {
		m_program = 0;
	} else {
		if (!fileExists(vertexShaderPath)) {
			std::cout << "File Missing: " + vertexShaderPath + "\n";
		}
		if (!fileExists(fragmentShaderPath)) {
			std::cout << "File Missing: " + fragmentShaderPath + "\n";
		}
	}
}

VisualEffectDx::~VisualEffectDx() {
}

void VisualEffectDx::set1iv(const char* name, unsigned int count, const int* v0) {
}

void VisualEffectDx::set1fv(const char* name, unsigned int count, const float* v0) {
}

void VisualEffectDx::set2iv(const char* name, unsigned int count, const int* v0) {
}

void VisualEffectDx::set2fv(const char* name, unsigned int count, const float* v0) {
}

void VisualEffectDx::set3iv(const char* name, unsigned int count, const int* value) {
}

void VisualEffectDx::set3fv(const char* name, unsigned int count, const float* value) {
}

void VisualEffectDx::set4iv(const char* name, unsigned int count, const int* value) {
}

void VisualEffectDx::set4fv(const char* name, unsigned int count, const float* value) {
}

void VisualEffectDx::setMatrix3fv(const char* name, unsigned int count, bool transpose, const float* value) {
}

void VisualEffectDx::setMatrix4fv(const char* name, unsigned int count, bool transpose, const float* value) {
}

void VisualEffectDx::set1iv(int location, unsigned int count, const int* v0) {
}

void VisualEffectDx::set1fv(int location, unsigned int count, const float* v0) {
}

void VisualEffectDx::set2iv(int location, unsigned int count, const int* v0) {
}

void VisualEffectDx::set2fv(int location, unsigned int count, const float* v0) {
}

void VisualEffectDx::set3iv(int location, unsigned int count, const int* value) {
}

void VisualEffectDx::set3fv(int location, unsigned int count, const float* value) {
}

void VisualEffectDx::set4iv(int location, unsigned int count, const int* value) {
}

void VisualEffectDx::set4fv(int location, unsigned int count, const float* value) {
}

void VisualEffectDx::setMatrix3fv(int location, unsigned int count, bool transpose, const float* value) {
}

void VisualEffectDx::setMatrix4fv(int location, unsigned int count, bool transpose, const float* value) {
}

FramebufferDx::FramebufferDx(const int& width, const int& height, const Framebuffer::Descriptor& descriptor)
: Framebuffer(width, height, descriptor) {
}

FramebufferDx::~FramebufferDx() {
}

void FramebufferDx::bind(const Target& target) {
	if (target == Target::Draw) {
	} else if (target == Target::Read) {
	} else {
	}
}

void FramebufferDx::unbind(const Target& target) {
	if (target == Target::Draw) {
	} else if (target == Target::Read) {
	} else {
	}
}

std::unordered_map<std::string, Texture2DDx::Container> Texture2DDx::s_map;

Texture2DDx::Texture2DDx(const std::string& type, const std::string& filepath)
: Texture2D() {
}

Texture2DDx::~Texture2DDx() {
}

void Texture2DDx::bind() {
}

void Texture2DDx::unbind() {
}

Texture2DDx* Texture2DDx::allocate(const std::string& type, const std::string& filepath) {
	std::string key = type + ":" + filepath;
	if (s_map[key].count == 0) {
		s_map[key].count = 1;
		s_map[key].pointer = new Texture2DDx(type, filepath);
		return s_map[key].pointer;
	} else {
		s_map[key].count += 1;
		return s_map[key].pointer;
	}
}

void Texture2DDx::deallocate(const std::string& type, const std::string& filepath) {
	std::string key = type + ":" + filepath;
	if (s_map[key].count == 1) {
		s_map[key].count = 0;
		delete s_map[key].pointer;
		s_map[key].pointer = nullptr;
	} else {
		s_map[key].count -= 1;
	}
}

VisualMaterialDx::VisualMaterialDx(const Material& material)
: VisualMaterial(material) {
}

VisualMaterialDx::~VisualMaterialDx() {
}

void VisualMaterialDx::enable() {
	if (ambient_texture) {
		ambient_texture->bind();
	}
	if (diffuse_texture) {
		diffuse_texture->bind();
	}
	if (specular_texture) {
		specular_texture->bind();
	}
	if (specular_highlight_texture) {
		specular_highlight_texture->bind();
	}
	if (bump_texture) {
		bump_texture->bind();
	}
	if (displacement_texture) {
		displacement_texture->bind();
	}
	if (alpha_texture) {
		alpha_texture->bind();
	}
}

void VisualMaterialDx::disable() {
	if (ambient_texture) {
		ambient_texture->unbind();
	}
	if (diffuse_texture) {
		diffuse_texture->unbind();
	}
	if (specular_texture) {
		specular_texture->unbind();
	}
	if (specular_highlight_texture) {
		specular_highlight_texture->unbind();
	}
	if (bump_texture) {
		bump_texture->unbind();
	}
	if (displacement_texture) {
		displacement_texture->unbind();
	}
	if (alpha_texture) {
		alpha_texture->unbind();
	}
}

void VisualMaterialDx::initialize() {
}

void VisualMaterialDx::deinitialize() {
}
