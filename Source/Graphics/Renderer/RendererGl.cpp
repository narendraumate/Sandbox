//
//  RendererGl.cpp
//
//
//  Created by Narendra Umate on 9/7/13.
//
//

#if defined(__APPLE__) || defined(__linux__) || defined(_WIN32)

#include "RendererGl.h"

#if defined(__APPLE__) || defined(_WIN32)
#include <chrono> // For chrono
#include <iomanip> // For timestamp
#endif

#include "glad/glad.h"

RendererGl::RendererGl(const int& width, const int& height)
: Renderer(width, height) {
	m_projectionDimension = X2Y2Z2;
	// initialize
	Effect quadEffect;
	quadEffect.vertexShader = "Quad";
	quadEffect.fragmentShader = "Quad";
	m_quadVisualScreenSpace = createVisualScreenSpace(quadEffect);
#if 0
	// setup things this is not really needed but good to be explicit so leaving it here
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); // OpenGL works with Back Face Right Handed Projection Counter Clockwise winding
	glFrontFace(GL_CCW); // OpenGL works with Back Face Right Handed Projection Counter Clockwise winding
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // GL_FILL or GL_LINE
#endif
}

RendererGl::~RendererGl() {
}

void RendererGl::clear() {
	static int index = 0;
	static int frame = 0;

	switch (index % 4) {
		case 0:
			glClearColor(1.0f, float(frame % 256) / 255.0f, float(frame % 256) / 255.0f, 1.0f);
			break;
		case 1:
			glClearColor(float(frame % 256) / 255.0f, 1.0f, float(frame % 256) / 255.0f, 1.0f);
			break;
		case 2:
			glClearColor(float(frame % 256) / 255.0f, float(frame % 256) / 255.0f, 1.0f, 1.0f);
			break;
		default:
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			break;
	}

	++index;
	++frame;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void RendererGl::setClearColor(const Color4f& clearColor) {
	m_clearColor = clearColor;
	glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
}

Color4f RendererGl::getClearColor() const {
	return m_clearColor;
}

void RendererGl::setClearDepth(const float& clearDepth) {
	m_clearDepth = clearDepth;
	glClearDepth(m_clearDepth);
}

float RendererGl::getClearDepth() const {
	return m_clearDepth;
}

void RendererGl::setClearStencil(const unsigned int& clearStencil) {
	m_clearStencil = clearStencil;
	glClearStencil(m_clearStencil);
}

unsigned int RendererGl::getClearStencil() const {
	return m_clearStencil;
}

void RendererGl::setViewport(const int& xPos, const int& yPos, const int& w, const int& h) {
	glViewport(xPos, yPos, w, h);
}

void RendererGl::getViewport(int& xPos, int& yPos, int& w, int& h) const {
	int params[4];
	glGetIntegerv(GL_VIEWPORT, params);
	xPos = params[0];
	yPos = params[1];
	w = params[2];
	h = params[3];
}

void RendererGl::setSize(const int& width, const int& height) {
	m_width = width;
	m_height = height;
}

void RendererGl::getSize(int& width, int& height) {
	width = m_width;
	height = m_height;
}

void RendererGl::blit(const std::vector<Framebuffer*>& framebuffers, const int& cols, const int& rows, const int& width, const int& height, void* texture, void* drawable) {
	if (cols * rows == framebuffers.size()) {
		// int width = 0;
		// int height = 0;
		// m_window->getFramebufferSize(width, height);
		if (width > 0 && height > 0) {
#if 0
			// This is now untested section.
			// Write to default framebuffer.
			{
				glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
				for (int col = 0; col < cols; ++col) {
					for (int row = 0; row < rows; ++row) {
						framebuffers[col]->bind(Framebuffer::Target::Read);
						if (col == 0) {
							glViewport(0, 0, width / 2, height);
							glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
						} else if (col == 1) {
							glViewport(0, 0, width / 2, height);
							glBlitFramebuffer(0, 0, width / 2, height, width / 2, 0, width, height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
						}
						framebuffers[col]->unbind(Framebuffer::Target::Read);
					}
				}
			}
#else
			// Write to default framebuffer.
			{
				glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
				glUseProgram(dynamic_cast<VisualEffectGl*>(m_quadVisualScreenSpace->getVisualEffect())->m_program);
				m_quadVisualScreenSpace->getVertexBuffer()->bind();
				m_quadVisualScreenSpace->getIndexBuffer(0)->bind();

				glEnable(GL_DEPTH_CLAMP);

				// this flickers
				// is not needed
				// draw 1 colors
				/*{
					GLsizei n = 2;
					const GLenum bufs[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
					glDrawBuffers(n, bufs);
				}*/

				for (int col = 0; col < cols; ++col) {
					for (int row = 0; row < rows; ++row) {
						unsigned int index = (rows - row - 1) * rows + col;
						m_quadVisualScreenSpace->getVisualEffect()->setSampler("colorTexture", 0, *((unsigned int*)framebuffers[index]->m_colorTexture));
						m_quadVisualScreenSpace->getVisualEffect()->setSampler("depthTexture", 1, *((unsigned int*)framebuffers[index]->m_depthTexture));
						int x = col * width / cols;
						int y = row * height / rows;
						int w = width / cols;
						int h = height / rows;
						glViewport(x, y, w, h);
						glDrawElements(GL_TRIANGLES, static_cast<int>(m_quadVisualScreenSpace->getIndexBuffer(0)->getIndexCount()), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
					}
				}

				// this flickers
				// is not needed
				// draw 0 colors
				/*{
					GLsizei n = 1;
					const GLenum bufs[1] = { GL_NONE };
					glDrawBuffers(n, bufs);
				}*/

				glDisable(GL_DEPTH_CLAMP);

				m_quadVisualScreenSpace->getIndexBuffer(0)->unbind();
				m_quadVisualScreenSpace->getVertexBuffer()->unbind();
				glUseProgram(0);
			}
#endif
#if 0
			// Write framebuffer to file.
			{
				std::ostringstream ss;
#if defined(__APPLE__)
				auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
				ss << std::put_time(gmtime(&now), "%c %Z");
#else
				static unsigned int index = 0;
				ss << index;
				++index;
#endif
				glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
				unsigned char* pixels = (unsigned char*)malloc((int)(width * height * 4));
				glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
				std::string result = getExecutableDirectory() + ss.str() + ".bmp";
				save(result, width, height, pixels);
				free(pixels);
			}
#endif
		}
	}
}

void RendererGl::beginFrame() {
	ProjectionDirection projectionDirection = m_projectionDirection;
	if (projectionDirection == Forward) {
		glDepthFunc(GL_LESS);
		glDepthRange(0.0, 1.0);
		glClearDepth(static_cast<double>(m_clearDepth));
	} else {
		glDepthFunc(GL_GREATER);
		glDepthRange(0.0, 1.0);
		glClearDepth(1.0 - static_cast<double>(m_clearDepth));
	}
}

void RendererGl::endFrame() {
}

void RendererGl::bind(Framebuffer* framebuffer, const Framebuffer::Target& target) {
	framebuffer->bind(target);
}

void RendererGl::unbind(Framebuffer* framebuffer, const Framebuffer::Target& target) {
	framebuffer->unbind(target);
}

SpatialBoundVisual3* RendererGl::createVisualSpatial(const size_t& hash, const Mesh& mesh, const Material& material) {
	return new SpatialBoundVisualGl3(hash, mesh, material);
}

VisualEffect* RendererGl::createVisualEffect(const Effect& effect) {
	return new VisualEffectGl(effect);
}

void RendererGl::draw(SpatialCamera* camera, std::list<SpatialBound3*> bounds, std::vector<VisualEffect*> visualEffects, const std::vector<SpatialLight*>& spatialLights) {
	size_t visualEffectIndex = 0;
	for (auto bound : bounds) {
		VisualEffect* visualEffect = visualEffects.at(visualEffectIndex++ % visualEffects.size());
		SpatialBoundVisual3* visual = dynamic_cast<SpatialBoundVisual3*>(bound);
		visual->setViewProjectionMatrix(camera->getViewProjectionMatrix(), camera->getProjectionMatrix());

		VisualEffectGl* visualEffectGl = dynamic_cast<VisualEffectGl*>(visualEffect);
		VisualMaterialGl* visualMaterialGl = dynamic_cast<VisualMaterialGl*>(visual->getVisualMaterial());
		VertexBufferGl* vertexBufferGl = dynamic_cast<VertexBufferGl*>(visual->getVertexBuffer());
		unsigned int visualIndexBufferCount = visual->getIndexBufferCount();
		if (visualMaterialGl && visualEffectGl && vertexBufferGl && visualIndexBufferCount) {
			// Enable Effect.
			glUseProgram(visualEffectGl->m_program);
			// Set range zero to one.
			ProjectionDirection projectionDirection = m_projectionDirection;
			int rangeZeroToOne = projectionDirection == Forward;
			visualEffectGl->set1iv("rangeZeroToOne", 1, &rangeZeroToOne);
			// Set light and eye parameters.
			int lightCount = spatialLights.size();
			std::vector<Vec3> lightCoefficients(lightCount);
			std::vector<Vec3> lightColor(lightCount);
			std::vector<Vec3> lightPosition(lightCount);
			for (unsigned int lightIndex = 0; lightIndex < lightCount; ++lightIndex) {
				lightCoefficients[lightIndex] = spatialLights[lightIndex]->getCoefficients().vec3();
				lightColor[lightIndex] = spatialLights[lightIndex]->getColor().vec3();
				lightPosition[lightIndex] = spatialLights[lightIndex]->getTranslate();
			}
			visualEffectGl->set1iv("lightCount", 1, &lightCount);
			visualEffectGl->set3fv("lightCoefficients", lightCount, (const float*)&lightCoefficients.front());
			visualEffectGl->set3fv("lightColor", lightCount, (const float*)&lightColor.front());
			visualEffectGl->set3fv("lightPosition", lightCount, (const float*)&lightPosition.front());
			Mat4 inverseViewMatrix = camera->getTransform();
			visualEffectGl->set3fv("eyePosition", 1, &inverseViewMatrix.d[3][0]);
			// Set World View Norm Matrix.
			visualEffectGl->setMatrix3fv("worldNormal", 1, GL_FALSE, (float*)&(visual->getWorldNormalMatrix()));
			// Set World View Projection Matrix.
			visualEffectGl->setMatrix4fv("worldViewProjection", 1, GL_FALSE, (float*)&(visual->getWorldViewProjectionMatrix()));
			// Set Parameters.
			visualEffectGl->set3fv("ambient", 1, visualMaterialGl->ambient);
			visualEffectGl->set3fv("diffuse", 1, visualMaterialGl->diffuse);
			visualEffectGl->set3fv("specular", 1, visualMaterialGl->specular);
			visualEffectGl->set3fv("transmittance", 1, visualMaterialGl->transmittance);
			visualEffectGl->set3fv("emission", 1, visualMaterialGl->emission);
			visualEffectGl->set1fv("shininess", 1, &visualMaterialGl->shininess);
			visualEffectGl->set1fv("ior", 1, &visualMaterialGl->ior);
			visualEffectGl->set1fv("dissolve", 1, &visualMaterialGl->dissolve);
			visualEffectGl->set1iv("illum", 1, &visualMaterialGl->illum);
			// Enable Material.
			visualMaterialGl->enable();
			////////////////////////////////////////////////////////////////////////
			// Bind Vertex Buffer.
			vertexBufferGl->bind();
			for (int indexBufferIndex = 0; indexBufferIndex < 5; ++indexBufferIndex) {
				IndexBuffer* indexBuffer = visual->getIndexBuffer(indexBufferIndex);
				if (indexBuffer) {
					IndexBufferGl* indexBufferGl = dynamic_cast<IndexBufferGl*>(indexBuffer);
					if (indexBufferGl) {
						// Bind Index Buffer.
						indexBufferGl->bind();
						glEnable(GL_DEPTH_TEST);
						glEnable(GL_DEPTH_CLAMP);
						switch (indexBufferGl->getType()) {
							case IndexBuffer::Point:
								glDrawElements(GL_POINTS, static_cast<int>(indexBufferGl->getIndexCount()), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
								break;
							case IndexBuffer::Line:
								glDrawElements(GL_LINES, static_cast<int>(indexBufferGl->getIndexCount()), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
								break;
							case IndexBuffer::Triangle:
								glDrawElements(GL_TRIANGLES, static_cast<int>(indexBufferGl->getIndexCount()), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
								break;
							default:
								break;
						}
						glDisable(GL_DEPTH_CLAMP);
						glDisable(GL_DEPTH_TEST);
						// Unbind Index Buffer.
						indexBufferGl->unbind();
					}
				}
			}
			// Unbind Vertex Buffer.
			vertexBufferGl->unbind();
			////////////////////////////////////////////////////////////////////////
			// Disable Material.
			visualMaterialGl->disable();
			// Disable Effect.
			glUseProgram(0);
		}
	}
}

void RendererGl::destroyVisualEffect(VisualEffect* visualEffect) {
	VisualEffectGl* visualEffectGl = dynamic_cast<VisualEffectGl*>(visualEffect);
	delete visualEffectGl;
}

void RendererGl::destroyVisualSpatial(SpatialBoundVisual3* visualSpatial) {
	SpatialBoundVisualGl3* glVisualSpatial = dynamic_cast<SpatialBoundVisualGl3*>(visualSpatial);
	delete glVisualSpatial;
}

VisualScreenSpace* RendererGl::createVisualScreenSpace(const Effect& effect) {
	return new VisualScreenSpaceGl(effect);
}

void RendererGl::draw(VisualScreenSpace* visualScreenSpace, Framebuffer* oldFramebuffer, Framebuffer* newFramebuffer, const Mat4& oldViewProjectionStraight, const Mat4& oldViewProjectionInverted, const Mat4& newViewProjectionStraight, const Mat4& newViewProjectionInverted) {
	glUseProgram(dynamic_cast<VisualEffectGl*>(visualScreenSpace->getVisualEffect())->m_program);

	visualScreenSpace->getVisualEffect()->setMatrix4fv("oldViewProjectionStraight", 1, GL_FALSE, (float*)&(oldViewProjectionStraight));
	visualScreenSpace->getVisualEffect()->setMatrix4fv("oldViewProjectionInverted", 1, GL_FALSE, (float*)&(oldViewProjectionInverted));
	visualScreenSpace->getVisualEffect()->setMatrix4fv("newViewProjectionStraight", 1, GL_FALSE, (float*)&(newViewProjectionStraight));
	visualScreenSpace->getVisualEffect()->setMatrix4fv("newViewProjectionInverted", 1, GL_FALSE, (float*)&(newViewProjectionInverted));

	visualScreenSpace->getVisualEffect()->setSampler("oldColorTexture", 0, ((unsigned int*)oldFramebuffer->m_colorTexture)[0]);
	visualScreenSpace->getVisualEffect()->setSampler("oldDepthTexture", 1, ((unsigned int*)oldFramebuffer->m_depthTexture)[0]);

	VertexBufferGl* vertexBufferGl = dynamic_cast<VertexBufferGl*>(visualScreenSpace->getVertexBuffer());
	IndexBufferGl* indexBufferGl = dynamic_cast<IndexBufferGl*>(visualScreenSpace->getIndexBuffer(0));

	vertexBufferGl->bind();
	indexBufferGl->bind();
	glDrawElements(GL_TRIANGLES, static_cast<int>(indexBufferGl->getIndexCount()), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
	indexBufferGl->unbind();
	vertexBufferGl->unbind();

	glUseProgram(0);
}

void RendererGl::destroyVisualScreenSpace(VisualScreenSpace* visualScreenSpace) {
	VisualScreenSpaceGl* glVisualScreenSpace = dynamic_cast<VisualScreenSpaceGl*>(visualScreenSpace);
	delete glVisualScreenSpace;
}

void RendererGl::draw(const Bound3& bound, VisualMaterial* visualMaterial, const Mat4& worldMatrix, const Mat4& viewMatrix, const Mat4& viewProjectionMatrix, VisualEffect* visualEffect, const std::vector<SpatialLight*>& spatialLights) {
#if 1
	// Initialize resources at highest level.
	if (true) {
		Mat3 worldNormalMatrix = Mat3(worldMatrix).inverse().transpose();
		Mat4 worldViewProjectionMatrix = viewProjectionMatrix * worldMatrix;

		VisualEffectGl* visualEffectGl = dynamic_cast<VisualEffectGl*>(visualEffect);
		VisualMaterialGl* visualMaterialGl = dynamic_cast<VisualMaterialGl*>(visualMaterial);

		// Enable Effect.
		glUseProgram(visualEffectGl->m_program);
		// Set range zero to one.
		ProjectionDirection projectionDirection = m_projectionDirection;
		int rangeZeroToOne = projectionDirection == Forward;
		visualEffectGl->set1iv("rangeZeroToOne", 1, &rangeZeroToOne);
		// Set light and eye parameters.
		int lightCount = 1;
		// visualEffectGl->set1iv("lightCount", 1, &lightCount);
		// visualEffectGl->set3fv("lightCoefficients", lightCount, lightCoefficients.data);
		// visualEffectGl->set3fv("lightColor", lightCount, lightColor.data);
		// visualEffectGl->set3fv("lightPosition", lightCount, lightPosition.data);
		// visualEffectGl->set3fv("eyePosition", 1, eyePosition.data);
		//  Set World View Norm Matrix.
		visualEffectGl->setMatrix3fv("worldNormal", 1, GL_FALSE, (float*)&(worldNormalMatrix));
		// Set World View Projection Matrix.
		visualEffectGl->setMatrix4fv("worldViewProjection", 1, GL_FALSE, (float*)&(worldViewProjectionMatrix));
		// Set Parameters.
		// visualEffectGl->set3fv("ambient", 1, visualMaterialGl->ambient);
		// visualEffectGl->set3fv("diffuse", 1, visualMaterialGl->diffuse);
		// visualEffectGl->set3fv("specular", 1, visualMaterialGl->specular);
		// visualEffectGl->set3fv("transmittance", 1, visualMaterialGl->transmittance);
		// visualEffectGl->set3fv("emission", 1, visualMaterialGl->emission);
		// visualEffectGl->set1fv("shininess", 1, &visualMaterialGl->shininess);
		// visualEffectGl->set1fv("ior", 1, &visualMaterialGl->ior);
		// Enable Material.
		// visualMaterialGl->enable();
	}

	// Draw your self.
	{
		// Scoped this draw to destroy buffers right away.
		std::vector<P4N4T4B4T2P2> vertices;
		bound.getCornerVertices(vertices);
		VertexBufferGl vertexBufferGl(vertices);
		std::vector<unsigned int> indices;
		bound.getCornerIndices(indices);
		IndexBufferGl indexBufferGl(IndexBufferGl::Line, indices);
		vertexBufferGl.bind();
		indexBufferGl.bind();
		glDrawElements(GL_LINES, static_cast<int>(indexBufferGl.getIndexCount()), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
		indexBufferGl.unbind();
		vertexBufferGl.unbind();
	}

	// Deinitialize resources at highest level.
	if (true) {
		// Disable Material.
		// VisualMaterialGl* visualMaterialGl = dynamic_cast<VisualMaterialGl*>(visualMaterial);
		// visualMaterialGl->disable();

		// Disable Effect.
		glUseProgram(0);
	}
#endif
}

Framebuffer* RendererGl::createFramebuffer(const int& width, const int& height, const Framebuffer::Descriptor& descriptor) {
	return new FramebufferGl(width, height, descriptor);
}

void RendererGl::destroyFramebuffer(Framebuffer* framebuffer) {
	FramebufferGl* glFramebuffer = dynamic_cast<FramebufferGl*>(framebuffer);
	delete glFramebuffer;
}

#endif // defined(__APPLE__) || defined(__linux__) || defined(_WIN32)
