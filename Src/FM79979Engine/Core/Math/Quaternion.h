#pragma once
/*
	Copyright (C) 2005-2009 Feeling Software Inc.
	Available only to licensees.
	Distribution of this file or its content is strictly prohibited.
*/

/**
	@file Quaternion.h
	The file containing the class for quaternions.
*/

#include "Vector4.h"
#include "Matrix44.h"
////#include <algorithm>
#include <cstdint>
/**
	A quaternion.
	Used to represent rotations: quaternions have the sole advantage of reduced
	floating-point error when concatenating rotations.
	
	@ingroup MyMath
*/
//please use it as below
//Quaternion::EulerRotationQuaternion(D3DXToRadian(AngleX),D3DXToRadian(AngleY),D3DXToRadian(AngleZ)).ToMatrix();
//above equal below
//cMatrix44::XAxisRotationMatrix(D3DXToRadian(m_fAngleX))*
//cMatrix44::YAxisRotationMatrix(D3DXToRadian(m_fAngleY))*
//cMatrix44::ZAxisRotationMatrix(D3DXToRadian(m_fAngleZ));
class Quaternion
{
public:
	float x;	/**< The x component. */
	float y;	/**< The y component. */
	float z;	/**< The z component. */
	float w;	/**< The w component. */

#ifndef DEBUG
	/**	Creates an empty FMQuaternion.
		The default values are non deterministic. */
	Quaternion() {}
#else
	Quaternion() { x = 123456789.0f; y = 123456789.0f; z = 123456789.0f; w = 123456789.0f; }
#endif 

	/**	Creates the quaternion with the given component values.
		@param _x The i component.
		@param _y The j component.
		@param _z The k component.
		@param _w The scalar component. */
	Quaternion(const float& _x, const float& _y, const float& _z, const float& _w) { x = _x; y = _y; z = _z; w = _w; }

	/**	Creates the quaternion with the given component values.
		@param values A static floating-point value array containing at least four elements. */
	Quaternion(const float* values);

	/**	Creates the quaternion from a given axis and angle of rotation.
		@param axis The axis of rotation.
		@param angle The angle of rotation in radians. */
	Quaternion(const Vector3& axis, const float& angle);

	/**	Creates the quaternion from a given rotation matrix.
		@param matrix The rotation matrix. */
	Quaternion(const cMatrix44& matrix);

	/** Retrieves this quaternion as an array of floating point values.
		@return The floating point values. */
	inline operator float*() { return &x; }
	inline operator const float*() const { return &x; } /**< See above. */

	/**	Retrieves the squared length of the vector.
		@return The squared length of this vector. */
	inline float LengthSquared() const { return x * x + y * y + z * z + w * w; }

	/**	Retrieves the length of the vector.
		@return The length of this vector. */
	inline float Length() const { return MyMath::SquareRoot(x * x + y * y + z * z + w * w); }

	/** Normalizes this vector. */
	inline void NormalizeIt() { float l = LengthSquared(); if (!IsEquivalent(l, (float) 0) && !IsEquivalent(l, (float) 1)) { l = MyMath::SquareRoot(l); x /= l; y /= l; z /= l; w /= l; }}

	/** Returns a quaternion with a unit length at the same space as this quaternion
		@return A FMVector3F with length 1 and same direction as this vector. */
	inline Quaternion Normalize() const { float l = Length(); return Quaternion(x / l, y / l, z / l, w / l); }

	/** Returns the concatenation of this quaternion rotation and the given
		quaternion rotation.
		@param q A second quaternion rotation.
		@return The result of the two quaternion rotation. */
	Quaternion operator*(const Quaternion& q) const;

	/** Returns the conjugate of this quaternion.
		@return The conjugate. */
	inline Quaternion operator~() const { return Quaternion(-x, -y, -z, w); }

	/** Applies quaternion multiplication of the given quaternion with this
		quaternion and returns the value.
		@param q The quaternion multiplied with.
		@return The resulting quaternion. */
	inline Quaternion& operator*=(const Quaternion& q) { return (*this) = (*this) * q; }

	/** Assign this FMQuaternion to the given floating point value array.
		Assigns each coordinate of this FMQuaternion to the elements in the 
		floating-point value array. The first element to the i component, the second to the
		j, the third to the k, and the forth to the scalar.
		@param v The floating point values.
		@return This quaternion. */
	inline Quaternion& operator=(const float* v) { x = *v; y = *(v + 1); z = *(v + 2); w = *(v + 3); return *this; }

	/** Copy constructor.
		Clones the given quaternion into this quaternion.
		@param q A quaternion to clone.
		@return This quaternion. */
	inline Quaternion& operator=(const Quaternion& q) { x = q.x; y = q.y; z = q.z; w = q.w; return (*this); }

	/** Returns the average of this quaternion to another quaternion.
		@param other A quaternion.
		@param factor The merge factor, in the range [0.0, 1.0].
		@return The merged quaternion formed by the average. */
	Quaternion Average(const Quaternion& other, const float& factor = (float) 0.5) const;

	/** Converts a quaternion to its Euler rotation angles.
		@param previousAngles To support animated quaternions conversion,
			you need to pass in the previous quaternion's converted angles.
			The closest angles to the previous angles will be returned for a smooth animation.
			If this parameter is nullptr, one valid set of angles will be returned.
		@return A 3D vector containing the Euler rotation angles. */
	Vector3 ToEuler(const Vector3* previousAngles = nullptr) const;

	Vector3 ToEulerWithDegree(const Vector3* previousAngles = nullptr) const;

	/** Converts a quaternion to a angle-axis rotation.
		@param axis (out) The returned axis for the rotation.
		@param angle (out) The returned angle for the rotation, in radians. */
	void ToAngleAxis(Vector3& axis, float& angle) const;

	/** Converts a quaternion to a transformation matrix.
		@return The transformation matrix for this quaternion. */
	cMatrix44 ToMatrix() const;

	/** Sets the Quaternions transform onto the specified matrix.
		This will overwrite any existing rotations, but not positions
		@param m (in-out) The matrix to set our transform onto. */
	void SetToMatrix(cMatrix44& m) const;

public:
	/** Retrieves whether this quaternion is equivalent to a given quaternion.
		@param b A quaternion.
		@return Whether the two quaternions are equivalent. */
	bool operator==(const Quaternion& b) const;

	/** Rotates a vector with this quaternion's rotation.
		NOTE: This is the same operation as "quaternion.ToMatrix().TransformVector(vec3d)"
		This effectively *applies* this quaternion's rotation to the vector.
		@param v A 3D vector to rotate.
		@return The rotated 3D vector. */
	Vector3 operator*(const Vector3& v) const;

public:
	/** Get the FMQuaternion representation of the Euler rotation angles.
		@param x The rotation about the x-axis (roll), in radians.
		@param y The rotation about the y-axis (pitch), in radians.
		@param z The rotation about the z-axis (yaw), in radians. */
	static Quaternion EulerRotationQuaternion(const float& x, const float& y, const float& z);
	static Quaternion EulerRotationQuaternion(Vector3 e_v);

	/** Get the FMQuaternion that represents the FMMatrix44 rotation
		@param mat The matrix whose rotation we will represent */
	static Quaternion MatrixRotationQuaternion(const cMatrix44& mat);

	/** Create a look-at quaternion.
		@param eye The position of the viewer.
		@param target The position targeted by the viewer.
		@param up The upward direction of the viewer. Defaults to the ZAxis.
		@return A quaternion representing this space basis. */
	static Quaternion LookAtQuaternion(const Vector3& eye, const Vector3& target, const Vector3& up = Vector3::ZAxis);

public:
	static const Quaternion Zero; /**< The zero quaternion. */

	/** The identity quaternion.
		Transforming a vector or a point with this quaternion
		returns the same vector or point. */
	static const Quaternion Identity;

	// Quaternion dot product
	static float Dot(const Quaternion& q1, const Quaternion& q2)
	{
		return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
	}

	// Quaternion Slerp function
	static Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t)
	{
		// Clamp interpolation factor between 0 and 1
		t = std::clamp(t, 0.0f, 1.0f);

		Quaternion qStart = q1.Normalize();
		Quaternion qEnd = q2.Normalize();

		float dot = Dot(qStart, qEnd);

		// If dot product is negative, flip one quaternion to take the shortest path
		if (dot < 0.0f)
		{
			qEnd.x = -qEnd.x;
			qEnd.y = -qEnd.y;
			qEnd.z = -qEnd.z;
			qEnd.w = -qEnd.w;
			dot = -dot;
		}

		// Use linear interpolation if quaternions are very close (to avoid division by zero)
		const float THRESHOLD = 0.9995f;
		if (dot > THRESHOLD)
		{
			// Perform LERP (Linear Interpolation)
			Quaternion result = {
				qStart.x + t * (qEnd.x - qStart.x),
				qStart.y + t * (qEnd.y - qStart.y),
				qStart.z + t * (qEnd.z - qStart.z),
				qStart.w + t * (qEnd.w - qStart.w)
			};
			return result.Normalize(); // Normalize result for safety
		}

		// Perform SLERP (Spherical Linear Interpolation)
		float theta_0 = (float)acos(dot); // Initial angle
		float theta = theta_0 * t;      // Scaled angle

		float sin_theta = (float)sin(theta);
		float sin_theta_0 = (float)sin(theta_0);

		float s0 = (float)cos(theta) - dot * sin_theta / sin_theta_0;
		float s1 = sin_theta / sin_theta_0;

		return {
			(s0 * qStart.x) + (s1 * qEnd.x),
			(s0 * qStart.y) + (s1 * qEnd.y),
			(s0 * qStart.z) + (s1 * qEnd.z),
			(s0 * qStart.w) + (s1 * qEnd.w)
		};
	}
};

/** Returns a vector with the given quaternion's inverse rotation
	applied to the given vector.
	NOTE: This is the same operation as "quaternion.ToMatrix().Inverted().TransformVector(vec3d)"
	This effectively *removes* the quaternion's rotation from the vector.
	@param v A 3D vector to rotate.
	@param q A quaternion.
	@return The rotated 3D vector. */
Vector3 operator*(const Vector3& v, const Quaternion& q);