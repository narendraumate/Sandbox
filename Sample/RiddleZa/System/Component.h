//
//  Component.h
//
//
//  Created by Narendra Umate on 8/18/16.
//
//

#pragma once

#include "Bound.h"
#include "Coeff.h"
#include "Color.h"
#include "Matrix.h"
#include "Vector.h"

#include "ECS.h"

struct CameraComponent {
	ECS_DECLARE_TYPE;
	Mat4 projection;
};

struct GravityComponent {
	ECS_DECLARE_TYPE;
	Vec3 force;
};

struct RigidBodyComponent {
	ECS_DECLARE_TYPE;
	Vec3 velocity;
	Vec3 acceleration;
};

struct TransformComponent {
	ECS_DECLARE_TYPE;
	Vec3 translation;
	Quat rotation;
	float scale;
};

struct BoundComponent {
	ECS_DECLARE_TYPE;
	Bound3 model;
	Bound3 world;
};

struct VisualComponent {
	ECS_DECLARE_TYPE;
	Color3f color;
};

struct LightComponent {
	ECS_DECLARE_TYPE;
	Coeff3f coeff;
	Color3f color;
};
