//
//  BufferVk.cpp
//
//
//  Created by Narendra Umate on 8/21/16.
//
//

#include "BufferVk.h"

#include "Image.h"

VertexBufferVk::VertexBufferVk(const std::vector<P2T2>& vertices)
: VertexBuffer(vertices)
, m_vertexArray(0)
, m_buffer(0)
, m_count(0)
, m_size(0) {
	initialize();
}

VertexBufferVk::VertexBufferVk(const std::vector<P4N4T4B4T2P2>& vertices)
: VertexBuffer(vertices)
, m_vertexArray(0)
, m_buffer(0)
, m_count(0)
, m_size(0) {
	initialize();
}

VertexBufferVk::~VertexBufferVk() {
	deinitialize();
}

void VertexBufferVk::bind() {
}

void VertexBufferVk::unbind() {
}

void VertexBufferVk::initialize() {
	m_count = m_vertexCount * sizeof(P4N4T4B4T2P2) / sizeof(float);
	m_size = m_count * sizeof(float);
}

void VertexBufferVk::deinitialize() {
	m_count = 0;
	m_size = 0;
}

IndexBufferVk::IndexBufferVk(const Type& type, const std::vector<unsigned int>& indices, const std::vector<unsigned int>& counts)
: IndexBuffer(type, indices, counts)
, m_buffer(0)
, m_count(0)
, m_size(0) {
	initialize();
}

IndexBufferVk::~IndexBufferVk() {
	deinitialize();
}

void IndexBufferVk::bind() {
}

void IndexBufferVk::unbind() {
}

void IndexBufferVk::initialize() {
	m_count = m_indices.size();
	m_size = m_count * sizeof(unsigned int);
}

void IndexBufferVk::deinitialize() {
	m_count = 0;
	m_size = 0;
}

VisualEffectVk::VisualEffectVk(const Effect& effect)
: VisualEffect(effect) {
}

VisualEffectVk::~VisualEffectVk() {
}

void VisualEffectVk::set1iv(const char* name, unsigned int count, const int* v0) {
}

void VisualEffectVk::set1fv(const char* name, unsigned int count, const float* v0) {
}

void VisualEffectVk::set2iv(const char* name, unsigned int count, const int* v0) {
}

void VisualEffectVk::set2fv(const char* name, unsigned int count, const float* v0) {
}

void VisualEffectVk::set3iv(const char* name, unsigned int count, const int* v0) {
}

void VisualEffectVk::set3fv(const char* name, unsigned int count, const float* v0) {
}

void VisualEffectVk::set4iv(const char* name, unsigned int count, const int* v0) {
}

void VisualEffectVk::set4fv(const char* name, unsigned int count, const float* v0) {
}

void VisualEffectVk::setMatrix3fv(const char* name, unsigned int count, bool transpose, const float* v0) {
}

void VisualEffectVk::setMatrix4fv(const char* name, unsigned int count, bool transpose, const float* v0) {
}

void VisualEffectVk::set1iv(int location, unsigned int count, const int* v0) {
}

void VisualEffectVk::set1fv(int location, unsigned int count, const float* v0) {
}

void VisualEffectVk::set2iv(int location, unsigned int count, const int* v0) {
}

void VisualEffectVk::set2fv(int location, unsigned int count, const float* v0) {
}

void VisualEffectVk::set3iv(int location, unsigned int count, const int* v0) {
}

void VisualEffectVk::set3fv(int location, unsigned int count, const float* v0) {
}

void VisualEffectVk::set4iv(int location, unsigned int count, const int* v0) {
}

void VisualEffectVk::set4fv(int location, unsigned int count, const float* v0) {
}

void VisualEffectVk::setMatrix3fv(int location, unsigned int count, bool transpose, const float* v0) {
}

void VisualEffectVk::setMatrix4fv(int location, unsigned int count, bool transpose, const float* v0) {
}

FramebufferVk::FramebufferVk(const int& width, const int& height, const Framebuffer::Descriptor& descriptor)
: Framebuffer(width, height, descriptor) {
}

FramebufferVk::~FramebufferVk() {
}

void FramebufferVk::bind(const Target& target) {
	if (target == Target::Draw) {
	} else if (target == Target::Read) {
	} else {
	}
}

void FramebufferVk::unbind(const Target& target) {
	if (target == Target::Draw) {
	} else if (target == Target::Read) {
	} else {
	}
}

std::unordered_map<std::string, Texture2DVk::Container> Texture2DVk::s_map;

Texture2DVk::Texture2DVk(const std::string& type, const std::string& filepath)
: Texture2D() {
	int sizeX = 0;
	int sizeY = 0;
	unsigned char* pixels = nullptr;

#if DEBUG_TEXTURE
	sizeX = 512;
	sizeY = 512;
	int channels = 4;
	int dimensions = sizeX * sizeY;
	pixels = new unsigned char[channels * dimensions];

	if ("ambient" == type) {
		for (int i = 0; i < dimensions; ++i) {
			pixels[i * 4] = 0xFF;
			pixels[i * 4 + 1] = 0x00;
			pixels[i * 4 + 2] = 0x00;
			pixels[i * 4 + 3] = 0xFF;
		}
	} else if ("diffuse" == type) {
		for (int i = 0; i < dimensions; ++i) {
			pixels[i * 4] = 0x00;
			pixels[i * 4 + 1] = 0xFF;
			pixels[i * 4 + 2] = 0x00;
			pixels[i * 4 + 3] = 0xFF;
		}
	} else if ("alpha" == type) {
		for (int i = 0; i < dimensions; ++i) {
			pixels[i * 4] = 0x00;
			pixels[i * 4 + 1] = 0x00;
			pixels[i * 4 + 2] = 0xFF;
			pixels[i * 4 + 3] = 0xFF;
		}
	} else {
		for (int i = 0; i < dimensions; ++i) {
			pixels[i * 4] = 0xFF;
			pixels[i * 4 + 1] = 0xFF;
			pixels[i * 4 + 2] = 0xFF;
			pixels[i * 4 + 3] = 0xFF;
		}
	}
#else
	load(filepath, sizeX, sizeY, pixels);
#endif

#if DEBUG_TEXTURE
	delete[] pixels;
#else
	unload(pixels);
#endif
}

Texture2DVk::~Texture2DVk() {
}

void Texture2DVk::bind() {
}

void Texture2DVk::unbind() {
}

Texture2DVk* Texture2DVk::allocate(const std::string& type, const std::string& filepath) {
	std::string key = type + ":" + filepath;
	if (s_map[key].count == 0) {
		s_map[key].count = 1;
		s_map[key].pointer = new Texture2DVk(type, filepath);
		return s_map[key].pointer;
	} else {
		s_map[key].count += 1;
		return s_map[key].pointer;
	}
}

void Texture2DVk::deallocate(const std::string& type, const std::string& filepath) {
	std::string key = type + ":" + filepath;
	if (s_map[key].count == 1) {
		s_map[key].count = 0;
		delete s_map[key].pointer;
		s_map[key].pointer = nullptr;
	} else {
		s_map[key].count -= 1;
	}
}

VisualMaterialVk::VisualMaterialVk(const Material& material)
: VisualMaterial(material) {
	if (!material.ambient_texname.empty()) {
		ambient_texture = Texture2DVk::allocate(std::string("ambient"), material.ambient_texname);
	}
	if (!material.diffuse_texname.empty()) {
		diffuse_texture = Texture2DVk::allocate(std::string("diffuse"), material.diffuse_texname);
	}
	if (!material.specular_texname.empty()) {
		specular_texture = Texture2DVk::allocate(std::string("specular"), material.specular_texname);
	}
	if (!material.specular_highlight_texname.empty()) {
		specular_highlight_texture = Texture2DVk::allocate(std::string("specular_highlight"), material.specular_highlight_texname);
	}
	if (!material.bump_texname.empty()) {
		bump_texture = Texture2DVk::allocate(std::string("bump"), material.bump_texname);
	}
	if (!material.displacement_texname.empty()) {
		displacement_texture = Texture2DVk::allocate(std::string("displacement"), material.displacement_texname);
	}
	if (!material.alpha_texname.empty()) {
		alpha_texture = Texture2DVk::allocate(std::string("alpha"), material.alpha_texname);
	}
}

VisualMaterialVk::~VisualMaterialVk() {
	if (ambient_texture) {
		Texture2DVk::deallocate(std::string("ambient"), ambient_texname);
	}
	if (diffuse_texture) {
		Texture2DVk::deallocate(std::string("diffuse"), diffuse_texname);
	}
	if (specular_texture) {
		Texture2DVk::deallocate(std::string("specular"), specular_texname);
	}
	if (specular_highlight_texture) {
		Texture2DVk::deallocate(std::string("specular_highlight"), specular_highlight_texname);
	}
	if (bump_texture) {
		Texture2DVk::deallocate(std::string("bump"), bump_texname);
	}
	if (displacement_texture) {
		Texture2DVk::deallocate(std::string("displacement"), displacement_texname);
	}
	if (alpha_texture) {
		Texture2DVk::deallocate(std::string("alpha"), alpha_texname);
	}
}

void VisualMaterialVk::enable() {
}

void VisualMaterialVk::disable() {
}

void VisualMaterialVk::initialize() {
}

void VisualMaterialVk::deinitialize() {
}
