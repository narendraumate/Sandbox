//
//  BufferMl.cpp
//
//
//  Created by Narendra Umate on 8/21/16.
//
//

#include "BufferMl.h"

#include "Platform.h"

VertexBufferMl::VertexBufferMl(const std::vector<P2T2>& vertices)
: VertexBuffer(vertices)
, m_vertexArray(0)
, m_buffer(0)
, m_count(0)
, m_size(0) {
	initialize();
}

VertexBufferMl::VertexBufferMl(const std::vector<P4N4T4B4T2P2>& vertices)
: VertexBuffer(vertices)
, m_vertexArray(0)
, m_buffer(0)
, m_count(0)
, m_size(0) {
	initialize();
}

VertexBufferMl::~VertexBufferMl() {
	deinitialize();
}

void VertexBufferMl::bind() {
}

void VertexBufferMl::unbind() {
}

void VertexBufferMl::initialize() {
	if (m_vertexFormat == Format::FormatP2T2) {
		m_count = m_vertexCount * sizeof(P2T2) / sizeof(float);
	} else if (m_vertexFormat == Format::FormatP4N4T4B4T2P2) {
		m_count = m_vertexCount * sizeof(P4N4T4B4T2P2) / sizeof(float);
	}
	m_size = m_count * sizeof(float);
#if defined(__APPLE__)
	mtlpp::Device device = mtlpp::Device::CreateSystemDefaultDevice();
	typedef struct
	{
		float position[4];
		float normal[4];
		float tangent[4];
		float bitangent[4];
		float texcoord[2];
		float pad[2];
	} Vertex;
	Vertex* vertexArray = (Vertex*)m_vertices;
	unsigned int vertexCount = m_vertexCount;
	float* vertices = (float*)&vertexArray[0];
	unsigned int vertexSize = sizeof(P4N4T4B4T2P2);
	unsigned int finalSize = vertexCount * vertexSize;
	m_vertexBuffer = device.NewBuffer(vertices, finalSize, mtlpp::ResourceOptions::StorageModeShared);
#endif // defined(__APPLE__)
}

void VertexBufferMl::deinitialize() {
	m_count = 0;
	m_size = 0;
}

IndexBufferMl::IndexBufferMl(const Type& type, const std::vector<unsigned int>& indices, const std::vector<unsigned int>& counts)
: IndexBuffer(type, indices, counts)
, m_buffer(0)
, m_count(0)
, m_size(0) {
	initialize();
}

IndexBufferMl::~IndexBufferMl() {
	deinitialize();
}

void IndexBufferMl::bind() {
}

void IndexBufferMl::unbind() {
}

void IndexBufferMl::initialize() {
	m_count = m_indices.size();
	m_size = m_count * sizeof(unsigned int);
#if defined(__APPLE__)
	mtlpp::Device device = mtlpp::Device::CreateSystemDefaultDevice();
	unsigned int* indices = &m_indices[0];
	unsigned int finalSize = m_indices.size() * sizeof(unsigned int);
	m_indexBuffer = device.NewBuffer(indices, finalSize, mtlpp::ResourceOptions::StorageModeShared);
#endif // defined(__APPLE__)
}

void IndexBufferMl::deinitialize() {
	m_count = 0;
	m_size = 0;
}

VisualEffectMl::VisualEffectMl(const Effect& effect)
: VisualEffect(effect) {
#if defined(__APPLE__)
	std::string vertexShaderPath = getShaderDirectory() + "/" + effect.vertexShader + "Ml.vert";
	std::string fragmentShaderPath = getShaderDirectory() + "/" + effect.fragmentShader + "Ml.frag";
	if (fileExists(vertexShaderPath) && fileExists(fragmentShaderPath)) {
		mtlpp::Device device = mtlpp::Device::CreateSystemDefaultDevice();

		// The function newLibraryWithSource slow leaks memory
		// Until the bug is resolved be cautius with this code
		// https://bugreport.apple.com/web/?problemID=34077222

		std::string vertexSource;
		fileReadIntoString(vertexShaderPath, vertexSource);
		mtlpp::CompileOptions vertexCompileOptions;
		mtlpp::Library vertexLibrary = device.NewLibrary(vertexSource.c_str(), vertexCompileOptions, nullptr);
		m_vertexFunction = vertexLibrary.NewFunction("vertex_main");

		std::string fragmentSource;
		fileReadIntoString(fragmentShaderPath, fragmentSource);
		mtlpp::CompileOptions fragmentCompileOptions;
		mtlpp::Library fragmentLibrary = device.NewLibrary(fragmentSource.c_str(), fragmentCompileOptions, nullptr);
		m_fragmentFunction = fragmentLibrary.NewFunction("fragment_main");
	} else {
		if (!fileExists(vertexShaderPath)) {
			std::cout << "File Missing: " + vertexShaderPath + "\n";
		}
		if (!fileExists(fragmentShaderPath)) {
			std::cout << "File Missing: " + fragmentShaderPath + "\n";
		}
	}
#endif // defined(__APPLE__)
}

VisualEffectMl::~VisualEffectMl() {
}

void VisualEffectMl::set1iv(const char* name, unsigned int count, const int* v0) {
}

void VisualEffectMl::set1fv(const char* name, unsigned int count, const float* v0) {
}

void VisualEffectMl::set2iv(const char* name, unsigned int count, const int* v0) {
}

void VisualEffectMl::set2fv(const char* name, unsigned int count, const float* v0) {
}

void VisualEffectMl::set3iv(const char* name, unsigned int count, const int* value) {
}

void VisualEffectMl::set3fv(const char* name, unsigned int count, const float* value) {
}

void VisualEffectMl::set4iv(const char* name, unsigned int count, const int* v0) {
}

void VisualEffectMl::set4fv(const char* name, unsigned int count, const float* v0) {
}

void VisualEffectMl::setMatrix3fv(const char* name, unsigned int count, bool transpose, const float* value) {
}

void VisualEffectMl::setMatrix4fv(const char* name, unsigned int count, bool transpose, const float* value) {
}

void VisualEffectMl::set1iv(int location, unsigned int count, const int* v0) {
}

void VisualEffectMl::set1fv(int location, unsigned int count, const float* v0) {
}

void VisualEffectMl::set2iv(int location, unsigned int count, const int* v0) {
}

void VisualEffectMl::set2fv(int location, unsigned int count, const float* v0) {
}

void VisualEffectMl::set3iv(int location, unsigned int count, const int* value) {
}

void VisualEffectMl::set3fv(int location, unsigned int count, const float* value) {
}

void VisualEffectMl::set4iv(int location, unsigned int count, const int* value) {
}

void VisualEffectMl::set4fv(int location, unsigned int count, const float* value) {
}

void VisualEffectMl::setMatrix3fv(int location, unsigned int count, bool transpose, const float* value) {
}

void VisualEffectMl::setMatrix4fv(int location, unsigned int count, bool transpose, const float* value) {
}

FramebufferMl::FramebufferMl(const int& width, const int& height, const Framebuffer::Descriptor& descriptor)
: Framebuffer(width, height, descriptor) {
#if defined(__APPLE__)
	// hack disable stencil for now
	m_descriptor.stencil.clear();

	mtlpp::Device device = mtlpp::Device::CreateSystemDefaultDevice();

	mtlpp::TextureDescriptor colorTextureDescriptor = mtlpp::TextureDescriptor::Texture2DDescriptor(mtlpp::PixelFormat::BGRA8Unorm_sRGB, width, height, false);
	unsigned int colorTextureUsage = 0;
	colorTextureUsage |= static_cast<unsigned int>(mtlpp::TextureUsage::RenderTarget);
	colorTextureUsage |= static_cast<unsigned int>(mtlpp::TextureUsage::ShaderRead);
	colorTextureDescriptor.SetUsage((mtlpp::TextureUsage)colorTextureUsage);
	for (int colorAttachmentIndex = 0; colorAttachmentIndex < m_descriptor.color.size(); ++colorAttachmentIndex) {
		mtlpp::Texture colorTexture = device.NewTexture(colorTextureDescriptor);
		m_renderPassDescriptor.GetColorAttachments()[colorAttachmentIndex].SetTexture(colorTexture);
		m_renderPassDescriptor.GetColorAttachments()[colorAttachmentIndex].SetLoadAction(mtlpp::LoadAction::Clear);
		m_renderPassDescriptor.GetColorAttachments()[colorAttachmentIndex].SetStoreAction(mtlpp::StoreAction::Store);
		m_renderPassDescriptor.GetColorAttachments()[colorAttachmentIndex].SetClearColor(mtlpp::ClearColor(0.0, 0.0, 0.0, 1.0));
	}

	mtlpp::TextureDescriptor depthTextureDescriptor = mtlpp::TextureDescriptor::Texture2DDescriptor(mtlpp::PixelFormat::Depth32Float_Stencil8, width, height, false);
	unsigned int depthResourceOptions = 0;
	depthResourceOptions |= static_cast<unsigned int>(mtlpp::ResourceOptions::StorageModePrivate);
	depthTextureDescriptor.SetResourceOptions((mtlpp::ResourceOptions)depthResourceOptions);
	unsigned int depthTextureUsage = 0;
	depthTextureUsage |= static_cast<unsigned int>(mtlpp::TextureUsage::RenderTarget);
	depthTextureUsage |= static_cast<unsigned int>(mtlpp::TextureUsage::ShaderRead);
	depthTextureDescriptor.SetUsage((mtlpp::TextureUsage)depthTextureUsage);
	mtlpp::Texture depthTexture = device.NewTexture(depthTextureDescriptor);

	m_renderPassDescriptor.GetDepthAttachment().SetTexture(depthTexture);
	m_renderPassDescriptor.GetDepthAttachment().SetLoadAction(mtlpp::LoadAction::Clear);
	m_renderPassDescriptor.GetDepthAttachment().SetStoreAction(mtlpp::StoreAction::Store);
	m_renderPassDescriptor.GetDepthAttachment().SetClearDepth(1.0);

	m_renderPassDescriptor.GetStencilAttachment().SetTexture(depthTexture);
	m_renderPassDescriptor.GetStencilAttachment().SetLoadAction(mtlpp::LoadAction::Clear);
	m_renderPassDescriptor.GetStencilAttachment().SetStoreAction(mtlpp::StoreAction::Store);
#endif // defined(__APPLE__)
}

FramebufferMl::~FramebufferMl() {
}

void FramebufferMl::bind(const Target& target) {
	if (target == Target::Draw) {
	} else if (target == Target::Read) {
	} else {
	}
}

void FramebufferMl::unbind(const Target& target) {
	if (target == Target::Draw) {
	} else if (target == Target::Read) {
	} else {
	}
}

std::unordered_map<std::string, Texture2DMl::Container> Texture2DMl::s_map;

Texture2DMl::Texture2DMl(const std::string& type, const std::string& filepath)
: Texture2D() {
}

Texture2DMl::~Texture2DMl() {
}

void Texture2DMl::bind() {
}

void Texture2DMl::unbind() {
}

Texture2DMl* Texture2DMl::allocate(const std::string& type, const std::string& filepath) {
	std::string key = type + ":" + filepath;
	if (s_map[key].count == 0) {
		s_map[key].count = 1;
		s_map[key].pointer = new Texture2DMl(type, filepath);
		return s_map[key].pointer;
	} else {
		s_map[key].count += 1;
		return s_map[key].pointer;
	}
}

void Texture2DMl::deallocate(const std::string& type, const std::string& filepath) {
	std::string key = type + ":" + filepath;
	if (s_map[key].count == 1) {
		s_map[key].count = 0;
		delete s_map[key].pointer;
		s_map[key].pointer = nullptr;
	} else {
		s_map[key].count -= 1;
	}
}

VisualMaterialMl::VisualMaterialMl(const Material& material)
: VisualMaterial(material) {
}

VisualMaterialMl::~VisualMaterialMl() {
}

void VisualMaterialMl::enable() {
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

void VisualMaterialMl::disable() {
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

void VisualMaterialMl::initialize() {
}

void VisualMaterialMl::deinitialize() {
}
