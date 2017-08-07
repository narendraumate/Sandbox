//
//  SampleWindow.cpp
//
//
//  Created by Narendra Umate on 8/18/15.
//
//

#include "SampleWindow.h"

#include <random>

SampleWindow::SampleWindow(Parameters& parameters)
: BaseWindow(parameters) {
	world = ECS::World::createWorld();

	physicsSystem = new PhysicsSystem();
	world->registerSystem(physicsSystem);
	renderSystem = new RenderSystem();
	world->registerSystem(renderSystem);

	std::default_random_engine generator;
	std::uniform_real_distribution<float> randPosition(-250.0f, 250.0f);
	std::uniform_real_distribution<float> randRotation(0.0f, 2.0f * M_PI);
	std::uniform_real_distribution<float> randScale(1.0f, 10.0f);
	std::uniform_real_distribution<float> randColor(0.0f, 1.0f);

	entityVector.resize(2500);

	for (auto& entity : entityVector) {
		entity = world->create();

		entity->assign<GravityComponent>(GravityComponent{
			.force = Vec3(0.0f, -9.8f, 0.0f) });

		entity->assign<RigidBodyComponent>(RigidBodyComponent{
			.velocity = Vec3(0.0f, 0.0f, 0.0f),
			.acceleration = Vec3(0.0f, 0.0f, 0.0f) });

		entity->assign<TransformComponent>(TransformComponent{
			.translation = Vec3(randPosition(generator), randPosition(generator), randPosition(generator)),
			.rotation = Quat(Vec3(randRotation(generator), randRotation(generator), randRotation(generator)), Y3X2Z1),
			.scale = Vec1::one * randScale(generator) });

		entity->assign<VisualComponent>(VisualComponent{
			.color = Vec3(randColor(generator), randColor(generator), randColor(generator)) });
	}
	world->emit<EventWindowCreated>({ m_type, m_xSize, m_ySize, this });
}

SampleWindow::~SampleWindow() {
	world->emit<EventWindowDestroyed>({});

	for (auto entity : entityVector) {
		world->destroy(entity);
	}

	world->unregisterSystem(renderSystem);
	world->unregisterSystem(physicsSystem);

	world->destroyWorld();
}

void SampleWindow::onResize(const int& xSize, const int& ySize) {
	world->emit<EventWindowResized>({ xSize, ySize });
}

void SampleWindow::onFocus(const int& focus) {
	world->emit<EventWindowResized>({ m_xSize, m_ySize });
}

void SampleWindow::update(float dt) {
	world->tick(dt);
}

void SampleWindow::onKeyDown(int k) {
}
