#pragma once
/*
	Copyright (C) 2005-2007 Feeling Software Inc.
	Portions of the code are:
	Copyright (C) 2005-2007 Sony Computer Entertainment America
	
	MIT License: http://www.opensource.org/licenses/mit-license.php
*/

/**
	@file Matrix44.h
	The file containing the class and global functions for 4x4 matrices.
*/
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
/**
	A 4x4 Row Major matrix: use to represent 3D transformations.

	@ingroup MyMath
*/

class ALIGN_STRUCT(16)
cMatrix44
{
public:
    union
    {
        VECTOR4 r[4];
        struct
        {
            float _11, _12, _13, _14;
            float _21, _22, _23, _24;
            float _31, _32, _33, _34;
            float _41, _42, _43, _44;
        };
        float m[4][4];
    };

	/** Creates a cMatrix44 from the \c float array.
		The float array stores the elements in the following order: m[0][0], 
		m[1][0], m[2][0], m[3][0], m[0][1], m[1][1], m[2][1], m[3][1], m[0][2],
		m[1][2], m[2][2], m[3][2], m[0][3], m[1][3], m[2][3], m[3][3].
		@param _m The \c float array to create the matrix from. */
	cMatrix44(const float* _m);
	cMatrix44(const double* _m); /**< See above. */

	/** Creates an empty cMatrix44.
		The default values are left un-initialized.
		To get an identity matrix:
		cMatrix44 identity(cMatrix44::Identity); */
	#ifndef DEBUG
	inline cMatrix44() {}
	#else
	inline cMatrix44() { memset(m, 55, 16 * sizeof(float)); }
	#endif 

	/** Get this cMatrix44 as an array of \c floats.
		The array contains the elements in the following order: m[0][0], 
		m[0][1], m[0][2], m[0][3], m[1][0], m[1][1], m[1][2], m[1][3], m[2][0],
		m[2][1], m[2][2], m[0][3], m[3][0], m[3][1], m[3][2], m[3][3].
		@return The \c float array. */
	inline operator float*() { return &m[0][0]; }
	inline operator const float*() const { return &m[0][0]; } /**< See above. */
	/** Get a specified row of cMatrix44 as an array of \c floats.
		@param a The row index, starting at 0, of the row to get.
		@return The \c float array of the elements in the specified row. */
	template <class Integer> float* operator[](Integer a) { return m[a]; }
	template <class Integer> const float* operator[](Integer a) const { return m[a]; } /**< See above. */

	/** Assign this cMatrix44's elements to be the same as that of the given 
		matrix.
		@param copy The FMMatrix to copy elements from.
		@return This FMMatrix. */
	cMatrix44& operator=(const cMatrix44& copy);

	/** Sets a cMatrix44 from the \c float array.
		The float array stores the elements in the following order: m[0][0],
		m[1][0], m[2][0], m[3][0], m[0][1], m[1][1], m[2][1], m[3][1], m[0][2], 
		m[1][2], m[2][2], m[3][2], m[0][3], m[1][3], m[2][3], m[3][3].
		@param _m The \c float array to create the matrix from. */
	void Set(const float* _m);
	void Set(const double* _m);	/**< See above. */

	/** Gets the transposed of this cMatrix44.
		@return The transposed of this FMMatrix. */
	cMatrix44 Transposed() const;

	/** Gets the inverse of this matrix.
		@return The inverse of this matrix. */
	cMatrix44 Inverted() const;

	/** Gets the determinant of this matrix.
		@return The determinant of this matrix. */
	float Determinant() const;
	
	/** Decompose this matrix into its scale, rotation, and translation
		components; it also tells whether it is inverted. To get back the 
		original matrix, perform the following multiplication:
		translation * axis-rotation [z*y*x order] * scale - or use
		the Recompose function. If invert is negative, 
		then to get back the original matrix, negate scale and perform the 
		above multiplication.
		@param scale The FMVector to place the scale components to.
		@param rotation The FMVector to place the rotation components to.
		@param translation The FMVector to place the translation components to.
		@param inverted -1.0 if inverted, 1.0 if not inverted. */
	void Decompose(Vector3& scale, Vector3& rotation, Vector3& translation, float& inverted) const;

	/** Recompose this matrix from its scale, rotation, and translation
		components.; it also tells whether it is inverted. To get back the 
		original matrix, perform the following multiplication:
		translation * rotation [x*y*z order] * scale. If invert is negative, 
		then to get back the original matrix, negate scale and perform the 
		above multiplication.
		@param scale The FMVector to place the scale components to.
		@param rotation The FMVector to place the rotation components to.
		@param translation The FMVector to place the translation components to.
		@param inverted -1.0 if inverted, 1.0 if not inverted. */
	void Recompose(const Vector3& scale, const Vector3& rotation, const Vector3& translation, float inverted = 1.0f);

	/** Transforms the given point by this matrix.
		@param coordinate The point to transform.
		@return The vector representation of the transformed point. */
	Vector2 TransformCoordinate(const Vector2& coordinate) const;
	Vector3 TransformCoordinate(const Vector3& coordinate) const;
	Vector4 TransformCoordinate(const Vector4& coordinate) const; /**< See above. */

	/** Transforms the given vector by this cMatrix44.
		@param v The vector to transform.
		@return The Vector3 representation of the transformed vector. */
	Vector3 TransformVector(const Vector3& v) const;
	Vector2 TransformVector(const Vector2& v) const;
	Vector4 TransformVector(const Vector4& v) const;

	/** Gets the translation component of this matrix.
		@return A Reference to the Vector3 representation of the translation. */
	inline const Vector3& GetTranslation() const { return GetAxis(MyMath::TRANS); }
	inline Vector3& GetTranslation() { return GetAxis(MyMath::TRANS); } /**< See Above */

	/** Sets the translation component of this matrix.
		@param translation The new translation component. */
	inline void SetTranslation(const Vector3& translation) { GetTranslation() = translation; }

	/** Gets a reference to an axis of this matrix
		@param axis The index of the axis to get (X = 0, Y = 1, Z = 2)
		@return A reference to the axis */
	inline const Vector3& GetAxis(MyMath::AXIS axis) const { return *(Vector3*)(size_t)(m[axis]); }
	inline Vector3& GetAxis(MyMath::AXIS axis) { return *(Vector3*)(size_t)(m[axis]); } /**< See above */

public:
	static cMatrix44 Identity;	/**< The identity matrix. */
	static cMatrix44 ZupToYUp;
	static cMatrix44 YupToYDown;
	static cMatrix44 X_TO_Z;
	static cMatrix44 Y_TO_Z;

	/** Gets the cMatrix44 representation of a 3D translation.
		The translation in the x, y and z directions correspond to the \a x, 
		\a y, and \a z components of the Vector3.
		@param translation The Vector3 to get the translation components from.
		@return The translation cMatrix44.  */
	static cMatrix44 TranslationMatrix(const Vector3& translation);
	static cMatrix44 TranslationMatrix(float e_fX,float e_fY,float e_fZ);
	static cMatrix44 TranslationMatrix(const VECTOR4& translation);

	/** Gets the cMatrix44 representation of a 3D rotation about a given axis
		by an angle.
		@param axis The axis of rotation.
		@param angle The angle of rotation in radians.
		@return The rotation cMatrix44. */
	static cMatrix44 AxisRotationMatrix(const Vector3& axis, float angle);

	/** Creates an cMatrix44 rotation matrix of angle degrees around the X axis
		@param angle The rotation angle in radians
		@returns The rotation cMatrix44 */
	static cMatrix44 XAxisRotationMatrix(float angle);

	/** Creates an cMatrix44 rotation matrix of angle degrees around the X axis
		@param angle The rotation angle in radians
		@returns The rotation cMatrix44 */
	static cMatrix44 YAxisRotationMatrix(float angle);

	/** Creates an cMatrix44 rotation matrix of angle degrees around the X axis
		@param angle The rotation angle in radians
		@returns The rotation cMatrix44 */
	static cMatrix44 ZAxisRotationMatrix(float angle);
	/** Creates an cMatrix44 rotation matrix of angle degrees if angle is not equal 0 
		@param angle The rotation angle in degree
		@returns The rotation cMatrix44 */
	static cMatrix44 RotationMatrix(Vector3 e_vAngle);

	/** Gets the matrix representation of a 3D euler rotation.
		The angles are considered in the order: x, y, z,
		which represents heading, banking and roll.
		@param rotation The euler rotation angles.
		@return The rotation matrix. */
	static cMatrix44 EulerRotationMatrix(const Vector3& rotation);

	/** Gets the cMatrix44 representation of a 3D axis-bound scale.
		@param scale The scaling components.
		@return The scale transform. */
	static cMatrix44 ScaleMatrix(const Vector3& scale);

	/** Gets the cMatrix44 represention of a look-at transformation.
		@param eye The eye position.
		@param target The target position.
		@param up The up direction.
		@return The look-at transformation matrix. */
	static cMatrix44 LookAtMatrix(const Vector3& eye, const Vector3& target, const Vector3& up);

	static cMatrix44	StripPosFromMatrix(cMatrix44 e_mat)
	{	
		e_mat.r[3].x = 0;
		e_mat.r[3].y = 0;
		e_mat.r[3].z = 0;
		return e_mat;
	}
};

/** Matrix multiplications.
	Effectively concatenates matrix transformations.
	FCollada does left-multiplication of matrices.
	@param m1 A first matrix.
	@param m2 A second matrix.
	@return The concatenation of the two matrix transformations. */
cMatrix44  operator*(const cMatrix44& m1, const cMatrix44& m2);

/** Transforms a four-dimensional vector by a given matrix.
	If the 'w' value of the vector is 1.0, this is a coordinate transformation.
	If the 'w' value of the vector is 0.0, this is a vector transformation.
	@param m A matrix.
	@param v A vector.
	@return The Vector4 representation of the resulting vector. */
Vector4  operator*(const cMatrix44& m, const Vector4& v);
Vector3  operator*(const cMatrix44& m, const Vector3& v);
Vector3  operator*(const Vector3& v  ,const cMatrix44& m);

/** Scalar multiplication with float and cMatrix44.
	All the components of the matrix get uniformly multiplied by the scalar.
	EDITOR NOTE: Is this functionality even useful? I cannot think of a reason
	this function would be used.
	@param a A scalar.
	@param m A matrix.
	@return The resulting matrix. */
cMatrix44  operator*(float a, const cMatrix44& m);

/** Matrix equality comparison function.
	@param m1 A first matrix.
	@param m2 A second matrix.
	@return Whether the given matrices are equal. */
bool  IsEquivalent(const cMatrix44& m1, const cMatrix44& m2);
inline bool operator==(const cMatrix44& m1, const cMatrix44& m2) { return IsEquivalent(m1, m2); } /**< See above. */

/** Left-multiplies a given matrix, in-place.
	This function effectively concatenates a transformation into
	a transform matrix.
	@param m1 The in-place multiplied matrix.
	@param m2 A matrix transformation.
	@return The in-place multiplied matrix. */
inline cMatrix44& operator*=(cMatrix44& m1, const cMatrix44& m2) { return m1 = m1 * m2; }
/** A dynamically-sized array of 4x4 matrices. */
//typedef fm::vector<cMatrix44> cMatrix44List;