//
//  Verify.cpp
//
//
//  Created by Narendra Umate on 7/5/17.
//
//

#include "Verify.h"

TEST_CASE("Quaternion") {

	AxisOrderConvention axisOrderConvention[6] = {
		X3Y2Z1,
		Y3Z2X1,
		Z3X2Y1,
		X3Z2Y1,
		Y3X2Z1,
		Z3Y2X1,
	};

	{
		Vec3 newFailX2;
		Vec3 oldFailX2(M_PI / 2, M_PI / 3, M_PI / 4);
		newFailX2 = Vec3(Quat(oldFailX2, X3Y2Z1).euler(X3Y2Z1));
		CHECK(almostEqual(oldFailX2, newFailX2));
		newFailX2 = Vec3(Quat(oldFailX2, Y3Z2X1).euler(Y3Z2X1));
		CHECK(almostEqual(oldFailX2, newFailX2));
		newFailX2 = Vec3(Quat(oldFailX2, Z3X2Y1).euler(Z3X2Y1));
		CHECK(!almostEqual(oldFailX2, newFailX2));
		newFailX2 = Vec3(Quat(oldFailX2, X3Z2Y1).euler(X3Z2Y1));
		CHECK(almostEqual(oldFailX2, newFailX2));
		newFailX2 = Vec3(Quat(oldFailX2, Y3X2Z1).euler(Y3X2Z1));
		CHECK(!almostEqual(oldFailX2, newFailX2));
		newFailX2 = Vec3(Quat(oldFailX2, Z3Y2X1).euler(Z3Y2X1));
		CHECK(almostEqual(oldFailX2, newFailX2));
	}

	{
		Vec3 newFailY2;
		Vec3 oldFailY2(M_PI / 4, M_PI / 2, M_PI / 3);
		newFailY2 = Vec3(Quat(oldFailY2, X3Y2Z1).euler(X3Y2Z1));
		CHECK(!almostEqual(oldFailY2, newFailY2));
		newFailY2 = Vec3(Quat(oldFailY2, Y3Z2X1).euler(Y3Z2X1));
		CHECK(almostEqual(oldFailY2, newFailY2));
		newFailY2 = Vec3(Quat(oldFailY2, Z3X2Y1).euler(Z3X2Y1));
		CHECK(almostEqual(oldFailY2, newFailY2));
		newFailY2 = Vec3(Quat(oldFailY2, X3Z2Y1).euler(X3Z2Y1));
		CHECK(almostEqual(oldFailY2, newFailY2));
		newFailY2 = Vec3(Quat(oldFailY2, Y3X2Z1).euler(Y3X2Z1));
		CHECK(almostEqual(oldFailY2, newFailY2));
		newFailY2 = Vec3(Quat(oldFailY2, Z3Y2X1).euler(Z3Y2X1));
		CHECK(!almostEqual(oldFailY2, newFailY2));
	}

	{
		Vec3 newFailZ2;
		Vec3 oldFailZ2(M_PI / 3, M_PI / 4, M_PI / 2);
		newFailZ2 = Vec3(Quat(oldFailZ2, X3Y2Z1).euler(X3Y2Z1));
		CHECK(almostEqual(oldFailZ2, newFailZ2));
		newFailZ2 = Vec3(Quat(oldFailZ2, Y3Z2X1).euler(Y3Z2X1));
		CHECK(!almostEqual(oldFailZ2, newFailZ2));
		newFailZ2 = Vec3(Quat(oldFailZ2, Z3X2Y1).euler(Z3X2Y1));
		CHECK(almostEqual(oldFailZ2, newFailZ2));
		newFailZ2 = Vec3(Quat(oldFailZ2, X3Z2Y1).euler(X3Z2Y1));
		CHECK(!almostEqual(oldFailZ2, newFailZ2));
		newFailZ2 = Vec3(Quat(oldFailZ2, Y3X2Z1).euler(Y3X2Z1));
		CHECK(almostEqual(oldFailZ2, newFailZ2));
		newFailZ2 = Vec3(Quat(oldFailZ2, Z3Y2X1).euler(Z3Y2X1));
		CHECK(almostEqual(oldFailZ2, newFailZ2));
	}

	{
		for (int i = 0; i < 8; ++i) {
			switch (i) {
				case 0: {
					Quat q = lookat(Vec3(-1, -1, -1).normal());
					Vec3 e = q.euler();
					CHECK(almostEqual(e, Vec3(0.615479767, -2.3561945, 0)));
				} break;
				case 1: {
					Quat q = lookat(Vec3(-1, -1, +1).normal());
					Vec3 e = q.euler();
					CHECK(almostEqual(e, Vec3(0.615479767, -0.785398185, 0)));
				} break;
				case 2: {
					Quat q = lookat(Vec3(-1, +1, -1).normal());
					Vec3 e = q.euler();
					CHECK(almostEqual(e, Vec3(-0.615479767, -2.3561945, 0)));
				} break;
				case 3: {
					Quat q = lookat(Vec3(-1, +1, +1).normal());
					Vec3 e = q.euler();
					CHECK(almostEqual(e, Vec3(-0.615479767, -0.785398185, 0)));
				} break;
				case 4: {
					Quat q = lookat(Vec3(+1, -1, -1).normal());
					Vec3 e = q.euler();
					CHECK(almostEqual(e, Vec3(0.615479767, 2.3561945, 0)));
				} break;
				case 5: {
					Quat q = lookat(Vec3(+1, -1, +1).normal());
					Vec3 e = q.euler();
					CHECK(almostEqual(e, Vec3(0.615479767, 0.785398185, 0)));
				} break;
				case 6: {
					Quat q = lookat(Vec3(+1, +1, -1).normal());
					Vec3 e = q.euler();
					CHECK(almostEqual(e, Vec3(-0.615479767, 2.3561945, 0)));
				} break;
				default: {
					Quat q = lookat(Vec3(+1, +1, +1).normal());
					Vec3 e = q.euler();
					CHECK(almostEqual(e, Vec3(-0.615479767, 0.785398185, 0)));
				} break;
			}
		}
	}

	{
		for (int i = -360; i < 360; i = i + 72) {
			for (int j = -360; j < 360; j = j + 72) {
				for (int k = -360; k < 360; k = k + 72) {
					float x = i * M_PI / 180.0f;
					float y = j * M_PI / 180.0f;
					float z = k * M_PI / 180.0f;
					Quat xRotationQuat(Quat(Vec3(1.0f, 0.0f, 0.0f), x));
					Quat yRotationQuat(Quat(Vec3(0.0f, 1.0f, 0.0f), y));
					Quat zRotationQuat(Quat(Vec3(0.0f, 0.0f, 1.0f), z));
					for (int index = 0; index < 6; ++index) {
						Quat fRotationQuat = Quat::identity;
						switch (axisOrderConvention[index]) {
							case X3Y2Z1: {
								fRotationQuat = xRotationQuat * yRotationQuat * zRotationQuat;
							} break;
							case Y3Z2X1: {
								fRotationQuat = yRotationQuat * zRotationQuat * xRotationQuat;
							} break;
							case Z3X2Y1: {
								fRotationQuat = zRotationQuat * xRotationQuat * yRotationQuat;
							} break;
							case X3Z2Y1: {
								fRotationQuat = xRotationQuat * zRotationQuat * yRotationQuat;
							} break;
							case Y3X2Z1: {
								fRotationQuat = yRotationQuat * xRotationQuat * zRotationQuat;
							} break;
							case Z3Y2X1: {
								fRotationQuat = zRotationQuat * yRotationQuat * xRotationQuat;
							} break;
						}
						CHECK(almostEqual(Quat(Vec3(x, y, z), axisOrderConvention[index]), fRotationQuat));
					}
				}
			}
		}
	}
}

TEST_CASE("Transform") {
	Vec3 oldS(10.0f, 20.0f, 30.0f);
	Quat oldR(Vec3(1, 1, 1).normal(), M_PI_2);
	Vec3 oldT(100.0f, 200.0f, 300.0f);
	Transform3 oldX(oldT, oldR, oldS);
	Vec3 newS = oldX.getScale();
	CHECK(almostEqual(oldS, newS));
	Quat newR = oldX.getRotate();
	CHECK(almostEqual(oldR, newR));
	Vec3 newT = oldX.getTranslate();
	CHECK(almostEqual(oldT, newT));
}

TEST_CASE("Vector") {
	Vec3 r(1.0f, 0.0f, 0.0f);
	Vec3 u(0.0f, 1.0f, 0.0f);
	Vec3 b(0.0f, 0.0f, 1.0f);
	CHECK(almostEqual(cross(r, u), b));
	CHECK(almostEqual(cross(u, b), r));
	CHECK(almostEqual(cross(b, r), u));
	CHECK(almostEqual(scalarTripleProduct(r, u, b), 1.0f));
	CHECK(almostEqual(scalarTripleProduct(u, b, r), 1.0f));
	CHECK(almostEqual(scalarTripleProduct(b, r, u), 1.0f));
	Vec3 d(1.0f, 2.0f, 3.0f);
	Vec3 e(4.0f, 5.0f, 6.0f);
	Vec3 f(7.0f, 8.0f, 9.0f);
	CHECK(almostEqual(vectorTripleProduct(d, e, f), Vec3(-24.0f, -6.0f, 12.0f)));
}
