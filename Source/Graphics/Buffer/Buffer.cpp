//
//  Buffer.cpp
//
//
//  Created by Narendra Umate on 8/21/16.
//
//

#include "Buffer.h"

VertexBuffer::VertexBuffer(const std::vector<P2T2>& vertices) {
	m_vertexFormat = Format::FormatP2T2;
	m_vertexCount = vertices.size();
	m_vertices = new float[m_vertexCount * sizeof(P2T2)];
	std::memcpy(m_vertices, &(vertices[0].position.x), m_vertexCount * sizeof(P2T2));
}

VertexBuffer::VertexBuffer(const std::vector<P4N4T4B4T2P2>& vertices) {
	m_vertexFormat = Format::FormatP4N4T4B4T2P2;
	m_vertexCount = vertices.size();
	m_vertices = new float[m_vertexCount * sizeof(P4N4T4B4T2P2)];
	std::memcpy(m_vertices, &(vertices[0].position.x), m_vertexCount * sizeof(P4N4T4B4T2P2));
}

VertexBuffer::~VertexBuffer() {
	delete[] m_vertices;
}

void VertexBuffer::bind() {
}

void VertexBuffer::unbind() {
}

void VertexBuffer::initialize() {
}

void VertexBuffer::deinitialize() {
}

float* VertexBuffer::getVertices() {
	return m_vertices;
}

unsigned long VertexBuffer::getVertexCount() {
	return m_vertexCount;
}

IndexBuffer::IndexBuffer(const Type& type, const std::vector<unsigned int>& indices, const std::vector<unsigned int>& counts)
: m_type(type)
, m_indices(indices)
, m_counts(counts) {
}

IndexBuffer::~IndexBuffer() {
}

void IndexBuffer::bind() {
}

void IndexBuffer::unbind() {
}

void IndexBuffer::initialize() {
}

void IndexBuffer::deinitialize() {
}

IndexBuffer::Type IndexBuffer::getType() {
	return m_type;
}

unsigned int* IndexBuffer::getIndices() {
	return &m_indices[0];
}

unsigned long IndexBuffer::getIndexCount() {
	return m_indices.size();
}

VisualEffect::VisualEffect(const Effect& effect) {
}

VisualEffect::~VisualEffect() {
}

void VisualEffect::set1iv(const char* name, unsigned int count, const int* v0) {
}

void VisualEffect::set1fv(const char* name, unsigned int count, const float* v0) {
}

void VisualEffect::set2iv(const char* name, unsigned int count, const int* v0) {
}

void VisualEffect::set2fv(const char* name, unsigned int count, const float* v0) {
}

void VisualEffect::set3iv(const char* name, unsigned int count, const int* v0) {
}

void VisualEffect::set3fv(const char* name, unsigned int count, const float* v0) {
}

void VisualEffect::set4iv(const char* name, unsigned int count, const int* v0) {
}

void VisualEffect::set4fv(const char* name, unsigned int count, const float* v0) {
}

void VisualEffect::setMatrix3fv(const char* name, unsigned int count, bool transpose, const float* v0) {
}

void VisualEffect::setMatrix4fv(const char* name, unsigned int count, bool transpose, const float* v0) {
}

void VisualEffect::set1iv(int location, unsigned int count, const int* v0) {
}

void VisualEffect::set1fv(int location, unsigned int count, const float* v0) {
}

void VisualEffect::set2iv(int location, unsigned int count, const int* v0) {
}

void VisualEffect::set2fv(int location, unsigned int count, const float* v0) {
}

void VisualEffect::set3iv(int location, unsigned int count, const int* v0) {
}

void VisualEffect::set3fv(int location, unsigned int count, const float* v0) {
}

void VisualEffect::set4iv(int location, unsigned int count, const int* v0) {
}

void VisualEffect::set4fv(int location, unsigned int count, const float* v0) {
}

void VisualEffect::setMatrix3fv(int location, unsigned int count, bool transpose, const float* v0) {
}

void VisualEffect::setMatrix4fv(int location, unsigned int count, bool transpose, const float* v0) {
}

void VisualEffect::setSampler(const char* name, int location, const unsigned int& texture) {
}

Framebuffer::Framebuffer(const int& width, const int& height, const Framebuffer::Descriptor& descriptor)
: m_width(width)
, m_height(height) {
	m_descriptor = descriptor;
}

Framebuffer::~Framebuffer() {
}

void Framebuffer::bind(const Target& target) {
}

void Framebuffer::unbind(const Target& target) {
}

int Framebuffer::getWidth() {
	return m_width;
}

int Framebuffer::getHeight() {
	return m_height;
}

Texture2D::Texture2D() {
}

Texture2D::~Texture2D() {
}

void Texture2D::bind() {
}

void Texture2D::unbind() {
}

VisualMaterial::VisualMaterial(const Material& material)
: ambient_texname(material.ambient_texname)
, diffuse_texname(material.diffuse_texname)
, specular_texname(material.specular_texname)
, specular_highlight_texname(material.specular_highlight_texname)
, bump_texname(material.bump_texname)
, displacement_texname(material.displacement_texname)
, alpha_texname(material.alpha_texname)
, ambient_texture(nullptr)
, diffuse_texture(nullptr)
, specular_texture(nullptr)
, specular_highlight_texture(nullptr)
, bump_texture(nullptr)
, displacement_texture(nullptr)
, alpha_texture(nullptr) {
	std::memcpy(ambient, &material.ambient, sizeof(float) * 3);
	std::memcpy(diffuse, &material.diffuse, sizeof(float) * 3);
	std::memcpy(specular, &material.specular, sizeof(float) * 3);
	std::memcpy(transmittance, &material.transmittance, sizeof(float) * 3);
	std::memcpy(emission, &material.emission, sizeof(float) * 3);
	shininess = material.shininess;
	ior = material.ior;
	dissolve = material.dissolve;
	illum = material.illum;
}

VisualMaterial::~VisualMaterial() {
}

void VisualMaterial::enable() {
}

void VisualMaterial::disable() {
}
