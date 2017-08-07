//
//  SampleWindow.cpp
//
//
//  Created by Narendra Umate on 8/18/15.
//
//

#include "SampleWindow.h"

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

	m_xDivs = 2;
	m_yDivs = 2;

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
	std::list<SpatialBoundVisual3*> spatialBoundVisuals;
	{
		m_octree->update();

		// Collect Volumes.
		std::list<SpatialBound3*> spatialBounds;
		m_octree->collectTreeSpatialBounds(spatialBounds, m_camera->getFrustum());
		static int spatialsSize = -1;
		if (spatialBounds.size() != spatialsSize) {
			spatialsSize = spatialBounds.size();
			std::cout << "Spatials size: " << spatialsSize << "\n";
		}

		// Collect Visuals.
		for (auto spatialBound : spatialBounds) {
			SpatialBoundVisual3* spatialBoundVisual = dynamic_cast<SpatialBoundVisual3*>(spatialBound);
			if (spatialBoundVisual) {
				spatialBoundVisuals.push_back(spatialBoundVisual);
			}
		}
	}

	// Render.
	{
		int currentRenderPassIndex = 0;

		// Pass I.
		Mat4 projectionMatrix = m_camera->getProjectionMatrix();
		Mat4 viewMatrix = m_camera->getViewMatrix();
		Vec3 cameraTranslation = m_camera->getTranslate();
		for (auto framebuffer : m_renderPassVector[currentRenderPassIndex]->m_framebufferVector) {
			m_renderer->bind(framebuffer, Framebuffer::Target::Draw);
			m_renderer->clear();
			m_renderer->setViewport(0, 0, framebuffer->getWidth(), framebuffer->getHeight());
			static int spatialBoundVisualIndex = 0;
			for (auto spatialBoundVisual : spatialBoundVisuals) {
				spatialBoundVisual->setViewProjectionMatrix(projectionMatrix, viewMatrix);
				m_renderer->draw(spatialBoundVisual, m_visualEffectVector[spatialBoundVisualIndex++ % 2], cameraTranslation, m_spatialLightVector);
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
		m_camera = new SpatialCamera(Perspective, m_renderer->getNdcMatrix());
		if (m_camera->getProjectionType() == Orthographic) {
			float s = 100.0f;
			float l = float(m_xSize) / float(m_ySize) / -2.0f * s;
			float r = -l;
			float b = -0.5f * s;
			float t = -b;
			Vec3 position = Vec3(-100.0f, +100.0f, +100.0f);
			Quat rotation = Quat(Vec3(0.0f, 1.0f, 0.0f), -M_PI_4) * Quat(Vec3(1.0f, 0.0f, 0.0f), atanf(-1.0f / sqrtf(2.0f)));
			m_camera->setRotate(rotation);
			m_camera->setTranslate(position);
			m_camera->setFrustum(l, r, b, t, 1.0f, 1000.0f);
		} else if (m_camera->getProjectionType() == Perspective) {
			m_camera->setFrame(Vec3(+100.0f, +100.0f, +100.0f), Vec3::zero);
			m_camera->setFrustum(65.0f * (M_PI / 180.0f), float(m_xSize) / float(m_ySize), 1.0f, 1000.0f);
		}
		m_renderer->setProjectionRange(m_camera->getProjectionRange());

		m_bvh = new Bvh(0, Bound3(Vec3::zero, Vec3(100.0f, 100.0f, 100.0f)));

		m_octree = new Octree(0, Bound3(Vec3::zero, Vec3(100.0f, 100.0f, 100.0f)));

		// Setup Meshes.
		std::vector<Mesh> objMeshVector;
		std::vector<Material> objMaterialVector;
		readMesh(getAssetDirectory() + "/house.obj", objMeshVector, objMaterialVector);
		for (int i = 0; i < objMeshVector.size(); ++i) {
			printMesh(&objMeshVector[i], &objMaterialVector[i]);
			SpatialBoundVisual3* spatialBoundVisual = m_renderer->createVisualSpatial(objMeshVector[i], objMaterialVector[i]);
			m_octree->insert(spatialBoundVisual);
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
	std::list<SpatialBound3*> spatialBounds;
	m_octree->collectTreeSpatialBounds(spatialBounds);
	for (auto spatialBound : spatialBounds) {
		bool removed = m_octree->remove(spatialBound);
		SpatialBoundVisual3* spatialBoundVisual = dynamic_cast<SpatialBoundVisual3*>(spatialBound);
		if (spatialBoundVisual && removed) {
			m_renderer->destroyVisualSpatial(spatialBoundVisual);
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
