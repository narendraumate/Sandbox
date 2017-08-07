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
					CHECK(almostEqual(e, Vec3(-0.615479767, 0.785398185, 0)));
				} break;
				case 1: {
					Quat q = lookat(Vec3(-1, -1, +1).normal());
					Vec3 e = q.euler();
					CHECK(almostEqual(e, Vec3(-0.615479767, 2.3561945, 0)));
				} break;
				case 2: {
					Quat q = lookat(Vec3(-1, +1, -1).normal());
					Vec3 e = q.euler();
					CHECK(almostEqual(e, Vec3(0.615479767, 0.785398185, 0)));
				} break;
				case 3: {
					Quat q = lookat(Vec3(-1, +1, +1).normal());
					Vec3 e = q.euler();
					CHECK(almostEqual(e, Vec3(0.615479767, 2.3561945, 0)));
				} break;
				case 4: {
					Quat q = lookat(Vec3(+1, -1, -1).normal());
					Vec3 e = q.euler();
					CHECK(almostEqual(e, Vec3(-0.615479767, -0.785398185, 0)));
				} break;
				case 5: {
					Quat q = lookat(Vec3(+1, -1, +1).normal());
					Vec3 e = q.euler();
					CHECK(almostEqual(e, Vec3(-0.615479767, -2.3561945, 0)));
				} break;
				case 6: {
					Quat q = lookat(Vec3(+1, +1, -1).normal());
					Vec3 e = q.euler();
					CHECK(almostEqual(e, Vec3(0.615479767, -0.785398185, 0)));
				} break;
				default: {
					Quat q = lookat(Vec3(+1, +1, +1).normal());
					Vec3 e = q.euler();
					CHECK(almostEqual(e, Vec3(0.615479767, -2.3561945, 0)));
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

TEST_CASE("Matrix") {
	Mat4 ss1 = Mat4::scale(Vec3(7, 7, 7));
	Mat4 ss2 = Mat4::scale(Vec3(4, 5, 6));
	Mat4 rrr = Mat4::rotate(Quat(Vec3(1, 1, 1).normal(), M_PI_2));
	Mat4 ttt = Mat4::translate(Vec3(1, 2, 3));
	Mat4 xx1 = ttt * rrr * ss1;
	Mat4 xx2 = ttt * rrr * ss2;

	CHECK(almostEqual(xx1, xx1.inverse().inverse(), 0.00001f));
	CHECK(almostEqual(xx2, xx2.inverse().inverse(), 0.00001f));
	CHECK(almostEqual(rrr, rrr.inverse().inverse(), 0.00001f));

	Mat3 rr2 = Mat3(rrr);
	Mat3 ri2 = rr2.inverse();
	ri2 = ri2.inverse();
	CHECK(almostEqual(rr2, ri2, 0.0001f));
}

TEST_CASE("Transform") {
#if 1
	Vec3 oldT(100.0f, 200.0f, 300.0f);
	Quat oldR(Vec3(1, 1, 1).normal(), M_PI_2);
	float oldS = 10.0f;

	Transform3 oldX(oldT, oldR, oldS);
	Vec3 retT = oldX.getTranslate();
	Quat retR = oldX.getRotate();
	float retS = oldX.getScale();

	Vec4 cccc = oldX.getMatrix() * Vec4(1, 2, 3, 1);
	Vec4 dddd = Transform3(retT, retR, retS).getMatrix() * Vec4(1, 2, 3, 1);

	std::cout << cccc << std::endl;
	std::cout << dddd << std::endl;
	std::cout << std::endl;

	CHECK(almostEqual(cccc, dddd, 0.001f));

	oldS = -2.0f;
	Transform3 newX(oldT, oldR, oldS);
	Vec4 aaaa = newX.getMatrix() * Vec4(1, 2, 3, 1);

	newX.getTranslateRotateScale(retT, retR, retS);
	Transform3 wowX(retT, retR, retS);
	Vec4 bbbb = wowX.getMatrix() * Vec4(1, 2, 3, 1);

	std::cout << aaaa << std::endl;
	std::cout << bbbb << std::endl;
	std::cout << std::endl;

	CHECK(almostEqual(aaaa, bbbb, 7.10f));
#else
	Transform3 invX(oldX.getMatrix().inverse());

	// This path only works for uniform scales.
	Transform3 oneX(Mat4::scale(invX.getScale()).inverse() * Mat4::rotate(invX.getRotate()).inverse() * Mat4::translate(invX.getTranslate()).inverse());
	Vec3 oneS = oneX.getScale();
	Quat oneR = oneX.getRotate();
	Vec3 oneT = oneX.getTranslate();
	CHECK(almostEqual(oldS, oneS));
	CHECK(almostEqual(oldR, oneR));
	CHECK(almostEqual(oldT, oneT, 0.0001f));

	// This path works for non uniform as well.
	Transform3 twoX(invX.getMatrix().inverse());
	Vec3 twoS = twoX.getScale();
	Quat twoR = twoX.getRotate();
	Vec3 twoT = twoX.getTranslate();
	CHECK(almostEqual(oldS, twoS, 0.00001f));
	CHECK(almostEqual(oldR, twoR));
	CHECK(almostEqual(oldT, twoT, 0.0001f));
#endif
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

TEST_CASE("Projection") {
	auto getProjection = [](bool forward, bool finite, bool ortho) {
		ProjectionVariation variation = ortho ? Orthographic : Perspective;
		ProjectionChirality chirality = Right;
		ProjectionDimension dimension = X2Y2Z1;
		float near = 0.1f;
		float far = 1000.0f;

		ProjectionParameter projectionParameter;
		switch (variation) {
			case Orthographic: {
				float l = -0.8f;
				float r = +0.8f;
				float b = -0.6f;
				float t = +0.6f;
				projectionParameter = {
					{ l, r, b, t },
					ProjectionParameter::makeExtent(near, far, forward, finite)
				};
			} break;
			case Perspective: {
				float upFovRadian = 2.0f * (M_PI / 180.0f);
				float aspectRatio = 1.0f;
				projectionParameter = {
					ProjectionParameter::makeTangent(upFovRadian, aspectRatio),
					ProjectionParameter::makeExtent(near, far, forward, finite)
				};
			} break;
		}

		return Projection(variation, chirality, dimension, projectionParameter).getMatrix();
	};

	Mat4 m = Mat4::identity;

	m = getProjection(false, false, false); // reverse infinite perspective

	CHECK(almostEqual(m.d[0][0], 57.289959));
	CHECK(almostEqual(m.d[0][1], 0.0f));
	CHECK(almostEqual(m.d[0][2], 0.0f));
	CHECK(almostEqual(m.d[0][3], 0.0f));

	CHECK(almostEqual(m.d[1][0], 0.0f));
	CHECK(almostEqual(m.d[1][1], 57.289959));
	CHECK(almostEqual(m.d[1][2], 0.0f));
	CHECK(almostEqual(m.d[1][3], 0.0f));

	CHECK(almostEqual(m.d[2][0], 0.0f));
	CHECK(almostEqual(m.d[2][1], 0.0f));
	CHECK(almostEqual(m.d[2][2], 0.0f));
	CHECK(almostEqual(m.d[2][3], -1.000000));

	CHECK(almostEqual(m.d[3][0], 0.0f));
	CHECK(almostEqual(m.d[3][1], 0.0f));
	CHECK(almostEqual(m.d[3][2], 0.100000001));
	CHECK(almostEqual(m.d[3][3], 0.0f));

	m = getProjection(false, true, false); // reverse finite perspective

	CHECK(almostEqual(m.d[0][0], 57.289959));
	CHECK(almostEqual(m.d[0][1], 0.0f));
	CHECK(almostEqual(m.d[0][2], 0.0f));
	CHECK(almostEqual(m.d[0][3], 0.0f));

	CHECK(almostEqual(m.d[1][0], 0.0f));
	CHECK(almostEqual(m.d[1][1], 57.289959));
	CHECK(almostEqual(m.d[1][2], 0.0f));
	CHECK(almostEqual(m.d[1][3], 0.0f));

	CHECK(almostEqual(m.d[2][0], 0.0f));
	CHECK(almostEqual(m.d[2][1], 0.0f));
	CHECK(almostEqual(m.d[2][2], 0.000100010002));
	CHECK(almostEqual(m.d[2][3], -1.000000));

	CHECK(almostEqual(m.d[3][0], 0.0f));
	CHECK(almostEqual(m.d[3][1], 0.0f));
	CHECK(almostEqual(m.d[3][2], 0.10001));
	CHECK(almostEqual(m.d[3][3], 0.0f));

	m = getProjection(true, false, false); // forward infinite perspective

	CHECK(almostEqual(m.d[0][0], 57.289959));
	CHECK(almostEqual(m.d[0][1], 0.0f));
	CHECK(almostEqual(m.d[0][2], 0.0f));
	CHECK(almostEqual(m.d[0][3], 0.0f));

	CHECK(almostEqual(m.d[1][0], 0.0f));
	CHECK(almostEqual(m.d[1][1], 57.289959));
	CHECK(almostEqual(m.d[1][2], 0.0f));
	CHECK(almostEqual(m.d[1][3], 0.0f));

	CHECK(almostEqual(m.d[2][0], 0.0f));
	CHECK(almostEqual(m.d[2][1], 0.0f));
	CHECK(almostEqual(m.d[2][2], -1.000000));
	CHECK(almostEqual(m.d[2][3], -1.000000));

	CHECK(almostEqual(m.d[3][0], 0.0f));
	CHECK(almostEqual(m.d[3][1], 0.0f));
	CHECK(almostEqual(m.d[3][2], -0.100000001));
	CHECK(almostEqual(m.d[3][3], 0.0f));

	m = getProjection(true, true, false); // forward finite perspective

	CHECK(almostEqual(m.d[0][0], 57.289959));
	CHECK(almostEqual(m.d[0][1], 0.0f));
	CHECK(almostEqual(m.d[0][2], 0.0f));
	CHECK(almostEqual(m.d[0][3], 0.0f));

	CHECK(almostEqual(m.d[1][0], 0.0f));
	CHECK(almostEqual(m.d[1][1], 57.289959));
	CHECK(almostEqual(m.d[1][2], 0.0f));
	CHECK(almostEqual(m.d[1][3], 0.0f));

	CHECK(almostEqual(m.d[2][0], 0.0f));
	CHECK(almostEqual(m.d[2][1], 0.0f));
	CHECK(almostEqual(m.d[2][2], -1.00010002));
	CHECK(almostEqual(m.d[2][3], -1.000000));

	CHECK(almostEqual(m.d[3][0], 0.0f));
	CHECK(almostEqual(m.d[3][1], 0.0f));
	CHECK(almostEqual(m.d[3][2], -0.10001));
	CHECK(almostEqual(m.d[3][3], 0.0f));

	m = getProjection(false, false, true); // reverse infinite ortho // no good reference
	//..

	m = getProjection(true, false, true); // forward infinite ortho // no good reference
	//..

	m = getProjection(false, true, true); // reverse finite ortho

	CHECK(almostEqual(m.d[0][0], 1.25));
	CHECK(almostEqual(m.d[0][1], 0.0f));
	CHECK(almostEqual(m.d[0][2], 0.0f));
	CHECK(almostEqual(m.d[0][3], 0.0f));

	CHECK(almostEqual(m.d[1][0], 0.0f));
	CHECK(almostEqual(m.d[1][1], 1.66666663));
	CHECK(almostEqual(m.d[1][2], 0.0f));
	CHECK(almostEqual(m.d[1][3], 0.0f));

	CHECK(almostEqual(m.d[2][0], 0.0f));
	CHECK(almostEqual(m.d[2][1], 0.0f));
	CHECK(almostEqual(m.d[2][2], 0.00100009993));
	CHECK(almostEqual(m.d[2][3], 0.0f));

	CHECK(almostEqual(m.d[3][0], 0.0f));
	CHECK(almostEqual(m.d[3][1], 0.0f));
	CHECK(almostEqual(m.d[3][2], 1.00010002));
	CHECK(almostEqual(m.d[3][3], 1.0f));

	m = getProjection(true, true, true); // forward finite ortho

	CHECK(almostEqual(m.d[0][0], 1.25));
	CHECK(almostEqual(m.d[0][1], 0.0f));
	CHECK(almostEqual(m.d[0][2], 0.0f));
	CHECK(almostEqual(m.d[0][3], 0.0f));

	CHECK(almostEqual(m.d[1][0], 0.0f));
	CHECK(almostEqual(m.d[1][1], 1.66666663));
	CHECK(almostEqual(m.d[1][2], 0.0f));
	CHECK(almostEqual(m.d[1][3], 0.0f));

	CHECK(almostEqual(m.d[2][0], 0.0f));
	CHECK(almostEqual(m.d[2][1], 0.0f));
	CHECK(almostEqual(m.d[2][2], -0.00100009993));
	CHECK(almostEqual(m.d[2][3], 0.0f));

	CHECK(almostEqual(m.d[3][0], 0.0f));
	CHECK(almostEqual(m.d[3][1], 0.0f));
	CHECK(almostEqual(m.d[3][2], -0.000100010002));
	CHECK(almostEqual(m.d[3][3], 1.0f));

	m = Mat4::identity;
}
