/*
	Copyright (C) 2005-2007 Feeling Software Inc.
	Portions of the code are:
	Copyright (C) 2005-2007 Sony Computer Entertainment America
	
	MIT License: http://www.opensource.org/licenses/mit-license.php
*/
#include "Matrix44.h"
#include <limits>
#ifndef D3DX_PI
#define D3DX_PI    (3.14159265358979323846f)
#endif
#ifndef D3DXToRadian
#define D3DXToRadian(degree) ((degree) * (D3DX_PI / 180.0f))
#endif

static float g_Matrix44IdentityFloat[] = { 1, 0, 0, 0, 0, 1, 0 ,0 ,0, 0, 1, 0, 0, 0, 0, 1 };
static float __ZupToYUp[] = { 1, 0, 0, 0,
							  0, 0, -1, 0,
							  0, 1, 0, 0,
							  0, 0, 0, 1 };

static float __YupToYDown[] = {  1,  0,  0, 0,
								 0,  0,  -1, 0,
								 0,  1,  0, 0,
								 0,  0,  0, 1 };

//static float X_TO_Z[] = {		0, 1, 0, 0,
//								1, 0, 0, 0,
//								0, 0, 1, 0,
//								0, 0, 0, 1 };

//static float Y_TO_Z[] = {		1, 0, 0, 0,
//								0, 0, 1, 0,
//								0, 1, 0, 0,
//								0, 0, 0, 1 };

cMatrix44 cMatrix44::ZupToYUp(__ZupToYUp);
cMatrix44 cMatrix44::YupToYDown(__YupToYDown);
cMatrix44 cMatrix44::Identity(g_Matrix44IdentityFloat);

cMatrix44 cMatrix44::X_TO_Z(__ZupToYUp);
cMatrix44 cMatrix44::Y_TO_Z(__ZupToYUp);

cMatrix44::cMatrix44(const float* _m)
{
	Set(_m);
}

cMatrix44::cMatrix44(const double* _m)
{
	Set(_m);
}

cMatrix44& cMatrix44::operator=(const cMatrix44& copy)
{
	m[0][0] = copy.m[0][0]; m[0][1] = copy.m[0][1]; m[0][2] = copy.m[0][2]; m[0][3] = copy.m[0][3];
	m[1][0] = copy.m[1][0]; m[1][1] = copy.m[1][1]; m[1][2] = copy.m[1][2]; m[1][3] = copy.m[1][3];
	m[2][0] = copy.m[2][0]; m[2][1] = copy.m[2][1]; m[2][2] = copy.m[2][2]; m[2][3] = copy.m[2][3];
	m[3][0] = copy.m[3][0]; m[3][1] = copy.m[3][1]; m[3][2] = copy.m[3][2]; m[3][3] = copy.m[3][3];
	return *this;
}

void cMatrix44::Set(const float* _m)
{
	m[0][0] = _m[0]; m[1][0] = _m[4]; m[2][0] = _m[8]; m[3][0] = _m[12];
	m[0][1] = _m[1]; m[1][1] = _m[5]; m[2][1] = _m[9]; m[3][1] = _m[13];
	m[0][2] = _m[2]; m[1][2] = _m[6]; m[2][2] = _m[10]; m[3][2] = _m[14];
	m[0][3] = _m[3]; m[1][3] = _m[7]; m[2][3] = _m[11]; m[3][3] = _m[15];
}

void cMatrix44::Set(const double* _m)
{
	m[0][0] = (float)_m[0]; m[1][0] = (float)_m[4]; m[2][0] = (float)_m[8]; m[3][0] = (float)_m[12];
	m[0][1] = (float)_m[1]; m[1][1] = (float)_m[5]; m[2][1] = (float)_m[9]; m[3][1] = (float)_m[13];
	m[0][2] = (float)_m[2]; m[1][2] = (float)_m[6]; m[2][2] = (float)_m[10]; m[3][2] = (float)_m[14];
	m[0][3] = (float)_m[3]; m[1][3] = (float)_m[7]; m[2][3] = (float)_m[11]; m[3][3] = (float)_m[15];
}

// Returns the transpose of this matrix
cMatrix44 cMatrix44::Transposed() const
{
	cMatrix44 mx;
	mx.m[0][0] = m[0][0]; mx.m[1][0] = m[0][1]; mx.m[2][0] = m[0][2]; mx.m[3][0] = m[0][3];
	mx.m[0][1] = m[1][0]; mx.m[1][1] = m[1][1]; mx.m[2][1] = m[1][2]; mx.m[3][1] = m[1][3];
	mx.m[0][2] = m[2][0]; mx.m[1][2] = m[2][1]; mx.m[2][2] = m[2][2]; mx.m[3][2] = m[2][3];
	mx.m[0][3] = m[3][0]; mx.m[1][3] = m[3][1]; mx.m[2][3] = m[3][2]; mx.m[3][3] = m[3][3];
	return mx;
}

Vector3 cMatrix44::TransformCoordinate(const Vector3& coordinate) const
{
	return Vector3(
		m[0][0] * coordinate.x + m[1][0] * coordinate.y + m[2][0] * coordinate.z + m[3][0],
		m[0][1] * coordinate.x + m[1][1] * coordinate.y + m[2][1] * coordinate.z + m[3][1],
		m[0][2] * coordinate.x + m[1][2] * coordinate.y + m[2][2] * coordinate.z + m[3][2]
	);
}

Vector4 cMatrix44::TransformCoordinate(const Vector4& coordinate) const
{
	return Vector4(
		m[0][0] * coordinate.x + m[1][0] * coordinate.y + m[2][0] * coordinate.z + m[3][0] * coordinate.w,
		m[0][1] * coordinate.x + m[1][1] * coordinate.y + m[2][1] * coordinate.z + m[3][1] * coordinate.w,
		m[0][2] * coordinate.x + m[1][2] * coordinate.y + m[2][2] * coordinate.z + m[3][2] * coordinate.w,
		m[0][3] * coordinate.x + m[1][3] * coordinate.y + m[2][3] * coordinate.z + m[3][3] * coordinate.w
	);
}
Vector2 cMatrix44::TransformVector(const Vector2& v) const
{
	Vector3	l_vPos(v.x,v.y,0.f);
	l_vPos = TransformVector(l_vPos);
	return Vector2(l_vPos.x,l_vPos.y);
}
Vector4 cMatrix44::TransformVector(const Vector4& v) const
{
	Vector3	l_vPos(v.x,v.y,v.z);
	l_vPos = TransformVector(l_vPos);
	return Vector4(l_vPos.x,l_vPos.y,l_vPos.z,v.w);
}
Vector3 cMatrix44::TransformVector(const Vector3& v) const
{
	return Vector3(
		m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z,
		m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z,
		m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z
	);
}
/*
void cMatrix44::SetTranslation(const Vector3& translation)
{
	m[3][0] = translation.x;
	m[3][1] = translation.y;
	m[3][2] = translation.z;
}
*/
static float det3x3(float a1, float a2, float a3, float b1, float b2, float b3, float c1, float c2, float c3);

void cMatrix44::Decompose(Vector3& scale, Vector3& rotation, Vector3& translation, float& inverted) const
{
	// translation * rotation [x*y*z order] * scale = original matrix
	// if inverted is true, then negate scale and the above formula will be correct
	scale.x = sqrtf(m[0][0] * m[0][0] + m[0][1] * m[0][1] + m[0][2] * m[0][2]);
	scale.y = sqrtf(m[1][0] * m[1][0] + m[1][1] * m[1][1] + m[1][2] * m[1][2]);
	scale.z = sqrtf(m[2][0] * m[2][0] + m[2][1] * m[2][1] + m[2][2] * m[2][2]);

	Vector3 savedScale(scale);
	if (IsEquivalent(scale.x, 0.0f)) { scale.x = EPSIONAL; }
	if (IsEquivalent(scale.y, 0.0f)) { scale.y = EPSIONAL; }
	if (IsEquivalent(scale.z, 0.0f)) { scale.z = EPSIONAL; }

	inverted = MyMath::Sign(det3x3(m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2]));

	if (inverted < 0.0f)
	{
		// intentionally do not want to do this on the savedScale
		scale.x = -scale.x;
		scale.y = -scale.y;
		scale.z = -scale.z;
	}

	// Calculate the rotation in Y first, using m[0][2], checking for out-of-bounds values
	float c;
	if (m[2][0] / scale.z >= 1.0f - EPSIONAL)
	{
		rotation.y = ((float) MyMath::Pi) / 2.0f;
		c = 0.0f;
	}
	else if (m[2][0] / scale.z <= -1.0f + EPSIONAL)
	{
		rotation.y = ((float) MyMath::Pi) / -2.0f;
		c = 0.0f;
	}
	else
	{
		rotation.y = asinf(m[2][0] / scale.z);
		c = cosf(rotation.y);
	}

	// Using the cosine of the Y rotation will give us the rotation in X and Z.
	// Check for the infamous Gimbal Lock.
	if (fabsf(c) > 0.01f) 
	{
		float rx = m[2][2] / scale.z / c;
		float ry = -m[2][1] / scale.z / c;
		rotation.x = atan2f(ry, rx);
		rx = m[0][0] / scale.x / c;
		ry = -m[1][0] / scale.y / c;
		rotation.z = atan2f(ry, rx);
	}
	else
	{
		rotation.z = 0;
		float rx = m[1][1] / scale.y;
		float ry = m[1][2] / scale.y;
		rotation.x = atan2f(ry, rx);
	}

	translation = GetTranslation();
	scale = savedScale;
}

void cMatrix44::Recompose(const Vector3& scale, const Vector3& rotation, const Vector3& translation, float inverted)
{
	(*this) = cMatrix44::TranslationMatrix(translation) * cMatrix44::AxisRotationMatrix(Vector3::ZAxis, rotation.z)
		* cMatrix44::AxisRotationMatrix(Vector3::YAxis, rotation.y) * cMatrix44::AxisRotationMatrix(Vector3::XAxis, rotation.x)
		* cMatrix44::ScaleMatrix(inverted * scale);
}

// Code taken and adapted from nVidia's nv_algebra: det2x2, det3x3, invert, multiply
// -----
// Calculate the determinant of a 2x2 matrix
static float det2x2(float a1, float a2, float b1, float b2)
{
    return a1 * b2 - b1 * a2;
}

// Calculate the determinent of a 3x3 matrix
static float det3x3(float a1, float a2, float a3, float b1, float b2, float b3, float c1, float c2, float c3)
{
    return a1 * det2x2(b2, b3, c2, c3) - b1 * det2x2(a2, a3, c2, c3) + c1 * det2x2(a2, a3, b2, b3);
}

// Returns the inverse of this matrix
cMatrix44 cMatrix44::Inverted() const
{
	cMatrix44 b;

	b.m[0][0] =  det3x3(m[1][1], m[1][2], m[1][3], m[2][1], m[2][2], m[2][3], m[3][1], m[3][2], m[3][3]);
	b.m[0][1] = -det3x3(m[0][1], m[0][2], m[0][3], m[2][1], m[2][2], m[2][3], m[3][1], m[3][2], m[3][3]);
	b.m[0][2] =  det3x3(m[0][1], m[0][2], m[0][3], m[1][1], m[1][2], m[1][3], m[3][1], m[3][2], m[3][3]);
	b.m[0][3] = -det3x3(m[0][1], m[0][2], m[0][3], m[1][1], m[1][2], m[1][3], m[2][1], m[2][2], m[2][3]);

	b.m[1][0] = -det3x3(m[1][0], m[1][2], m[1][3], m[2][0], m[2][2], m[2][3], m[3][0], m[3][2], m[3][3]);
	b.m[1][1] =  det3x3(m[0][0], m[0][2], m[0][3], m[2][0], m[2][2], m[2][3], m[3][0], m[3][2], m[3][3]);
	b.m[1][2] = -det3x3(m[0][0], m[0][2], m[0][3], m[1][0], m[1][2], m[1][3], m[3][0], m[3][2], m[3][3]);
	b.m[1][3] =  det3x3(m[0][0], m[0][2], m[0][3], m[1][0], m[1][2], m[1][3], m[2][0], m[2][2], m[2][3]);

	b.m[2][0] =  det3x3(m[1][0], m[1][1], m[1][3], m[2][0], m[2][1], m[2][3], m[3][0], m[3][1], m[3][3]);
	b.m[2][1] = -det3x3(m[0][0], m[0][1], m[0][3], m[2][0], m[2][1], m[2][3], m[3][0], m[3][1], m[3][3]);
	b.m[2][2] =  det3x3(m[0][0], m[0][1], m[0][3], m[1][0], m[1][1], m[1][3], m[3][0], m[3][1], m[3][3]);
	b.m[2][3] = -det3x3(m[0][0], m[0][1], m[0][3], m[1][0], m[1][1], m[1][3], m[2][0], m[2][1], m[2][3]);

	b.m[3][0] = -det3x3(m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2], m[3][0], m[3][1], m[3][2]);
	b.m[3][1] =  det3x3(m[0][0], m[0][1], m[0][2], m[2][0], m[2][1], m[2][2], m[3][0], m[3][1], m[3][2]);
	b.m[3][2] = -det3x3(m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[3][0], m[3][1], m[3][2]);
	b.m[3][3] =  det3x3(m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2]);

	double det = (m[0][0] * b.m[0][0]) + (m[1][0] * b.m[0][1]) + (m[2][0] * b.m[0][2]) + (m[3][0] * b.m[0][3]);

	double epsilon = std::numeric_limits<double>::epsilon();
	if (det + epsilon >= 0.0f && det - epsilon <= 0.0f) det = MyMath::Sign(det) * 0.0001f;
	float oodet = (float) (1.0 / det);

	b.m[0][0] *= oodet;
	b.m[0][1] *= oodet;
	b.m[0][2] *= oodet;
	b.m[0][3] *= oodet;

	b.m[1][0] *= oodet;
	b.m[1][1] *= oodet;
	b.m[1][2] *= oodet;
	b.m[1][3] *= oodet;

	b.m[2][0] *= oodet;
	b.m[2][1] *= oodet;
	b.m[2][2] *= oodet;
	b.m[2][3] *= oodet;

	b.m[3][0] *= oodet;
	b.m[3][1] *= oodet;
	b.m[3][2] *= oodet;
	b.m[3][3] *= oodet;

	return b;
}

float cMatrix44::Determinant() const
{
	float cofactor0 = det3x3(m[1][1], m[1][2], m[1][3], m[2][1], m[2][2], 
							 m[2][3], m[3][1], m[3][2], m[3][3]);
	float cofactor1 = -det3x3(m[0][1], m[0][2], m[0][3], m[2][1], m[2][2], 
							  m[2][3], m[3][1], m[3][2], m[3][3]);
	float cofactor2 = det3x3(m[0][1], m[0][2], m[0][3], m[1][1], m[1][2], 
							 m[1][3], m[3][1], m[3][2], m[3][3]);
	float cofactor3 = -det3x3(m[0][1], m[0][2], m[0][3], m[1][1], m[1][2], 
							  m[1][3], m[2][1], m[2][2], m[2][3]);
	return (m[0][0] * cofactor0) + (m[1][0] * cofactor1) + 
           (m[2][0] * cofactor2) + (m[3][0] * cofactor3);
}

cMatrix44 operator*(const cMatrix44& m1, const cMatrix44& m2)
{
    cMatrix44 mx;
    mx.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[1][0] * m2.m[0][1] + m1.m[2][0] * m2.m[0][2] + m1.m[3][0] * m2.m[0][3];
    mx.m[0][1] = m1.m[0][1] * m2.m[0][0] + m1.m[1][1] * m2.m[0][1] + m1.m[2][1] * m2.m[0][2] + m1.m[3][1] * m2.m[0][3];
    mx.m[0][2] = m1.m[0][2] * m2.m[0][0] + m1.m[1][2] * m2.m[0][1] + m1.m[2][2] * m2.m[0][2] + m1.m[3][2] * m2.m[0][3];
    mx.m[0][3] = m1.m[0][3] * m2.m[0][0] + m1.m[1][3] * m2.m[0][1] + m1.m[2][3] * m2.m[0][2] + m1.m[3][3] * m2.m[0][3];
    mx.m[1][0] = m1.m[0][0] * m2.m[1][0] + m1.m[1][0] * m2.m[1][1] + m1.m[2][0] * m2.m[1][2] + m1.m[3][0] * m2.m[1][3];
    mx.m[1][1] = m1.m[0][1] * m2.m[1][0] + m1.m[1][1] * m2.m[1][1] + m1.m[2][1] * m2.m[1][2] + m1.m[3][1] * m2.m[1][3];
    mx.m[1][2] = m1.m[0][2] * m2.m[1][0] + m1.m[1][2] * m2.m[1][1] + m1.m[2][2] * m2.m[1][2] + m1.m[3][2] * m2.m[1][3];
    mx.m[1][3] = m1.m[0][3] * m2.m[1][0] + m1.m[1][3] * m2.m[1][1] + m1.m[2][3] * m2.m[1][2] + m1.m[3][3] * m2.m[1][3];
    mx.m[2][0] = m1.m[0][0] * m2.m[2][0] + m1.m[1][0] * m2.m[2][1] + m1.m[2][0] * m2.m[2][2] + m1.m[3][0] * m2.m[2][3];
    mx.m[2][1] = m1.m[0][1] * m2.m[2][0] + m1.m[1][1] * m2.m[2][1] + m1.m[2][1] * m2.m[2][2] + m1.m[3][1] * m2.m[2][3];
    mx.m[2][2] = m1.m[0][2] * m2.m[2][0] + m1.m[1][2] * m2.m[2][1] + m1.m[2][2] * m2.m[2][2] + m1.m[3][2] * m2.m[2][3];
    mx.m[2][3] = m1.m[0][3] * m2.m[2][0] + m1.m[1][3] * m2.m[2][1] + m1.m[2][3] * m2.m[2][2] + m1.m[3][3] * m2.m[2][3];
    mx.m[3][0] = m1.m[0][0] * m2.m[3][0] + m1.m[1][0] * m2.m[3][1] + m1.m[2][0] * m2.m[3][2] + m1.m[3][0] * m2.m[3][3];
    mx.m[3][1] = m1.m[0][1] * m2.m[3][0] + m1.m[1][1] * m2.m[3][1] + m1.m[2][1] * m2.m[3][2] + m1.m[3][1] * m2.m[3][3];
    mx.m[3][2] = m1.m[0][2] * m2.m[3][0] + m1.m[1][2] * m2.m[3][1] + m1.m[2][2] * m2.m[3][2] + m1.m[3][2] * m2.m[3][3];
    mx.m[3][3] = m1.m[0][3] * m2.m[3][0] + m1.m[1][3] * m2.m[3][1] + m1.m[2][3] * m2.m[3][2] + m1.m[3][3] * m2.m[3][3];
    return mx;
}

Vector3 operator*(const cMatrix44& m, const Vector3& v)
{
	return Vector3(
		m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z,
		m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z,
		m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z
	);
}

Vector3  operator*(const Vector3& v  ,const cMatrix44& m)
{
	return Vector3(
		m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z,
		m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z,
		m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z
	);
}

Vector4 operator*(const cMatrix44& m, const Vector4& v)
{
	float x = m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0] * v.w;
	float y = m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1] * v.w;
	float z = m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2] * v.w;
	float w = m[0][3] * v.x + m[1][3] * v.y + m[2][3] * v.z + m[3][3] * v.w;
	return Vector4(x, y, z, w);
}

cMatrix44 operator*(float a, const cMatrix44& m1)
{
	cMatrix44 mx;
    mx.m[0][0] = a * m1.m[0][0];
    mx.m[0][1] = a * m1.m[0][1];
    mx.m[0][2] = a * m1.m[0][2];
    mx.m[0][3] = a * m1.m[0][3];
    mx.m[1][0] = a * m1.m[1][0];
    mx.m[1][1] = a * m1.m[1][1];
    mx.m[1][2] = a * m1.m[1][2];
    mx.m[1][3] = a * m1.m[1][3];
    mx.m[2][0] = a * m1.m[2][0];
    mx.m[2][1] = a * m1.m[2][1];
    mx.m[2][2] = a * m1.m[2][2];
    mx.m[2][3] = a * m1.m[2][3];
    mx.m[3][0] = a * m1.m[3][0];
    mx.m[3][1] = a * m1.m[3][1];
    mx.m[3][2] = a * m1.m[3][2];
    mx.m[3][3] = a * m1.m[3][3];
    return mx;
}

cMatrix44 cMatrix44::TranslationMatrix(float e_fX,float e_fY,float e_fZ)
{
	cMatrix44 matrix;
	matrix[0][0] = 1.0f; matrix[0][1] = 0.0f; matrix[0][2] = 0.0f; matrix[0][3] = 0.0f;
	matrix[1][0] = 0.0f; matrix[1][1] = 1.0f; matrix[1][2] = 0.0f; matrix[1][3] = 0.0f;
	matrix[2][0] = 0.0f; matrix[2][1] = 0.0f; matrix[2][2] = 1.0f; matrix[2][3] = 0.0f;
	matrix[3][0] = e_fX; matrix[3][1] = e_fY; matrix[3][2] = e_fZ; matrix[3][3] = 1.0f;
	return matrix;
}

cMatrix44 cMatrix44::TranslationMatrix(const Vector3& translation)
{
	cMatrix44 matrix;
	matrix[0][0] = 1.0f; matrix[0][1] = 0.0f; matrix[0][2] = 0.0f; matrix[0][3] = 0.0f;
	matrix[1][0] = 0.0f; matrix[1][1] = 1.0f; matrix[1][2] = 0.0f; matrix[1][3] = 0.0f;
	matrix[2][0] = 0.0f; matrix[2][1] = 0.0f; matrix[2][2] = 1.0f; matrix[2][3] = 0.0f;
	matrix[3][0] = translation.x; matrix[3][1] = translation.y; matrix[3][2] = translation.z; matrix[3][3] = 1.0f;
	return matrix;
}

cMatrix44 cMatrix44::TranslationMatrix(const VECTOR4& translation)
{
	cMatrix44 matrix;
	matrix[0][0] = 1.0f; matrix[0][1] = 0.0f; matrix[0][2] = 0.0f; matrix[0][3] = 0.0f;
	matrix[1][0] = 0.0f; matrix[1][1] = 1.0f; matrix[1][2] = 0.0f; matrix[1][3] = 0.0f;
	matrix[2][0] = 0.0f; matrix[2][1] = 0.0f; matrix[2][2] = 1.0f; matrix[2][3] = 0.0f;
	matrix[3][0] = translation.x; matrix[3][1] = translation.y; matrix[3][2] = translation.z; matrix[3][3] = 1.0f;
	return matrix;
}

cMatrix44 cMatrix44::AxisRotationMatrix(const Vector3& axis, float angle)
{
	// Formulae inspired from http://www.mines.edu/~gmurray/ArbitraryAxisRotation/ArbitraryAxisRotation.html
	cMatrix44 matrix;
	Vector3 a = (IsEquivalent(axis.LengthSquared(), 1.0f)) ? axis : axis.Normalize();
	float xSq = a.x * a.x;
	float ySq = a.y * a.y;
	float zSq = a.z * a.z;
	float cT = cosf(angle);
	float sT = sinf(angle);

	matrix[0][0] = xSq + (ySq + zSq) * cT;
	matrix[0][1] = a.x * a.y * (1.0f - cT) + a.z * sT;
	matrix[0][2] = a.x * a.z * (1.0f - cT) - a.y * sT;
	matrix[0][3] = 0;
	matrix[1][0] = a.x * a.y * (1.0f - cT) - a.z * sT;
	matrix[1][1] = ySq + (xSq + zSq) * cT;
	matrix[1][2] = a.y * a.z * (1.0f - cT) + a.x * sT;
	matrix[1][3] = 0;
	matrix[2][0] = a.x * a.z * (1.0f - cT) + a.y * sT;
	matrix[2][1] = a.y * a.z * (1.0f - cT) - a.x * sT;
	matrix[2][2] = zSq + (xSq + ySq) * cT;
	matrix[2][3] = 0;
	matrix[3][2] = matrix[3][1] = matrix[3][0] = 0;
	matrix[3][3] = 1;
	return matrix;
}

cMatrix44 cMatrix44::XAxisRotationMatrix(float angle)
{
	cMatrix44 ret = cMatrix44::Identity;
	ret[1][1] = ret[2][2] = cos(angle);
	ret[2][1] = -(ret[1][2] = sin(angle));
	return ret;
}

cMatrix44 cMatrix44::YAxisRotationMatrix(float angle)
{
	cMatrix44 ret = cMatrix44::Identity;
	ret[0][0] = ret[2][2] = cos(angle);
	ret[0][2] = -(ret[2][0] = sin(angle));
	return ret;
}

cMatrix44 cMatrix44::ZAxisRotationMatrix(float angle)
{
	cMatrix44 ret = cMatrix44::Identity;
	ret[0][0] = ret[1][1] = cos(angle);
	ret[1][0] = -(ret[0][1] = sin(angle));
	return ret;
}

cMatrix44 cMatrix44::RotationMatrix(Vector3 e_vAngle)
{
	//cMatrix44	l_mat = cMatrix44::Identity;
	//if( e_vAngle.x != 0.f )
	//	l_mat = XAxisRotationMatrix(D3DXToRadian(e_vAngle.x));
	//if( e_vAngle.y != 0.f )
	//	l_mat *= YAxisRotationMatrix(D3DXToRadian(e_vAngle.y));
	//if( e_vAngle.z != 0.f )
	//	l_mat *= ZAxisRotationMatrix(D3DXToRadian(e_vAngle.z));
	//return l_mat;

	float cx = cos(D3DXToRadian(e_vAngle.x));
	float sx = sin(D3DXToRadian(e_vAngle.x));
	float cy = cos(D3DXToRadian(e_vAngle.y));
	float sy = sin(D3DXToRadian(e_vAngle.y));
	float cz = cos(D3DXToRadian(e_vAngle.z));
	float sz = sin(D3DXToRadian(e_vAngle.z));
	float l_fMatrixResult[] = 
	{ 
		cy * cz		,cx * sz + sx * sy * cz	,sx * sz - cx * sy * cz	, 0.f,
		-cy * sz	,cx * cz - sx * sy * sz	,sx * cz + cx * sy * sz	, 0.f,
		sy			,-sx * cy				,cx * cy				, 0.f,
		1.f			,1.f					,1.f					, 1.f 
	};
	return cMatrix44(l_fMatrixResult);
}

cMatrix44 cMatrix44::EulerRotationMatrix(const Vector3& rotation)
{
	cMatrix44 transform;
	if (!IsEquivalent(rotation.x, 0.0f)) transform = XAxisRotationMatrix(rotation.x);
	else transform = cMatrix44::Identity;
	if (!IsEquivalent(rotation.y, 0.0f)) transform *= YAxisRotationMatrix(rotation.y);
	if (!IsEquivalent(rotation.z, 0.0f)) transform *= ZAxisRotationMatrix(rotation.z);
	return transform;
}

cMatrix44 cMatrix44::ScaleMatrix(const Vector3& scale)
{
	cMatrix44 mx(Identity);
	mx[0][0] = scale.x; mx[1][1] = scale.y; mx[2][2] = scale.z;
	return mx;
}

cMatrix44 cMatrix44::LookAtMatrix(const Vector3& eye, const Vector3& target, const Vector3& up)
{
	cMatrix44 mx;

	Vector3 forward = (target - eye).Normalize();
	Vector3 sideways, upward;
	if (!IsEquivalent(forward, up) && !IsEquivalent(forward, -up))
	{
		sideways = (forward ^ up).Normalize();
	}
	else if (!IsEquivalent(up, Vector3::XAxis))
	{
		sideways = Vector3::XAxis;
	}
	else
	{
		sideways = Vector3::ZAxis;
	}
	upward = (sideways ^ forward);

	mx[0][0] = sideways.x;  mx[0][1] = sideways.y;  mx[0][2] = sideways.z; 
	mx[1][0] = upward.x;    mx[1][1] = upward.y;    mx[1][2] = upward.z; 
	mx[2][0] = -forward.x;	mx[2][1] = -forward.y;	mx[2][2] = -forward.z; 
	mx[3][0] = eye.x;		mx[3][1] = eye.y;		mx[3][2] = eye.z;

	mx[0][3] = mx[1][3] = mx[2][3] = 0.0f;
	mx[3][3] = 1.0f;

	return mx;
}

bool IsEquivalent(const cMatrix44& m1, const cMatrix44& m2)
{
	return IsEquivalent(m1.m[0][0], m2.m[0][0]) && IsEquivalent(m1.m[1][0], m2.m[1][0])
		&& IsEquivalent(m1.m[2][0], m2.m[2][0]) && IsEquivalent(m1.m[3][0], m2.m[3][0])
		&& IsEquivalent(m1.m[0][1], m2.m[0][1]) && IsEquivalent(m1.m[1][1], m2.m[1][1])
		&& IsEquivalent(m1.m[2][1], m2.m[2][1]) && IsEquivalent(m1.m[3][1], m2.m[3][1])
		&& IsEquivalent(m1.m[0][2], m2.m[0][2]) && IsEquivalent(m1.m[1][2], m2.m[1][2])
		&& IsEquivalent(m1.m[2][2], m2.m[2][2]) && IsEquivalent(m1.m[3][2], m2.m[3][2])
		&& IsEquivalent(m1.m[0][3], m2.m[0][3]) && IsEquivalent(m1.m[1][3], m2.m[1][3])
		&& IsEquivalent(m1.m[2][3], m2.m[2][3]) && IsEquivalent(m1.m[3][3], m2.m[3][3]);
}
