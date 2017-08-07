//
//  RenderSystem.h
//
//
//  Created by Narendra Umate on 8/18/16.
//
//

#pragma once

#define USE_CAMERA_COMPONENT 1

#define USE_LIGHT_COMPONENT 0

#define USE_BOUND_VISUAL_COMPONENT 0

#include <memory>

#if defined(_WIN32)
#include "RendererDx.h" // needs to be after SampleWindow.h
#endif // defined(_WIN32)

#if defined(__APPLE__)
#include "RendererMl.h" // needs to be after SampleWindow.h
#endif // defined(__APPLE__)

#if 1
#include "RendererGl.h" // needs to be after SampleWindow.h
#endif

#if 1
#include "RendererVk.h" // needs to be after SampleWindow.h
#endif

#include "ECS.h"

#include "Component.h"

#include "Event.h"

using namespace ECS;

class RenderSystem : public EntitySystem, public EventSubscriber<EventWindowCreated>, public EventSubscriber<EventWindowResized>, public EventSubscriber<EventWindowDestroyed> {
public:
	RenderSystem() {
	}

	~RenderSystem() {
	}

	// EntitySystem

	virtual void configure(World* world) override {
		world->subscribe<EventWindowCreated>(this);
		world->subscribe<EventWindowResized>(this);
		world->subscribe<EventWindowDestroyed>(this);
	}

	virtual void unconfigure(World* world) override {
		world->unsubscribe<EventWindowCreated>(this);
		world->unsubscribe<EventWindowResized>(this);
		world->unsubscribe<EventWindowDestroyed>(this);
	}

	virtual void tick(World* world, float deltaTime) override {
		// Begin Frame.
		m_renderer->beginFrame();

		// Render.
		{
			int currentRenderPassIndex = 0;

			// Pass I.
#if USE_CAMERA_COMPONENT
			auto cameraProjection = m_camera->get<CameraComponent>();
			Mat4 projectionMatrix = cameraProjection->projection;
			auto cameraTransform = m_camera->get<TransformComponent>();
			Mat4 viewMatrix = Transform3(cameraTransform->translation, cameraTransform->rotation, cameraTransform->scale).getMatrix().inverse();
			Vec3 cameraTranslation = cameraTransform->translation;
#else
			Mat4 projectionMatrix = m_camera->getProjectionMatrix();
			Mat4 viewMatrix = m_camera->getViewMatrix();
			Vec3 cameraTranslation = m_camera->getTranslate();
#endif
			/*for (auto framebuffer : m_renderPassVector[currentRenderPassIndex]->m_framebufferVector) {
				m_renderer->bind(framebuffer, Framebuffer::Target::Draw);
				m_renderer->clear();
				m_renderer->setViewport(0, 0, framebuffer->getWidth(), framebuffer->getHeight());
				static int spatialBoundVisualIndex = 0;
				for (auto entity : world->each<VisualComponent>()) {
					auto const& modelTransform = entity->get<TransformComponent>();
#if USE_BOUND_VISUAL_COMPONENT
#else
					for (auto spatialBoundVisual : m_spatialBoundVisualVector) {
						spatialBoundVisual->setScale(modelTransform->scale);
						spatialBoundVisual->setRotate(modelTransform->rotation);
						spatialBoundVisual->setTranslate(modelTransform->translation);
						spatialBoundVisual->setViewProjectionMatrix(viewProjectionMatrix, viewMatrix, invertedViewMatrix);
						m_renderer->draw(spatialBoundVisual, m_visualEffectVector[spatialBoundVisualIndex++ % 2], m_spatialLightVector);
					}
#endif
				}
				m_renderer->unbind(framebuffer, Framebuffer::Target::Draw);
			}*/

			// Blit.
			int width = 0;
			int height = 0;
			m_window->getFramebufferSize(width, height);
			void* texture;
			void* drawable;
			m_window->getMetalHackBoth(&texture, &drawable);
			m_renderer->blit(m_renderPassVector[currentRenderPassIndex]->m_framebufferVector, m_window->m_xDivs, m_window->m_yDivs, width, height, texture, drawable);
		}

		// End Frame.
		m_renderer->endFrame();
	}

	// EventSubscriber

	virtual void receive(World* world, const EventWindowCreated& event) override {
		m_window = (AbstractWindow*)event.window;

		switch (event.type) {
			default:
			case None:
				break;
			case DirectX:
#if defined(_WIN32)
				////m_renderer = new RendererDx(event.xSize, event.ySize);
#endif // defined(_WIN32)
				break;
			case Metal:
#if defined(__APPLE__)
				////m_renderer = new RendererMl(event.xSize, event.ySize);
#endif // defined(__APPLE__)
				break;
			case OpenGL:
				////m_renderer = new RendererGl(event.xSize, event.ySize);
				break;
			case Vulkan:
				////m_renderer = new RendererVk(event.xSize, event.ySize);
				break;
		}

		if (m_renderer) { /*
#if USE_CAMERA_COMPONENT
			m_camera = world->create();

			m_camera->assign<TransformComponent>(TransformComponent{
				.translation = Vec3(0.0f, 0.0f, 500.0f),
				.rotation = Quat::identity,
				.scale = Vec3::one });

			ProjectionParameter projectionParameter(-0.0849426985f,
				+0.0849426985f,
				-0.0637070238f,
				+0.0637070238f,
				+100.0f,
				+0.100f);

			Mat4 projection = makeProjection(m_renderer->getNdcMatrix(),
				Perspective,
				ForwardFinite,
				projectionParameter,
				RUB);

			m_camera->assign<CameraComponent>(CameraComponent{
				.projection = projection });

			m_renderer->setProjectionRestraint(ForwardFinite);
#else
			m_camera = new SpatialCamera(Perspective, m_renderer->getNdcMatrix());
			if (m_camera->getProjectionType() == Orthographic) {
				float s = 100.0f;
				float l = float(event.xSize) / float(event.ySize) / -2.0f * s;
				float r = -l;
				float b = -0.5f * s;
				float t = -b;
#if 0
				Vec3 position = Vec3(-100.0f, +100.0f, +100.0f);
				Quat rotation = Quat(Vec3(0.0f, 1.0f, 0.0f), -M_PI_4) * Quat(Vec3(1.0f, 0.0f, 0.0f), atanf(-1.0f / sqrtf(2.0f)));
				m_camera->setRotate(rotation);
				m_camera->setTranslate(position);
#else
				Vec3 position = Vec3(0.0f, 0.0f, 500.0f);
				m_camera->setTranslate(position);
#endif
				m_camera->setFrustum(l, r, b, t, 1.0f, 1000.0f);
			} else if (m_camera->getProjectionType() == Perspective) {
				m_camera->setFrame(Vec3(+100.0f, +100.0f, +100.0f), Vec3::zero);
				m_camera->setFrustum(65.0f * (M_PI / 180.0f), float(event.xSize) / float(event.ySize), 1.0f, 1000.0f);
			}
			m_renderer->setProjectionRestraint(m_camera->getProjectionDirectionRestraint());
#endif
			// Setup Meshes.
			std::vector<Mesh> objMeshVector;
			std::vector<Material> objMaterialVector;
			objMeshVector.resize(1);
			objMaterialVector.resize(1);
			createCube(&objMeshVector[0]);
			for (int i = 0; i < objMeshVector.size(); ++i) {
				printMesh(&objMeshVector[i], &objMaterialVector[i]);
#if USE_BOUND_VISUAL_COMPONENT
#else
				SpatialBoundVisual3* spatialBoundVisual = m_renderer->createVisualSpatial(hashCombine(std::hash<std::string>{}(filepath), i), objMeshVector[i], objMaterialVector[i]);
				m_spatialBoundVisualVector.push_back(spatialBoundVisual);
#endif
			}

			// Setup Spatial Lights.
			Bound3 bound = Bound3(Vec3::zero, Vec3(100.0f, 100.0f, 100.0f));
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
		*/
		}

		m_window->m_xDivs = 2;
		m_window->m_yDivs = 2;

		Framebuffer::Descriptor descriptors[1];
		descriptors[0].color = { Framebuffer::Format::Color };
		descriptors[0].depth = { Framebuffer::Format::Depth };
		descriptors[0].stencil = {};

		for (int currentRenderPassIndex = 0; currentRenderPassIndex < sizeof(descriptors) / sizeof(Framebuffer::Descriptor); ++currentRenderPassIndex) {
			m_renderPassVector.push_back(new RenderPass());
			int cols = m_window->m_xDivs;
			int rows = m_window->m_yDivs;
			for (int i = 0; i < cols; ++i) {
				for (int j = 0; j < rows; ++j) {
					m_renderPassVector[currentRenderPassIndex]->m_framebufferVector.push_back(m_renderer->createFramebuffer(event.xSize / cols, event.ySize / rows, descriptors[currentRenderPassIndex]));
				}
			}
		}
	}

	virtual void receive(World* world, const EventWindowResized& event) override {
		if (m_renderer) {
			m_renderer->setSize(xSize, ySize);
		}
#if USE_CAMERA_COMPONENT
#else
		if (m_camera) {
			m_camera->setSize(xSize, ySize);
		}
#endif
		for (auto renderPass : m_renderPassVector) {
			std::vector<Framebuffer::Descriptor> descriptors;
			for (auto framebuffer : renderPass->m_framebufferVector) {
				Framebuffer::Descriptor descriptor = framebuffer->m_descriptor;
				descriptors.push_back(descriptor);
				m_renderer->destroyFramebuffer(framebuffer);
			}
			renderPass->m_framebufferVector.clear();
			for (auto descriptor : descriptors) {
				renderPass->m_framebufferVector.push_back(m_renderer->createFramebuffer(xSize / m_window->m_xDivs, ySize / m_window->m_yDivs, descriptor));
			}
		}
	}

	virtual void receive(World* world, const EventWindowDestroyed& event) override {
		for (auto renderPass : m_renderPassVector) {
			for (auto framebuffer : renderPass->m_framebufferVector) {
				m_renderer->destroyFramebuffer(framebuffer);
			}
			renderPass->m_framebufferVector.clear();
			delete renderPass;
		}
		m_renderPassVector.clear();

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
		for (auto spatialBoundVisual : m_spatialBoundVisualVector) {
#if USE_BOUND_VISUAL_COMPONENT
#else
			m_renderer->destroyVisualSpatial(spatialBoundVisual);
#endif
		}
		m_spatialBoundVisualVector.clear();

		if (m_camera) {
#if USE_CAMERA_COMPONENT
			world->destroy(m_camera);
#else
			delete m_camera;
#endif
		}

		if (m_renderer) {
			delete m_renderer;
		}
	}

private:
	int xSize;
	int ySize;

	AbstractWindow* m_window;

	Renderer* m_renderer;

#if USE_CAMERA_COMPONENT
	Entity* m_camera;
#else
	SpatialCamera* m_camera;
#endif

#if USE_LIGHT_COMPONENT
	std::vector<Entity*> m_spatialLightVector;
#else
	std::vector<SpatialLight*> m_spatialLightVector;
#endif

#if USE_BOUND_VISUAL_COMPONENT
	std::vector<Entity*> m_spatialBoundVisualVector;
#else
	std::vector<SpatialBoundVisual3*> m_spatialBoundVisualVector;
#endif

	struct RenderPass {
		std::vector<Framebuffer*> m_framebufferVector;
	};

	std::vector<RenderPass*> m_renderPassVector;
	std::vector<VisualEffect*> m_visualEffectVector;
};
