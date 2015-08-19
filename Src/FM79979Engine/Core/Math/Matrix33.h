/*
	Copyright (C) 2005-2007 Feeling Software Inc.
	Portions of the code are:
	Copyright (C) 2005-2007 Sony Computer Entertainment America
	
	MIT License: http://www.opensource.org/licenses/mit-license.php
*/

/**
	@file Matrix33.h
	The file containing the class and global functions for 3x3 matrices.
*/

#ifndef _FM_MATRIX33_H_
#define _FM_MATRIX33_H_
#include "Vector2.h"
/**
	A 3x3 matrix: use to represent 2D transformations.
	Not used within FCollada.
	
	@ingroup MyMath
*/
class cMatrix33
{
public:
	float m[3][3];	/**< The matrix elements stored in a 2D array. */

	/**
	 * Creates a cMatrix33 from the \c float array.
	 *
	 * The float array stores the elements in the following order: m[0][0], 
	 * m[1][0], m[2][0], m[0][1], m[1][1], m[2][1], m[0][2], m[1][2], m[2][2].
	 *
	 * @param _m The \c float array to create the matrix from.
	 */
	cMatrix33(float* _m);
	
	/**
	 * Creates an empty cMatrix33.
	 *
	 * The default values are non deterministic.
	 */
	#ifndef DEBUG
	cMatrix33() {}
	#else
	cMatrix33() { memset(m, 55, 3 * 3 * sizeof(float)); }
	#endif 

	/**
	 * Get this cMatrix33 as an array of \c floats.
	 *
	 * The array contains the elements in the following order: m[0][0], 
	 * m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2].
	 *
	 * @return The \c float array.
	 */
	operator float*() { return &m[0][0]; }

	/**
	 * Get this FMMatrix as an array of \c floats.
	 *
	 * The array contains the elements in the following order: m[0][0], 
	 * m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2].
	 *
	 * @return The \c float array.
	 */
	operator const float*() const { return &m[0][0]; }

	/**
	 * Get a specified row of cMatrix33 as an array of \c floats.
	 *
	 * @param a The row index, starting at 0, of the row to get.
	 * @return The \c float array of the elements in the specified row.
	 */
	float* operator[](int a) { return m[a]; }

	/**
	 * Assign this cMatrix33's elements to be the same as that of the given 
	 * cMatrix33.
	 *
	 * @param copy The FMMatrix to copy elements from.
	 * @return This FMMatrix.
	 */
	cMatrix33& operator=(const cMatrix33& copy);

	/**
	 * Gets the transposed of this cMatrix33.
	 *
	 * @return The transposed of this FMMatrix.
	 */
	cMatrix33 Transposed() const;

	/**
	 * Gets the inversion of this cMatrix33.
	 *
	 * @return The inversion of this FMMatrix.
	 */
	cMatrix33 Inverted() const;

public:
	static cMatrix33 identity;	/**< The identity cMatrix33. */

	/**
	 * Gets the cMatrix33 representation of a 2D counter-clockwise rotation
	 * about the z-axis.
	 *
	 * @param angle The angle of rotation in radians.
	 * @return The rotation FMMatrix.
	 */
	static cMatrix33 RotationMatrix(float angle);

	/**
	 * Gets the cMatrix33 representation of a 2D translation.
	 *
	 * @param tx The translation in the x direction.
	 * @param ty The translation in the y direction.
	 * @return The translation FMMatrix.
	 */
	static cMatrix33 TranslationMatrix(float tx, float ty);
	
	/**
	 * Gets the cMatrix33 representation of a 2D scale.
	 *
	 * @param sx The scale factor in the x direction.
	 * @param sy The scale factor in the y direction.
	 * @return The scale FMMatrix.
	 */
	static cMatrix33 ScaleMatrix(float sx, float sy);

	/**
	 * Gets the cMatrix33 representation of a 2D translation.
	 *
	 * The translation in the x direction is the \a u componenet of the given
	 * Vector2 and the translation in the y direction is the \a v component.
	 *
	 * @param translation The Vector2 to get the translation components from.
	 * @return The translation cMatrix33.
	 */
	static inline cMatrix33 TranslationMatrix(Vector2 translation) { return TranslationMatrix(translation.u, translation.v); }

	/**
	 * Gets the cMatrix33 representation of a 2D scale.
	 *
	 * The scale in the x direction is the \a u componenet of the given
	 * FMVector and the scale in the y direction is the \a v component.
	 *
	 * @param scale The Vector2 to get the scale components from.
	 * @return The scale cMatrix33.
	 */
	static inline cMatrix33 ScaleMatrix(Vector2 scale) { return ScaleMatrix(scale.u, scale.v); }
};

/**
 * Matrix multiplication with two cMatrix33.
 *
 * @param m1 The first matrix.
 * @param m2 The second matrix.
 * @return The cMatrix44 representation of the resulting matrix.
 */
cMatrix33 operator*(const cMatrix33& m1, const cMatrix33& m2);

#endif // _FM_MATRIX33_H_
