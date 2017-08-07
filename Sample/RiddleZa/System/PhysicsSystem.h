//
//  PhysicsSystem.h
//
//
//  Created by Narendra Umate on 8/18/16.
//
//

#pragma once

#include "ECS.h"

#include "Component.h"

#include "Event.h"

using namespace ECS;

class PhysicsSystem : public EntitySystem {
public:
	PhysicsSystem() {
	}

	~PhysicsSystem() {
	}

	// EntitySystem

	virtual void configure(World* world) override {
	}

	virtual void unconfigure(World* world) override {
	}

	virtual void tick(World* world, float deltaTime) override {
		for (auto entity : world->each<TransformComponent>()) {
			auto rigidBody = entity->get<RigidBodyComponent>();
			auto transform = entity->get<TransformComponent>();
			auto gravity = entity->get<GravityComponent>();
			if (rigidBody && transform && gravity) {
				auto const& acceleration = rigidBody->acceleration + gravity->force;
				auto const& oldVelocity = rigidBody->velocity;
				auto const& newVelocity = oldVelocity + acceleration * deltaTime;
				auto const& oldPosition = transform->translation;
				auto const& newPosition = oldPosition + (oldVelocity * deltaTime) + (acceleration * deltaTime * deltaTime * 0.5f);
				transform->translation = newPosition;
				rigidBody->velocity = newVelocity;
			}
		}
	}
};
