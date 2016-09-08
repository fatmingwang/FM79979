/*
	Copyright (C) 2005-2007 Feeling Software Inc.
	Portions of the code are:
	Copyright (C) 2005-2007 Sony Computer Entertainment America
	
	MIT License: http://www.opensource.org/licenses/mit-license.php
*/

/**
	@file Vector4.h
	This file contains the class for 4 dimensional vectors.
*/
#ifndef VECTOR4_H
#define	VECTOR4_H
#include "Vector2.h"
#include "Vector3.h"
#define ISNAN(x)  ((*(unsigned int*)&(x) & 0x7F800000) == 0x7F800000 && (*(unsigned int*)&(x) & 0x7FFFFF) != 0)
#define ISINF(x)  ((*(unsigned int*)&(x) & 0x7FFFFFFF) == 0x7F800000)
#define SELECT_0         0x00000000
#define SELECT_1         0xFFFFFFFF

typedef struct __vector4
{
    union
    {
        struct
        {
            float x;
            float y;
            float z;
            float w;
        };
        float v[4];
        unsigned int  u[4];
    };
} __vector4;

// Vector intrinsic: Four 32 bit floating point components aligned on a 16 byte 
// boundary and mapped to hardware vector registers
typedef __vector4  VECTOR4;

inline VECTOR4 VECTOR4Set(float x,float y,float z,float w)
{
    VECTOR4 V;
    V.v[0] = x;
    V.v[1] = y;
    V.v[2] = z;
    V.v[3] = w;
    return V;
}
/**
	A 4 dimensional vector.
	Not used within FCollada.
	
	@ingroup MyMath
*/
class
ALIGN_STRUCT(16)
Vector4
{
public:
    union
    {
        struct
        {
			float x;	/**< The first coordinate. */
			float y;	/**< The second coordinate. */
			float z;	/**< The third coordinate. */
			float w;	/**< The fourth coordinate. */
        };
        struct
        {
			float r;	/**< The first coordinate. */
			float g;	/**< The second coordinate. */
			float b;	/**< The third coordinate. */
			float a;	/**< The fourth coordinate. */
        };
        float v[4];
        unsigned int  u[4];
    };

	/** Creates an empty 4D vector. */
	#ifndef DEBUG
	inline Vector4() {}
	#else
	inline Vector4() { x = 123456789.0f; y = 123456789.0f; z = 123456789.0f; w = 123456789.0f; }
	#endif 

	/** Creates the Vector4 from a list of \c floats.
		It takes the first 4 \c floats starting from and including \a startIndex
		(0 indexing) in the array as the 4 coordinates. The first as the first 
		coordinate, the second as the second, and the third as the third, the fourth as w
		@param source The \c float array.
		@param startIndex The index of the first element. */
	inline Vector4(const float* source, int startIndex = 0) { source = source + startIndex; x = *source++; y = *source++; z = *source++; w = *source; }

	/** Creates the Vector4 with the coordinates given.
		The first three coordinates are taken from the Vector3, where the
		first one is the x value, the second is that y, and the third is the z.
		The forth value is the \c float specified.
		@param v The Vector3 representing the first three coordinates.
		@param _w The final coordinate. */
	inline Vector4(const Vector3& v, float _w) { x = v.x; y = v.y; z = v.z; w = _w; }

	/** Creates the Vector4 with the coordinates given.
		@param v1 The Vector2 representing the first two coordinates.
		@param v2 The Vector2 representing the last two coordinates. */
	inline Vector4(const Vector2& v1, const Vector2& v2) { x = v1.x; y = v1.y; z = v2.x; w = v2.y; }

	/** Creates the Vector4 with the coordinates given.
		@param _x The first coordinate.
		@param _y The second coordinate.
		@param _z The third coordinate.
		@param _w The forth coordinate. */
	inline Vector4(float _x, float _y, float _z, float _w) { x = _x; y = _y; z = _z; w = _w; }

	/** Creates the 4D vector from a HSV color value.
		HSV stands for hue, saturation and value and is
		a more humane way to express color values, as opposed to RGB.
		@param hue The color hue. In the range [0,1].
		@param saturation The color saturation. In the range [0,1].
		@param value The color value. In the range [0,1]. */
	static Vector4 FromHSVColor(float hue, float saturation, float value);

	/** Retrieves the HSV color value for this RGBA color value.
		All the color components are expected to be in the [0,1] range:
		no clamping will be done.
		@return The equivalent HSV color value. All the components will be
			in the [0,1] range. */
	Vector3 ToHSVColor();
	//RGBA
	Vector4(int e_Color);
	Vector4(const char*e_str);
	Vector4(const wchar_t*e_str);
public:
	/** Get this Vector4 as an array of \c floats.
		@return The \c float array. */
	inline operator float*() { return &x; }

	/** Get this Vector4 as an array of \c floats.
		@return The \c float array. */
	inline operator const float*() const { return &x; }

	/** Assign this Vector4 to the given float array.
		Assigns each coordinate of this Vector4 to the elements in the \c
		float array. The first element to the first coordinate, the second to
		the second, and the third to the third. It returns this Vector4.
		@param v The \c float array to assign with.
		@return This Vector4. */
	inline Vector4& operator =(const float* v) { x = *v; y = *(v + 1); z = *(v + 2); w = *(v+3); return *this; }

	/** Update each component of this FMVector to the minimum of two Vector4s.
		Updates each of the four components to be the minimum of the current
		value and that of the corresponding value of the given Vector4.
		@param min The FMVector to take values from. */
	inline void ComponentMinimum(const Vector4& min) { if (x < min.x) x = min.x; if (y < min.y) y = min.y; if (z < min.z) z = min.z; if (w < min.w) w = min.w; }

	/** Update each component of this FMVector to the maximum of two Vector4s.
		Updates each of the four components to be the maximum of the current
		value and that of the corresponding value of the given Vector4.
		@param max The FMVector to take values from. */
	inline void ComponentMaximum(const Vector4& max) { if (x > max.x) x = max.x; if (y > max.y) y = max.y; if (z > max.z) z = max.z; if (w > max.w) w = max.w;}

	inline	void    SetColor(unsigned char*e_pColor){	x = e_pColor[0]/255.f;	y = e_pColor[1]/255.f;	z = e_pColor[2]/255.f;	w = e_pColor[3]/255.f;}
	inline	void    SetColor(unsigned char e_cR,char e_cG,char e_cB,char e_cA){	x = e_cR/255.f;	y = e_cG/255.f;	z = e_cB/255.f;	w = e_cA/255.f;}
	//find center position,scale width and height then minus x,y,z,w by width or height's new length
	//make a center basis scale.
	inline	Vector4	ScaleByCenter(float e_fScale)
	{
		float	l_fScale = (1-e_fScale)/2;
		float	l_fWidth =	(z-x)*l_fScale;
		float	l_fHeight = (w-y)*l_fScale;
		return Vector4(x+l_fWidth,y+l_fHeight,z-l_fWidth,w-l_fHeight);
	}

	inline	Vector2	GetCenter(){return Vector2((z-x)/2+x,(w-y)/2+y);}
	inline	void	Move(Vector2 e_vPos){ x += e_vPos.x; y += e_vPos.y; z += e_vPos.x; w += e_vPos.y; }
	inline	void	Move(Vector3 e_vPos){ x += e_vPos.x; y += e_vPos.y; z += e_vPos.x; w += e_vPos.y; }
	inline	void	Move(float e_fX,float e_fY){ x += e_fX; y += e_fY; z += e_fX; w += e_fY; }
	inline	void	Move(int e_iX,int e_iY){ x += e_iX; y += e_iY; z += e_iX; w += e_iY; }
	//while touch points is at any point on the sctreen,smoothly to move center by new position
	inline	Vector4	ScaleToNewPos(Vector2 e_vCenterPos,float e_fScale)
	{
		Vector2	l_vCenter = GetCenter();
		l_vCenter.x += ((e_vCenterPos.x-l_vCenter.x))*(1-e_fScale);
		l_vCenter.y += ((e_vCenterPos.y-l_vCenter.y))*(1-e_fScale);
		float	l_fHalfWidth =	(z-x)*e_fScale/2.f;
		float	l_fHalfHeight = (w-y)*e_fScale/2.f;
		return Vector4(l_vCenter.x-l_fHalfWidth,l_vCenter.y-l_fHalfHeight,l_vCenter.x+l_fHalfWidth,l_vCenter.y+l_fHalfHeight);
	}

	inline	bool	Collide(Vector4 e_vRect)
	{
		if(e_vRect.x > z) return false;
		if(e_vRect.z < x) return false;
		if(e_vRect.y > w) return false;
		if(e_vRect.w < y) return false;
		return true;
	}
	inline	bool	CollidePoint(float e_fPosX,float e_fPosY)
	{
		if(this->x<=e_fPosX&&this->z>=e_fPosX&&this->y<=e_fPosY&&this->w>=e_fPosY)
			return true;
		return false;
	}
	inline	bool	CollidePoint(int e_iPosX,int e_iPosY)
	{
		if(this->x<=e_iPosX&&this->z>=e_iPosX&&this->y<=e_iPosY&&this->w>=e_iPosY)
			return true;
		return false;		
	}
	inline	float		ClampWidth(float e_fValue){if(e_fValue>z)return z;if(e_fValue<x)return x;return e_fValue;}
	inline	float		ClampHeight(float e_fValue){if(e_fValue>w)return w;if(e_fValue<y)return y;return e_fValue;}
	inline	int			ClampWidth(int e_iValue){if(e_iValue>z)return (int)z;if(e_iValue<x)return (int)x;return e_iValue;}
	inline	int			ClampHeight(int e_iValue){if(e_iValue>w)return (int)w;if(e_iValue<y)return (int)y;return e_iValue;}

	inline	void		Clamp(Vector4 e_vRestrictRange)
	{
		if( x<e_vRestrictRange.x )
			x = e_vRestrictRange.x;
		if( y<e_vRestrictRange.y )
			y = e_vRestrictRange.y;
		if( z>e_vRestrictRange.z )
			z = e_vRestrictRange.z;
		if( w>e_vRestrictRange.w )
			w = e_vRestrictRange.w;
	}
	inline	void		ClampWithSize(Vector4 e_vRestrictRange,float e_fWidth,float e_fHeight)
	{
		if( x<e_vRestrictRange.x )
		{
			x = e_vRestrictRange.x;
			z = x+e_fWidth;
		}
		else
		if( z>e_vRestrictRange.z )
		{
			z = e_vRestrictRange.z;
			x = z-e_fWidth;
		}
		if( y<e_vRestrictRange.y )
		{
			y = e_vRestrictRange.y;
			w = y+e_fHeight;
		}
		else
		if( w>e_vRestrictRange.w )
		{
			w = e_vRestrictRange.w;
			y = w-e_fHeight;
		}
	}
	inline	float		Width(){ return z-x; }
	inline	float		Height(){ return w-y; }
	inline	Vector2		Size(){ return Vector2(Width(),Height()); }
	//give a point find out which 2 sides are close the point.
	inline	Vector2	GetCloseWall(float e_fPosX,float e_fPosY)
	{
		float	l_fDisX = e_fPosX-this->x;
		float	l_fDisZ = this->z-e_fPosX;
		float	l_fDisY = e_fPosY-this->y;
		float	l_fDisW = this->w-e_fPosY;
		return Vector2(l_fDisX>l_fDisZ?z:x,l_fDisY>l_fDisW?w:y);
	}
	inline Vector3	GetRelativeDirectionFromPos(Vector3 e_vPos)
	{
		return Vector3(0.5f-(e_vPos.x/Width()),0.5f-(e_vPos.y/Height()),0).Normalize();
	}
	Vector4			Extend(float e_fExtendSize){ return Vector4(x-e_fExtendSize,y-e_fExtendSize,z+e_fExtendSize,w+e_fExtendSize); }
	bool			CollideReflect(Vector3 e_vPos,Vector3 e_vDirection,Vector3*e_pvRefelectDirection,Vector3*e_pvPos,float e_fRadius)
	{
		Vector3	l_vReflectNormal(0,0,0);	bool	l_bHit = false;		*e_pvPos = e_vPos;

		float	l_fLeftPos = e_vPos.x-e_fRadius;
		float	l_fRightPos = e_vPos.x+e_fRadius;
		float	l_fUpPos = e_vPos.y-e_fRadius;
		float	l_fDownPos = e_vPos.y+e_fRadius;

		if( l_fLeftPos < x ){	l_vReflectNormal.x = 1.f;	l_bHit = true;	e_pvPos->x = x+e_fRadius;}
		else 
		if( l_fRightPos > z ){	l_vReflectNormal.x = -1.f;	l_bHit = true;	e_pvPos->x = z-e_fRadius;} 
		//
		if( l_fUpPos < y ){		l_vReflectNormal.y = 1.f;	l_bHit = true;	e_pvPos->y = y+e_fRadius;}
		else 
		if( l_fDownPos > w ){	l_vReflectNormal.y = -1.f;	l_bHit = true;	e_pvPos->y = w-e_fRadius;} 
		//
		if( l_bHit ){*e_pvRefelectDirection = Vector3Reflect(e_vDirection,l_vReflectNormal);}
		return l_bHit;
	}
	inline bool	 IsOutOfRange(Vector4 e_vViewRect)
	{
		if( e_vViewRect.x < x ||
			e_vViewRect.y < y ||
			e_vViewRect.z > z ||
			e_vViewRect.w > w )
		{
			return true;
		}
		return false;
	}
	//a rect contain a smaller rect.
	inline	bool	IsContainRect(Vector4 e_vViewRect)
	{
		if(x < e_vViewRect.x &&y < e_vViewRect.y &&z > e_vViewRect.z &&w > e_vViewRect.w)
			return true;
		return false;
	}
	inline float Length() const { return sqrtf(x * x + y * y + z * z+w*w); }

	static const Vector4 Zero;	/**< The 4D vector containing all zeroes. */
	static const Vector4 One;		/**< The 4D vector containing all ones. */
	static const Vector4 AlphaOne;
	static const Vector4 Red;
	static const Vector4 Green;
	static const Vector4 Blue;
	static const Vector4 HalfDark;
	static const Vector4 HalfBright;
	static const Vector4 HalfBrightWithHalfAlpha;
};

/** Scalar multiplication with a Vector4.
	@param a The vector.
	@param b The scalar.
	@return The Vector4 representing the resulting vector. */
inline Vector4 operator *(const Vector4& a, float b) { return Vector4(a.x * b, a.y * b, a.z * b, a.w * b); }

/** Scalar multiplication with a Vector4.
	@param a The scalar.
	@param b The vector.
	@return The Vector4 representing the resulting vector. */
inline Vector4 operator *(float a, const Vector4& b) { return Vector4(a * b.x, a * b.y, a * b.z, a * b.w); }

/** Vector addition with two Vector4.
	@param a The first vector.
	@param b The second vector.
	@return The Vector4 representation of the resulting vector. */
inline Vector4 operator +(const Vector4& a, const Vector4& b) { return Vector4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }

/** Vector substraction with two Vector4.
	@param a The first vector.
	@param b The second vector.
	@return The Vector4 representation of the resulting vector. */
inline Vector4 operator -(const Vector4& a, const Vector4& b) { return Vector4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }

/** Assignment of the addition of two Vector4.
	@param b The first vector, which will also be assigned to the result.
	@param a The second vector.
	@return The first vector, after it has been assigned new values. */
inline Vector4& operator +=(Vector4& b, const Vector4& a) { b.x += a.x; b.y += a.y; b.z += a.z; b.w += a.w; return b; }

/** Assignment of the subtraction of two Vector4.
	@param b The first vector, which will also be assigned to the result.
	@param a The second vector.
	@return The first vector, after it has been assigned new values. */
inline Vector4& operator -=(Vector4& b, const Vector4& a) { b.x -= a.x; b.y -= a.y; b.z -= a.z; b.w -= a.w; return b; }

/** Assignment of the scalar multiplication of a Vector4.
	@param b The vector, which will also be assigned to the result.
	@param a The scalar.
	@return The vector, after it has been assigned new values. */
inline Vector4& operator *=(Vector4& b, float a) { b.x *= a; b.y *= a; b.z *= a; b.w *= a; return b; }

/** Assignment of the scalar division of a Vector4.
	@param b The vector, which will also be assigned to the result.
	@param a The scalar.
	@return The vector, after it has been assigned new values. */
inline Vector4& operator /=(Vector4& b, float a) { b.x /= a; b.y /= a; b.z /= a; b.w /= a; return b; }

inline Vector4 operator /(Vector4 b, float a) { b.x /= a; b.y /= a; b.z /= a; b.w /= a; return b; }

/** Dot product of two Vector4.
	@param a The first vector.
	@param b The second vector.
	@return The result of the dot product. */
inline float operator *(const Vector4& a, const Vector4& b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }


inline Vector4	ToVector4(VECTOR4 e_v) { return Vector4(e_v.x,e_v.y,e_v.z,e_v.w); }
inline Vector3	ToVector3(VECTOR4 e_v) { return Vector3(e_v.x,e_v.y,e_v.z); }

/** Returns whether two 4D vectors are equivalent.
	@param p A first vector.
	@param q A second vector.
	@return Whether the vectors are equivalent. */
inline bool IsEquivalent(const Vector4& p, const Vector4& q) { return IsEquivalent(p.x, q.x) && IsEquivalent(p.y, q.y) && IsEquivalent(p.z, q.z) && IsEquivalent(p.w, q.w); }
inline bool operator==(const Vector4& p, const Vector4& q) { return IsEquivalent(p.x, q.x) && IsEquivalent(p.y, q.y) && IsEquivalent(p.z, q.z) && IsEquivalent(p.w, q.w); } /**< See above. */
inline bool operator!=(const Vector4&p, const Vector4& q)
{
	if(!IsEquivalent(p.x, q.x)||!IsEquivalent(p.y, q.y)||!IsEquivalent(p.z, q.z)||!IsEquivalent(p.w, q.w))
		return true;
	return false;
}

inline	Vector4	Vector4Multiply(Vector4 e_v1,Vector4 e_v2){ return Vector4(e_v1.x*e_v2.x,e_v1.y*e_v2.y,e_v1.z*e_v2.z,e_v1.w*e_v2.w); }
inline	Vector4	Vector4Multiply(Vector4 e_v1,VECTOR4 e_v2){ return Vector4(e_v1.x*e_v2.x,e_v1.y*e_v2.y,e_v1.z*e_v2.z,e_v1.w*e_v2.w); }

Vector4 VectorNearEqual(Vector4 V1,Vector4 V2,Vector4 Epsilon);
bool Vector4NearEqual(Vector4 V1,Vector4 V2,Vector4 Epsilon);
//is vector a number?
//http://en.wikipedia.org/wiki/NaN
bool Vector4IsNaN(Vector4 V);
//is vector infinitine?
bool Vector4IsInf(Vector4 V);
///** A dynamically-sized array of 4D vectors or points. */
//typedef fm::vector<Vector4> Vector4List;




Vector4 VectorMax(Vector4 V1,Vector4 V2);
Vector4 Vector4Select(Vector4 V1,Vector4 V2,Vector4 Control);
Vector4 Vector4Cross(Vector4 V1,Vector4 V2,Vector4 V3);
Vector4 Vector3Dot(Vector3 V1,Vector3 V2);
Vector4 Vector4Dot(Vector4 V1,Vector4 V2);
Vector4 Vector3LengthSq(Vector3 V);
Vector4 Vector4LengthSq(Vector4 V);
Vector4 VectorReciprocalEst(Vector4 V);
Vector4 VectorReciprocalSqrtEst(Vector4 V);
Vector4 VectorEqualInt(Vector4	V1,Vector4 V2);
Vector4	VectorEqual(Vector4 V1,Vector4 V2);
Vector4 VectorSqrt(Vector4 V);
Vector4 VectorSqrtEst(Vector4 V);
Vector4 Vector4Length(Vector4 V);
Vector3 Vector3LengthEst(Vector3 V);
Vector4 Vector3Cross(Vector4 V1,Vector4 V2);
// Replicate the w component of the vector
Vector4 VectorSplatX(Vector4 V);
Vector4 VectorSplatY(Vector4 V);
Vector4 VectorSplatZ(Vector4 V);
Vector4 VectorSplatW(Vector4 V);
Vector4	VectorReciprocal(Vector4	V);
bool	Vector4LessOrEqual(Vector4 V1,Vector4 V2);
Vector4	VectorLessOrEqual(Vector4	V1,Vector4	V2);
Vector4	Vector3Normalize(Vector4 V);
//define
static Vector4	g_XMSelect0101  = Vector4((float)SELECT_0, (float)SELECT_1, (float)SELECT_0, (float)SELECT_1);
static Vector4	g_XMSelect1010  = Vector4((float)SELECT_1, (float)SELECT_0, (float)SELECT_1, (float)SELECT_0);
static Vector4	g_XMSelect1110  = Vector4((float)SELECT_1, (float)SELECT_1, (float)SELECT_1, (float)SELECT_0);
static Vector4	g_XMInfinity	= Vector4(0x7F800000, 0x7F800000, 0x7F800000, 0x7F800000);
static Vector4	g_XMEpsilon		= Vector4(1.192092896e-7f,1.192092896e-7f,1.192092896e-7f,1.192092896e-7f);
static Vector4	g_XMQNaN		= Vector4(0x7FC00000, 0x7FC00000, 0x7FC00000, 0x7FC00000);
#endif