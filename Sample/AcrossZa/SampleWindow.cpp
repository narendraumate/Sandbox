//
//  SampleWindow.cpp
//
//
//  Created by Narendra Umate on 8/18/15.
//
//

#include "SampleWindow.h"

#include "Mathematics.h"

#include "Platform.h"

#if defined(_WIN32)
#include "RendererDx.h" // needs to be after SampleWindow.h
#endif // defined(_WIN32)

#if defined(__APPLE__)
#include "RendererMl.h" // needs to be after SampleWindow.h
#endif // defined(__APPLE__)

#include "RendererGl.h" // needs to be after SampleWindow.h

#include "RendererVk.h" // needs to be after SampleWindow.h

SampleWindow::SampleWindow(Parameters& parameters)
: BaseWindow(parameters) {
	create();

#if defined(COMPILE_API_ML)
	m_xDivs = 1;
	m_yDivs = 1;
#else
	m_xDivs = 2;
	m_yDivs = 2;
#endif

	Framebuffer::Descriptor descriptors[1];
	descriptors[0].color = { Framebuffer::Format::Color };
	descriptors[0].depth = { Framebuffer::Format::Depth };
	descriptors[0].stencil = {};

	for (int currentRenderPassIndex = 0; currentRenderPassIndex < sizeof(descriptors) / sizeof(Framebuffer::Descriptor); ++currentRenderPassIndex) {
		m_renderPassVector.push_back(new RenderPass());
		int cols = m_xDivs;
		int rows = m_yDivs;
		for (int i = 0; i < cols; ++i) {
			for (int j = 0; j < rows; ++j) {
				m_renderPassVector[currentRenderPassIndex]->m_framebufferVector.push_back(m_renderer->createFramebuffer(parameters.xSize / cols, parameters.ySize / rows, descriptors[currentRenderPassIndex]));
			}
		}
	}
}

SampleWindow::~SampleWindow() {
	for (auto renderPass : m_renderPassVector) {
		for (auto framebuffer : renderPass->m_framebufferVector) {
			m_renderer->destroyFramebuffer(framebuffer);
		}
		renderPass->m_framebufferVector.clear();
		delete renderPass;
	}
	m_renderPassVector.clear();
	destroy();
}

void SampleWindow::onResize(const int& xSize, const int& ySize) {
	BaseWindow::onResize(xSize, ySize);
	if (m_renderer) {
		m_renderer->setSize(xSize, ySize);
	}
	if (m_camera) {
		m_camera->setSize(xSize, ySize);
	}
	for (auto renderPass : m_renderPassVector) {
		std::vector<Framebuffer::Descriptor> descriptors;
		for (auto framebuffer : renderPass->m_framebufferVector) {
			Framebuffer::Descriptor descriptor = framebuffer->m_descriptor;
			descriptors.push_back(descriptor);
			m_renderer->destroyFramebuffer(framebuffer);
		}
		renderPass->m_framebufferVector.clear();
		for (auto descriptor : descriptors) {
			renderPass->m_framebufferVector.push_back(m_renderer->createFramebuffer(xSize / m_xDivs, ySize / m_yDivs, descriptor));
		}
	}
}

void SampleWindow::update(float dt) {
	BaseWindow::update(dt);

	// Begin Frame.
	m_renderer->beginFrame();

	// Update.
	std::unordered_map<size_t, std::list<SpatialBound3*>> hashSpatialBounds;
	{
		m_octree->update();

		// Collect Spatial Bounds.
		m_octree->collectTreeSpatialBounds(hashSpatialBounds, m_camera->getFrustum());
	}

	// Render.
	{
		int currentRenderPassIndex = 0;

		// Pass I.
		for (auto framebuffer : m_renderPassVector[currentRenderPassIndex]->m_framebufferVector) {
			m_renderer->bind(framebuffer, Framebuffer::Target::Draw);
			m_renderer->clear();
#if 0 // SUBDIVIDE
			m_renderer->setViewport(framebuffer->getWidth()/4, framebuffer->getHeight()/4, framebuffer->getWidth()/2, framebuffer->getHeight()/2); // 1 / 2
			//m_renderer->setViewport(framebuffer->getWidth()/8*3, framebuffer->getHeight()/8*3, framebuffer->getWidth()/4, framebuffer->getHeight()/4); // 1 / 4
#else
			m_renderer->setViewport(0, 0, framebuffer->getWidth(), framebuffer->getHeight());
#endif
			m_renderer->draw(m_octree, nullptr, Mat4::identity, m_camera->getViewMatrix(), m_camera->getViewProjectionMatrix(), m_visualEffectVector[0], m_spatialLightVector);
			for (const auto& pair : hashSpatialBounds) {
				size_t hash = pair.first;
				std::list<SpatialBound3*> spatialBounds = pair.second;
				m_renderer->draw(m_camera, spatialBounds, m_visualEffectVector, m_spatialLightVector);
			}
			m_renderer->unbind(framebuffer, Framebuffer::Target::Draw);
		}

		// Blit.
		int width = 0;
		int height = 0;
		getFramebufferSize(width, height);
		void* texture;
		void* drawable;
		this->getMetalHackBoth(&texture, &drawable);
		m_renderer->blit(m_renderPassVector[currentRenderPassIndex]->m_framebufferVector, m_xDivs, m_yDivs, width, height, texture, drawable);
	}

	// End Frame.
	m_renderer->endFrame();
}

void SampleWindow::onKeyDown(int k) {
	BaseWindow::onKeyDown(k);
	const float increment = 2.0f;
#if 0
	switch (k) {
		case KEY_UP: {
			Vec3 position = m_camera->getPosition();
			m_camera->setPosition(position + Vec3(0, 0, increment));
		} break;
		case KEY_DOWN: {
			Vec3 position = m_camera->getPosition();
			m_camera->setPosition(position + Vec3(0, 0, -increment));
		} break;
		case KEY_RIGHT: {
			Vec3 position = m_camera->getPosition();
			m_camera->setPosition(position + Vec3(increment, 0, 0));
		} break;
		case KEY_LEFT: {
			Vec3 position = m_camera->getPosition();
			m_camera->setPosition(position + Vec3(-increment, 0, 0));
		} break;
		case KEY_Q: {
			Vec3 position = m_camera->getPosition();
			m_camera->setPosition(position + Vec3(0, increment, 0));
		} break;
		case KEY_E: {
			Vec3 position = m_camera->getPosition();
			m_camera->setPosition(position + Vec3(0, -increment, 0));
		} break;
		default:
			break;
	}
#endif
}

void SampleWindow::create() {
	switch (m_type) {
		default:
		case None:
			break;
		case DirectX:
#if defined(_WIN32)
			m_renderer = new RendererDx(m_xSize, m_ySize);
#endif // defined(_WIN32)
			break;
		case Metal:
#if defined(__APPLE__)
			m_renderer = new RendererMl(m_xSize, m_ySize);
#endif // defined(__APPLE__)
			break;
		case OpenGL:
			m_renderer = new RendererGl(m_xSize, m_ySize);
			break;
		case Vulkan:
			m_renderer = new RendererVk(m_xSize, m_ySize);
			break;
	}

	if (m_renderer) {
		ProjectionVariation projectionVariation = Orthographic;
		m_camera = new SpatialCamera(projectionVariation, Right, m_renderer->getProjectionDimension(), Reverse, Infinite, Transform3());
		if (m_camera->getProjectionVariation() == Orthographic) {
			m_camera->setFrame(Vec3(200.0f, 200.0f, 200.0f), Vec3(0, 0, 0));
		} else if (m_camera->getProjectionVariation() == Perspective) {
			m_camera->setFrame(Vec3(0.0f, 47.5f, 47.5f), Vec3(0, -50, -100));
		}
		m_renderer->setProjectionDirection(m_camera->getProjectionParameter().isForward() ? Forward : Reverse);

		m_bvh = new Bvh(0, Bound3(Vec3::zero, Vec3(250.0f, 250.0f, 250.0f)));

		m_octree = new Octree(0, Bound3(Vec3::zero, Vec3(250.0f, 250.0f, 250.0f)));

		// Setup Meshes.
		std::vector<Mesh> objMeshVector;
		std::vector<Material> objMaterialVector;
		std::string filepath = getAssetDirectory() + "/house.obj";
		readMesh(filepath, objMeshVector, objMaterialVector);
		for (int j = 0; j < 2; ++j) {
			for (int i = 0; i < objMeshVector.size(); ++i) {
				printMesh(&objMeshVector[i], &objMaterialVector[i]);
				SpatialBoundVisual3* spatialBoundVisual = m_renderer->createVisualSpatial(hashCombine(std::hash<std::string>{}(filepath), i), objMeshVector[i], objMaterialVector[i]);
				if (projectionVariation == Orthographic) {
					spatialBoundVisual->setScale(1.0f);
					if (j == 0) {
						spatialBoundVisual->setTranslate(Vec3(-35, -11, 0));
					} else {
						spatialBoundVisual->setTranslate(Vec3(+35, -11, 0));
					}
				} else {
					if (j == 0) {
						spatialBoundVisual->setTranslate(Vec3(-37, 0, 0));
					} else {
						spatialBoundVisual->setTranslate(Vec3(+37, 0, 0));
					}
				}
				m_octree->insert(spatialBoundVisual);
			}
		}

		// Setup Spatial Lights.
		Bound3 bound = m_octree->getBoundingBox();
		for (int i = 0; i < 16; ++i) {
			Coeff3f coeff = Coeff3f(rand(1.0f, 4.0f));
			Color3f color = Color3f::random();
			Transform3 transform = Transform3(randomPoint(bound));
			m_spatialLightVector.push_back(new SpatialLight(coeff, color, transform));
		}

		// Setup Visual Effects.
		Effect effects[2];
		effects[0].vertexShader = effects[0].fragmentShader = "ForwardGouraud";
		effects[1].vertexShader = effects[1].fragmentShader = "ForwardPhong";
		m_visualEffectVector.push_back(m_renderer->createVisualEffect(effects[0]));
		m_visualEffectVector.push_back(m_renderer->createVisualEffect(effects[1]));
	}
}

void SampleWindow::destroy() {
	// Destroy Visual Effects.
	for (auto visualEffect : m_visualEffectVector) {
		m_renderer->destroyVisualEffect(visualEffect);
	}

	// Destroy Spatial Lights.
	for (auto spatialLight : m_spatialLightVector) {
		delete spatialLight;
	}
	m_spatialLightVector.clear();

	// Destroy Meshes.
	std::unordered_map<size_t, std::list<SpatialBound3*>> hashSpatialBounds;
	m_octree->collectTreeSpatialBounds(hashSpatialBounds);
	for (const auto& pair : hashSpatialBounds) {
		size_t hash = pair.first;
		std::list<SpatialBound3*> spatialBounds = pair.second;
		for (auto spatialBound : spatialBounds) {
			bool removed = m_octree->remove(spatialBound);
			SpatialBoundVisual3* spatialBoundVisual = dynamic_cast<SpatialBoundVisual3*>(spatialBound);
			if (spatialBoundVisual && removed) {
				m_renderer->destroyVisualSpatial(spatialBoundVisual);
			}
		}
	}

	if (m_octree) {
		delete m_octree;
	}

	if (m_bvh) {
		delete m_bvh;
	}

	if (m_camera) {
		delete m_camera;
	}

	if (m_renderer) {
		delete m_renderer;
	}
}
