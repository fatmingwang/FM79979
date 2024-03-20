#ifndef VECTOR3_H
#define	VECTOR3_H
#include "Vector2.h"
class Vector4;

/**
	A 3 dimensional vector.

	Simple, non-optimized vector class: * is the dot-product, ^ is the 
	cross-product.
	
	@ingroup MyMath
*/
class 
ALIGN_STRUCT(16)
Vector3
{
public:
	float x;	/**< The first coordinate. */
	float y;	/**< The second coordinate. */
	float z;	/**< The third coordinate. */
private:
	//float w;	// For alignment purposes.

public:
	/**
	 * Creates an empty Vector3.
	 */
	#ifndef DEBUG
	inline Vector3() {}
	#else
	inline Vector3() { x = 123456789.0f; y = 123456789.0f; z = 123456789.0f; }
	#endif 

	/** Creates the Vector3 with the coordinates given.
		@param _x The first coordinate.
		@param _y The second coordinate.
		@param _z The third coordinate. */
	inline Vector3(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }

	/** Copy constuctor. 
		@param rhs A second 3D vector. */
	inline Vector3(const Vector3& rhs) { x = rhs.x; y = rhs.y; z = rhs.z; }

	/** Constructor.
		Reduces the 4D vector into 3D by removing the 4th dimension.
		@param vect4 A 4D vector. */
	Vector3(const Vector4& vect4);

	/** Constructor.
		increase the 2D vector become 3D ,z set as  0
		@param vect2 A 2D vector. */
	Vector3(Vector2 vect2){ x = vect2.x; y = vect2.y; z = 0.f;}
	Vector3(const char*e_str);
	Vector3(const wchar_t*e_str);
	/** Creates the Vector3 from a list of floating-point values.
		It takes the first three floating-point starting from and including \a startIndex
		(0 indexing) in the array as the 3 coordinates. The first as the first 
		coordinate, the second as the second, and the third as the third.
		@param source The floating-point value array.
		@param startIndex The index of the first element. */
	Vector3(const float* source, int startIndex = 0);
	Vector3(const double* source, int startIndex = 0);	/**< See above. */
	
	inline	bool	IsVisible(Vector3 e_vVector)
	{
		Vector3	a = this->Normalize();
		Vector3	b = e_vVector.Normalize();
		float	l_fAngle = a.x * b.x + a.y * b.y + a.z * b.z;
		return l_fAngle<0?false:true;
	}
	inline	Vector2	ToVector2(){ return Vector2(x,y); }
	/**	Retrieves the squared length of the vector.
		@return The squared length of this vector. */
	inline float LengthSquared() const { return x * x + y * y + z * z; }

	/**	Retrieves the length of the vector.
		@return The length of this vector. */
	inline float Length() const { return sqrtf(x * x + y * y + z * z); }
	//2 points distance
	inline bool	IsCollided(Vector3 e_vPoint,float e_fRadius) const
	{
		Vector3 l_vVector = Vector3(e_vPoint.x - x, e_vPoint.y - y, e_vPoint.z - z);
		float l_fDis = l_vVector.Length();
		if (l_fDis <= e_fRadius)
		{
			return true;
		}
		return false;
	}

	inline bool	IsCollided(Vector2 e_vPoint, float e_fRadius) const
	{
		return IsCollided(Vector3(e_vPoint.x, e_vPoint.y,0.f),e_fRadius);
	}

	inline bool	IsCollided(float e_fPosX, float e_fPosY, float e_fRadius) const
	{

		return IsCollided(Vector3(e_fPosX, e_fPosY, 0.f), e_fRadius);
	}

	/** Normalizes this vector. */
	inline void NormalizeIt() { float l = Length(); if (l > 0.0f) { x /= l; y /= l; z /= l; } else { x = y = 0; z = 1; }}

	/** Get a normalized Vector3 with the same direction as this vector.
		@return A Vector3 with length 1 and same direction as this vector. */
	inline Vector3 Normalize() const { float l = Length(); return (l > 0.0f) ? Vector3(x / l, y / l, z / l) : Vector3::XAxis; }

	/** Project this Vector3 onto another Vector3.
		@param unto The Vector3 to project onto. */
	inline void Project(const Vector3& unto) { (*this) = Projected(unto); }
	inline void Project(const Vector3& unto,float* e_fMag) { (*this) = Projected(unto,e_fMag); }
	/** Get the projection of this Vector3 onto another Vector3.
		@param unto The Vector3 to project onto.
		@return The projected Vector3. */
	inline Vector3 Projected(const Vector3& unto) const;
	//project this vector on to v, return signed magnatude
	inline Vector3 Projected(const Vector3& unto,float* e_fMag);

	/** Get this Vector3 as an array of \c floats.
		@return The \c float array. */
	inline operator float*() { return &x; }

    /** Get this Vector3 as an array of \c floats.
		@return The \c float array. */
	inline operator const float*() const { return &x; }

	/** Set the values of this vector
		@param x The new X value
		@param y The new Y value
		@param z The new Z value */
	inline void Set(float x, float y, float z) { this->x = x; this->y = y, this->z = z; }

	/** Assign this Vector3 to the given float array.
		Assigns each coordinate of this Vector3 to the elements in the \c
		float array. The first element to the first coordinate, the second to
		the second, and the third to the third. It returns this Vector3.
		@param v The \c float array to assign with.
		@return This vector. */
	inline Vector3& operator =(const float* v) { x = *v; y = *(v + 1); z = *(v + 2); return *this; }

	/** Assigns the Vector3 passed to outselves.
		Copies XYZ from the passed vector
		@param rhs The vector copy off */
	inline Vector3& operator =(const Vector3& rhs) { x=rhs.x; y=rhs.y; z=rhs.z; return *this; }


	inline void	ToMin() { x = y = z = -FLT_MIN;}
	inline void	ToMax() { x = y = z = FLT_MAX;}

	/** Update each component of this FMVector to the minimum of two Vector3s.
		Updates each of the three components to be the minimum of the current
		value and that of the corresponding value of the given Vector3.
		@param min The FMVector to take values from. */
	inline void ComponentMinimum(const Vector3& min) { if (x > min.x) x = min.x; if (y > min.y) y = min.y; if (z > min.z) z = min.z; }

	/** Retrieves the smallest component of the vector.
		@return The smallest component of the vector. */
	inline float ComponentMinimum() const { return min(fabsf(x), min(fabsf(y), fabsf(z))); }

	/** Update each component of this FMVector to the maximum of two Vector3s.
		Updates each of the three components to be the maximum of the current
		value and that of the corresponding value of the given Vector3.
		@param max The FMVector to take values from. */
	inline void ComponentMaximum(const Vector3& max) { if (x < max.x) x = max.x; if (y < max.y) y = max.y; if (z < max.z) z = max.z; }

	/** Retrieves the largest component of the vector.
		@return The largest component of the vector. */
	inline float ComponentMaximum() const { return max(fabsf(x), max(fabsf(y), fabsf(z))); }

	/** Clamp each component of this FMVector by the corresponding components
		in the specified min and max Vector3.
		Clamp refers to setting a value within a given range. If the value is
		lower than the minimum of the range, it is set to the minimum; same for
		the maximum.
		@param min The FMVector to take the minimum values from.
		@param max The FMVector to take the maximum values from. */
	inline void ComponentClamp(const Vector3& min, const Vector3& max) { ComponentMinimum(min); ComponentMaximum(max); }

	/** Retrieves the average of the three vector components.
		@return The component average. */
	inline float ComponentAverage() const { return (fabsf(x) + fabsf(y) + fabsf(z)) / 3.0f; }

	inline Vector3 Abs(){ return Vector3((float)fabs(x), (float)fabs(y), (float)fabs(z)); }

public:
	static const Vector3 XAxis; /**< The Vector3 representing the x axis */
	static const Vector3 YAxis; /**< The Vector3 representing the y axis */
	static const Vector3 ZAxis; /**< The Vector3 representing the z axis */
	static const Vector3 Origin;/**< The Vector3 representing the origin */
	static const Vector3 Zero;  /**< The Vector3 containing all zeroes: (0,0,0). */
	static const Vector3 One;	  /**< The Vector3 containing all ones: (1,1,1). */
};


inline Vector2 operator +(const Vector2& a,const Vector3& b) { return Vector2(a.x+b.x,a.y+b.y); }
inline Vector2 operator +(const Vector3& a,const Vector2& b) { return Vector2(a.x+b.x,a.y+b.y); }
inline Vector2 operator *(const Vector2& a,const Vector3& b) { return Vector2(a.x*b.x,a.y*b.y); }
inline Vector2 operator *(const Vector3& a,const Vector2& b) { return Vector2(a.x*b.x,a.y*b.y); }
/** Vector addition with two Vector3.
	@param a The first vector.
	@param b The second vector.
	@return The Vector3 representation of the resulting vector. */
inline Vector3 operator +(const Vector3& a, const Vector3& b) { return Vector3(a.x + b.x, a.y + b.y, a.z + b.z); }

/** Vector subtraction with two Vector3.
	@param a The first vector.
	@param b The second vector.
	@return The Vector3 representation of the resulting vector. */
inline Vector3 operator -(const Vector3& a, const Vector3& b) { return Vector3(a.x - b.x, a.y - b.y, a.z - b.z); }

/** Positive operator of the given Vector3.
	It applies the positive operator to each of the components of the Vector3.
	@param a The vector to apply the positive operator to.
	@return The Vector3 representation of the resulting vector. */
inline Vector3 operator +(const Vector3& a) { return Vector3(+a.x, +a.y, +a.z); }

/** Negates the given Vector3.
	It negates each of the components of the Vector3.
	@param a The vector to negate.
	@return The Vector3 representation of the resulting vector. */
inline Vector3 operator -(const Vector3& a) { return Vector3(-a.x, -a.y, -a.z); }

/** Dot product of two Vector3.
	@param a The first vector.
	@param b The second vector.
	@return The result of the dot product. */
inline float operator *(const Vector3& a, const Vector3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
/** Scalar multiplication with a Vector3.
	@param a The vector.
	@param b The scalar.
	@return The Vector3 representing the resulting vector. */
inline Vector3 operator *(const Vector3& a, float b) { return Vector3(a.x * b, a.y * b, a.z * b); }

/** Scalar multiplication with a Vector3.
	@param a The scalar.
	@param b The vector.
	@return The Vector3 representing the resulting vector. */
inline Vector3 operator *(float a, const Vector3& b) { return Vector3(a * b.x, a * b.y, a * b.z); }

/** Scalar division with a Vector3.
	@param a The vector.
	@param b The scalar.
	@return The Vector3 representing the resulting vector. */
inline Vector3 operator /(const Vector3& a, float b) { return Vector3(a.x / b, a.y / b, a.z / b); }
inline Vector3 operator /(const Vector3& a, const Vector3& b) { return Vector3(a.x / b.x, a.y / b.y,a.z/b.z); }

/** Cross product of two Vector3.
	@param a The first vector.
	@param b The second vector.
	@return The result of the dot product. */
inline Vector3 operator ^(const Vector3& a, const Vector3& b) { return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }

/** Assignment of the addition of two Vector3.
	@param b The first vector, which will also be assigned to the result.
	@param a The second vector.
	@return The first vector, after it has been assigned new values. */
inline Vector3& operator +=(Vector3& b, const Vector3& a) { b.x += a.x; b.y += a.y; b.z += a.z; return b; }

/**	Assignment of the subtraction of two Vector3.
	@param b The first vector, which will also be assigned to the result.
	@param a The second vector.
	@return The first vector, after it has been assigned new values. */
inline Vector3& operator -=(Vector3& b, const Vector3& a) { b.x -= a.x; b.y -= a.y; b.z -= a.z; return b; }
inline Vector3& operator -=(Vector3& b, const Vector2& a) { b.x -= a.x; b.y -= a.y; return b; }

/** Assignment of the scalar multiplication of a Vector3.
	@param b The vector, which will also be assigned to the result.
	@param a The scalar.
	@return The vector, after it has been assigned new values. */
inline Vector3& operator *=(Vector3& b, float a) { b.x *= a; b.y *= a; b.z *= a; return b; }

/** Assignment of the scalar division of a Vector3.
	@param b The vector, which will also be assigned to the result.
	@param a The scalar.
	@return The vector, after it has been assigned new values. */
inline Vector3& operator /=(Vector3& b, float a) { b.x /= a; b.y /= a; b.z /= a; return b; }

/** Returns whether two 3D vectors or points are equivalent.
	@param p A first vector.
	@param q A second vector.
	@return Whether the vectors are equivalent. */
inline bool IsEquivalent(const Vector3& p, const Vector3& q) { return IsEquivalent(p.x, q.x) && IsEquivalent(p.y, q.y) && IsEquivalent(p.z, q.z); }
inline bool operator == (const Vector3& p, const Vector3& q) { return IsEquivalent(p, q); } /**< See above. */
//for 3D
//if the wall is right side normal would be (1,0,0)
//if the wall is left side normal would be (-1,0,0)
//if the wall is up side normal would be (0,1,0)
//if the wall is down side normal would be (0,-1,0)
//for 2D,because Y axis is up side down
//if the wall is right side normal would be (1,0,0)
//if the wall is left side normal would be (-1,0,0)
//if the wall is up side normal would be (0,-1,0)
//if the wall is down side normal would be (0,1,0)
Vector3	Vector3Reflect(Vector3	Incident,Vector3	Normal);
// Already documented above.
inline Vector3 Vector3::Projected(const Vector3& unto) const { return ((*this) * unto) / unto.LengthSquared() * unto; }
//project this vector on to v, return signed magnatude
inline Vector3 Vector3::Projected(const Vector3& unto,float* e_pfMag)
{
    //projected vector = (this dot v) * v;
    float thisDotV = (*this) * unto;
    *e_pfMag = thisDotV;
    return unto * thisDotV;
}
//	get direction offset angle the look at is 0,0,1
//after called this usuall called XMMatrixRotationY
//this one is right hand so z is 0,0,-1
float		GetDirectionOffset(Vector3 e_vDir);
//in the 2D z is y,and y is 0
float		GetDirectionOffset(Vector2 e_vDir);
//	get Up offset angle the look at is 0,1,0
//  after called this usuall called XMMatrixRotationX
//this one is right hand so z is 0,0,-1
float		GetUpOffset(Vector3 e_vUp);
inline	Vector3	Vector3Lerp(Vector3 e_vPos1,Vector3 e_vPos2,float e_fTime){	return (e_vPos2-e_vPos1)*e_fTime+e_vPos1;}
//These points are arranged in an anti-clockwise order around the face. In order to calculate Normals this MUST ALWAYS be the case
//For quads or any other irregular shaped faces you just need to choose 3 of them.
//The result will be the same because all points are on the same plane. We could use A,B,C OR A,B,D to calculate the normal for the quad above.
inline	Vector3 CalculateSurfaceNormal(Vector3 e_v1,Vector3 e_v2,Vector3 e_v3)
{
	Vector3 l_vNormal,l_v1,l_v2,l_vOutNomal;
	float l_fNormalisationFactor;
	//get 2 vector,then Cross Product.
	l_v1 = e_v2 - e_v1;
	l_v2 = e_v3 - e_v1;	
	l_vNormal = l_v1^l_v2;//same as below
	//l_vNormal.x = (l_v1.y * l_v2.z) - (l_v1.z * l_v2.y);
	//l_vNormal.y = -((l_v2.z * l_v1.x) - (l_v2.x * l_v1.z));
	//l_vNormal.z = (l_v1.x * l_v2.y) - (l_v1.y * l_v2.x);
	l_fNormalisationFactor = l_vNormal.Length();
	l_vOutNomal.x = l_vNormal.x / l_fNormalisationFactor;
	l_vOutNomal.y = l_vNormal.y / l_fNormalisationFactor;
	l_vOutNomal.z = l_vNormal.z / l_fNormalisationFactor;
	return l_vOutNomal;
}
//Without texture coordinates there is no one currect tangent and bi-normal vector.
//Any two vectors that are perpendicular to the normal, and to each other would be valid.
//The whole point of those vectors is to form a matrix which transforms your light vector 
//to a space in which the absolute normals in your normal map match the normals you would get,
//if you bumped that surface in the direction of it's face normal.
//Because the non-bumpedmapped normal in a normalmap often goes in the (0,0,1) direction,
//you transform the polygon so that it lays flat on the xy plane (and so the face's normal matches the up direction of the normal map).
//Then you rotate it so that the x and y coordinates match the u and v direction of your bumpmap,
//to make the absulate bumpmap normals match the normals you would get by bumping the plane by the heightmap.
//Note that if you rotate a heightmap the normals that it would generate will be rotated too,
//but if you rotate a normalmap, to normals will still point in the same direction in world space.
//Also note that you are never actually rotating the face yourself,
//but it's to imagine the face's position in surface local space.
//Now without a bumpmap,
//there is no way of letting the x and y coordinate of the surface local space match the u and v coordinates of a bumpmap,
//because you don't have one. But, because all normals will point in the (0,0,1) direction anyway,
//it doesn't matter how you rotate your face in surface local space, it doesn't affect the result at all,
//since the vector 0,0,1 will always stay the same.
//So to make a long story short. It doesn't matter which vectors you take.
//You can just take a vector (1,0,0) or so, then take the CrossProduct of that vector and the normal (and normalize it),
//use that as one vector, then take the crossproduct of that new vector and the normal (and normalize),
//and use that as the other vector. 
//It is important though that the bi normal and tangents of multiple vertices on the same face are more or less the same,
//so your vectors can't be totally arbitrary, but the method I described before will work fine for that.
//I may seem to overcomplicate things a bit, but I think that if you see this, you see the big picture, which is always a good thing imho.
inline	void ComputeTangentBasis(
      const Vector3& P1, const Vector3& P2, const Vector3& P3, 
      const Vector2& UV1, const Vector2& UV2, const Vector2& UV3,
      Vector3 &tangent, Vector3 &bitangent )
{
   Vector3 Edge1 = P2 - P1;
   Vector3 Edge2 = P3 - P1;
   Vector2 Edge1uv = UV2 - UV1;
   Vector2 Edge2uv = UV3 - UV1;

   float cp = Edge1uv.y * Edge2uv.x - Edge1uv.x * Edge2uv.y;

   if ( cp != 0.0f ) {
      float mul = 1.0f / cp;
      tangent   = (Edge1 * -Edge2uv.y + Edge2 * Edge1uv.y) * mul;
      bitangent = (Edge1 * -Edge2uv.x + Edge2 * Edge1uv.x) * mul;

	  tangent.Normalize();
      bitangent.Normalize();
   }
}
//http://www.gamedev.net/community/forums/topic.asp?topic_id=500579
//http://www.terathon.com/code/tangent.html
//bitange is get by below
//Cross(normal, tangent).*tangents.w
//bitangents[count] = (normals[count] % tangents[count]) * tangents[count].w;
//tangent and bitangent is xyz
//u have to create enough buffer for tangent and bitangents
//float l_fBuffer = new float[vertexCount*3];
void CalculateTangentArray(long vertexCount, const float *vertex, const float *normal,
        const float *texcoord, long triangleCount, const unsigned int*triangleIndices,
		float *tangent,float *bitangents);

std::vector<Vector3>	Vector2ToVector3(std::vector<Vector2>*e_Vector2Vector,float e_fZValue = 0.f);

Vector2	VectorToInteger(Vector2 e_Vec3);
Vector3	VectorToInteger(Vector3 e_Vec3);
Vector4	VectorToInteger(Vector4 e_Vec3);

#endif