#ifndef VECTOR2_H
#define VECTOR2_H
//
//#if !defined(_INC_FLOAT)
//#include <float.h>
//#endif

#include <float.h>
#include "math.h"
#include <vector>
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
using namespace std;

#ifdef WIN32
//#pragma warning(disable:4324) // Don't bother me about forcing the padding of aligned structure.
/** Alignment macro for classes and structures.
	Only supported in MSVS 2005 for now.
	@param byteCount The number of bytes to align to.*/
//#define ALIGN_STRUCT(byteCount) __declspec(align(byteCount))
#pragma warning( disable : 4996 )
#define ALIGN_STRUCT(byteCount)
#else // !WIN32
#define ALIGN_STRUCT(byteCount)
#endif // WIN32

#ifndef EPSIONAL
#define	EPSIONAL	0.0001f
#endif
/** The default tolerance for double-sized floating-point comparison functions. */
#define DBL_TOLERANCE 0.0001
/** The default tolerance for single-sized floating-point comparison functions. */
#define FLT_TOLERANCE 0.0001f

/* min positive value */ 
#ifndef FLT_MIN
#define FLT_MIN 1.175494351e-38F 
#endif
/* max value */ 
#ifndef FLT_MAX
#define FLT_MAX 3.402823466e+38F 
#endif
/* smallest such that 1.0+FLT_EPSILON != 1.0 */ 
#ifndef FLT_EPSILON
#define FLT_EPSILON 1.192092896e-07F 
#endif
/** Returns whether two floating-point values are equivalent within a given tolerance.
	@param f1 A first floating-point value.
	@param f2 A second floating-point value. */
inline bool IsEquivalent(float f1, float f2) { return f1 - f2 < FLT_TOLERANCE && f2 - f1 < FLT_TOLERANCE; }

/** Returns whether two floating-point values are equivalent within a given tolerance.
	@param f1 A first floating-point value.
	@param f2 A second floating-point value.
	@param tolerance The tolerance in which to accept the two floating-point values as equivalent. */
inline bool IsEquivalent(float f1, float f2, float tolerance) { return f1 - f2 < tolerance && f2 - f1 < tolerance; }

/** Returns whether two double-sized floating-point values are equivalent.
	@param f1 A first double-sized floating-point value.
	@param f2 A second double-sized floating-point value. */
inline bool IsEquivalent(double f1, double f2) { return f1 - f2 < DBL_TOLERANCE && f2 - f1 < DBL_TOLERANCE; }

/** Returns whether two double-sized floating-point values are equivalent within a given tolerance.
	@param f1 A first double-sized floating-point value.
	@param f2 A second double-sized floating-point value.
	@param tolerance The tolerance in which to accept the two double-sized floating-point values as equivalent. */
inline bool IsEquivalent(double f1, double f2, double tolerance) { return f1 - f2 < tolerance && f2 - f1 < tolerance; }

#ifndef FLT_MAX
/** The maximum value that can be expressed using a 32-bit floating point value.
	This macro is pre-defined in the Windows API but is missing on MacOSX and other platforms. */
#define FLT_MAX __FLT_MAX__
#endif
namespace MyMath
{
    /** Mathematical value of pi to 50 decimals. */
	const double Pi = 3.14159265358979323846264338327950288419716939937510; 
	const float Pif = 3.14159265358979323846264338327950288419716939937510f; /**< See above. */
	const double TwoPi = 6.283185307179586476925286766558; /**< Mathematical value of 2*Pi to double precision. (360 degrees)*/
	const double PiOverTwo = 1.5707963267948966192313216916395; /**< Mathematical value of Pi/2 to single precision. (90 degrees)*/

    /** Numerical values for the different axis of the standard axis system.
        You can use these enumerated types with the FcMatrix44::GetAxis function. */
	enum AXIS
	{
		X = 0, /**< The X-axis. */
		Y, /**< The Y-axis. */
		Z, /**< The Z-axis. */
        W, /**< The W-axis. */
        
		TRANS = W/**< The 3D translation value. */
	};

	/** Convert radians to degrees.
		@param val The value in radians.
		@return The value in degrees. */
	inline double RadToDeg(double val) { return (val * 180.0/Pi); }

	/** Convert radians to degrees.
		@param val The value in radians.
		@return The value in degrees. */
	inline float RadToDeg(float val) { return (val * 180.0f/Pif); }

	/** Convert degrees to radians.
		@param val The value in degrees.
		@return The value in radians. */
	inline double DegToRad(double val) { return (val * Pi/180.0); }

	/** Convert degrees to radians.
		@param val The value in degrees.
		@return The value in radians. */
	inline float DegToRad(float val) { return (val * (float)Pi/180.0f); }

	/** Determines if given float is encoding for not a number (NAN).
		@param f The float to check.
		@return 0 if it is a number, something else if is NAN. */
#ifdef WIN32
	inline int IsNotANumber(float f) { return _isnan(f); }
#elif __PPU__
	inline int IsNotANumber(float f) { return !isfinite(f); }
#else // Linux and Mac
	//inline int IsNotANumber(float f) { return !finite(f); }
#endif

	/**	Retrieves the sign of a number.
		@param val The number to check.
		@return 1 if positive, -1 if negative. */
	template <class T>
	inline T Sign(const T& val) { return (val >= T(0)) ? T(1) : T(-1); }

	/** Clamps the specified object within a range specified by two other objects
		of the same class.
		Clamping refers to setting a value within a given range. If the value is
		lower than the minimum of the range, it is set to the minimum; same for
		the maximum.
		@param val The object to clamp.
		@param mx The highest object of the range.
		@param mn The lowest object of the range.
		@return The clamped value. */
	template <class T, class T2, class T3>
	inline T Clamp(T val, T2 mn, T3 mx) { return (T) ((val > (T) mx) ? (T) mx : (val < (T) mn) ? (T) mn : val); }

	/** Wraps the specified object within a range specified by two other objects
		of the same class.
		Wrapping refers to looping around maximum and minimum values until we're
		within that range. e.g. Provided that mn = 0.0f, mx = 1.0f and val = 1.01f,
		then the Wrap result will be 0.01f.
		@param val The value to wrap.
		@param mn The minimum value.
		@param mx The maximum value (no sanity checks are made).
		@return The wrapped value.*/
	template <class T, class T2, class T3>
	inline T Wrap(T val, T2 mn, T3 mx) { T d = (T)(mx - mn); while (val > (T)mx) val -= d; while (val < (T)mn) val += d; return val; }

	/** Template specializations of the Wrap method. Optimizing for known primitives.
		Feel free to add more supported primitives when needed.*/
	inline float Wrap(float val, float mn, float mx) { 
		if (val > mx) return (mn + fmodf(val - mx, mx - mn));
		else if (val < mn) return (mx - fmodf(mn - val, mx - mn));
		return val; }
	/** See above.*/
	inline double Wrap(double val, double mn, double mx) { 
		if (val > mx) return (mn + fmod(val - mx, mx - mn));
		else if (val < mn) return (mx - fmod(mn - val, mx - mn));
		return val; }
	/** See above.*/
	inline int Wrap(int val, int mn, int mx) { 
		if (val > mx) return mn + ((val - mx) % (mx - mn));
		else if (val < mn) return mx - ((mn - val) % (mx - mn));
		return val; }
	///** See above.*/
	//	inline int Wrap(unsigned long  val, unsigned long  mn, unsigned long  mx) { 
	//	if (val > mx) return mn + ((val - mx) % (mx - mn));
	//	else if (val < mn) return mx - ((mn - val) % (mx - mn));
	//	return val; }
	inline	float SquareRoot(const float& v) { return sqrtf(v); }
	inline	double SquareRoot(const double& v) { return sqrt(v); } /**< See above. */
	inline	int SquareRoot(const int& v) { return (int)sqrtf((float)v); } /**< See above. */

	inline	float Abs(const float& v) { return fabsf(v); }
	inline	double Abs(const double& v) { return fabs(v); } /**< See above. */
};
class Vector2
{
public:
	union
	{
		float u;	/**< The first coordinate. */
		float x;	/**< The first coordinate. */
	};
	union
	{
		float v;	/**< The second coordinate. */
		float y;	/**< The second coordinate. */
	};

public:
	/** Creates an empty Vector2. */
	#ifndef DEBUG
	Vector2() {}
	#else
	Vector2() { u = 123456789.0f; v = 123456789.0f; }
	#endif 

	/** Creates the vector with the coordinates given.
		@param _u The first coordinate.
		@param _v The second coordinate. */
	Vector2(float _u, float _v) { u = _u; v = _v; }
	Vector2(float* e_pfValue) { u = e_pfValue[0]; v = e_pfValue[1]; }
	Vector2(double _u, double _v) { u = (float)_u; v = (float)_v; }
	Vector2(int _u, int _v) { u = (float)_u; v = (float)_v; }
	Vector2(long _u, long _v) { u = (float)_u; v = (float)_v; }
	Vector2(float _u, int _v) { u = _u; v = (float)_v; }
	Vector2(int _u, float _v) { u = (float)_u; v = _v; }
	//format as below "0.5,0.5"
	Vector2(const char*e_str);
	Vector2(const wchar_t*e_str);
	/**	Retrieves the squared length of the vector.
		@return The squared length of this vector. */
	inline float LengthSquared() const { return x * x + y * y; }
	inline float Distance(){ return y-x; }
	float Rand();

	/**	Retrieves the length of the vector.
		@return The length of this vector. */
	inline float Length() const { return sqrtf(x * x + y * y); }

	/** Normalizes this vector. */
	inline void NormalizeIt() { float l = Length(); if (l > 0.0f) { x /= l; y /= l; } else { x = 0; y = 1; }}

	/** Get a normalized vector with the same direction as this vector.
		@return A vector with length 1 and same direction as this vector. */
	inline Vector2 Normalize() const { float l = Length(); return (l > 0.0f) ? Vector2(x / l, y / l) : Vector2::XAxis; }

	/** Get this vector as an array of \c floats.
		@return The \c float array. */
	inline operator float*() { return &u; }

	/** Adds two vector.
		Adds to this vector's coordinates the individual components of the
		given vector and returns this vector.
		@param a The vector to add with this one.
		@return This vector. */
	inline Vector2& operator +=(const Vector2& a) { u += a.u; v += a.v; return *this; }

	/** Multiplies this vector by a scaler.
		Multiplies each of this vector's coordinates with the scaler and
		returns this vector.
		@param a The scalar to multiply with.
		@return This vector. */
	inline Vector2& operator *=(float a) { u *= a; v *= a; return *this; }

	inline Vector2& operator /=(float a) { u /= a; v /= a; return *this; }

	inline Vector2& operator -=(float a) { u -= a; v -= a; return *this; }

	/** Assign this vector to the given float array.
		Assigns each coordinate of this vector to the elements in the \c
		float array. The first element to the first coordinate and the second to
		the second. It returns this vector.
		@param f The \c float array to assign with.
		@return This vector. */
	inline Vector2& operator =(const float* f) { u = *f; v = *(f + 1); return *this; }

	inline void	ToMin() { x = y = -FLT_MIN;}
	inline void	ToMax() { x = y = FLT_MAX;}
	/** Update each component of this FMVector to the minimum of two Vector3s.
		Updates each of the three components to be the minimum of the current
		value and that of the corresponding value of the given Vector3.
		@param min The FMVector to take values from. */
	inline void ComponentMinimum(const Vector2& min) { if (x > min.x) x = min.x; if (y > min.y) y = min.y;  }

	/** Retrieves the smallest component of the vector.
		@return The smallest component of the vector. */
	inline float ComponentMinimum() const { return min(fabsf(x),fabsf(y)); }

	/** Update each component of this FMVector to the maximum of two Vector3s.
		Updates each of the three components to be the maximum of the current
		value and that of the corresponding value of the given Vector3.
		@param max The FMVector to take values from. */
	inline void ComponentMaximum(const Vector2& max) { if (x < max.x) x = max.x; if (y < max.y) y = max.y;}

	/** Retrieves the largest component of the vector.
		@return The largest component of the vector. */
	inline float ComponentMaximum() const { return max(fabsf(x), fabsf(y)); }

	inline Vector2 Abs(){ return Vector2(fabs(x),fabs(y)); }

public:
	static const Vector2 Zero; /**< The zero vector. */
	static const Vector2 Origin; /**< The zero vector. */
	static const Vector2 XAxis; /**< The 2D X axis. */
	static const Vector2 YAxis; /**< The 2D Y axis. */
};

/** Vector addition with two vectors.
	@param a The first vector.
	@param b The second vector.
	@return The Vector2 representation of the resulting vector. */
inline Vector2 operator + (const Vector2& a, const Vector2& b) { return Vector2(a.u + b.u, a.v + b.v); }

/** Vector subtraction with two vectors.
	@param a The first vector.
	@param b The second vector.
	@return The Vector2 representation of the resulting vector. */
inline Vector2 operator -(const Vector2& a, const Vector2& b) { return Vector2(a.u - b.u, a.v - b.v); }

/** Dot product of two vectors.
	@param a The first vector.
	@param b The second vector.
	@return The result of the dot product.
 */
inline float operator *(const Vector2& a, const Vector2& b) { return a.u * b.u + a.v * b.v; }

/** Scalar multiplication with a vector.
	@param a The vector.
	@param b The scalar.
	@return The Vector2 representing the resulting the vector. */
inline Vector2 operator *(const Vector2& a, float b) { return Vector2(a.u * b, a.v * b); }

/**	Scalar multiplication with a vector.
	@param a The scalar.
	@param b The vector.
	@return The vector representing the resulting the vector. */
inline Vector2 operator *(float a, const Vector2& b) { return Vector2(a * b.u, a * b.v); }

/** Scalar divison with a vector.
	@param a The vector.
	@param b The scalar.
	@return The vector representing the resulting the vector. */
inline Vector2 operator /(const Vector2& a, float b) { return Vector2(a.x / b, a.y / b); }

inline Vector2 operator /(const Vector2& a, const Vector2& b) { return Vector2(a.x / b.x, a.y / b.y); }

/** Retrieves whether two 2D vectors are equivalent.
	@param a A first vector.
	@param b A second vector.
	@return Whether the two given 2D vectors are equivalent. */
inline bool IsEquivalent(const Vector2& a, const Vector2& b) { return IsEquivalent(a.x, b.x) && IsEquivalent(a.y, b.y); }
inline bool operator==(const Vector2& a, const Vector2& b) { return IsEquivalent(a.x, b.x) && IsEquivalent(a.y, b.y); } /**< See above. */

/** A dynamically-sized array of 2D vectors or points. */
typedef vector<Vector2> Vector2List;
inline	float	LengthByTwoVector(Vector2 e_P1,Vector2 e_P2){ return (e_P2-e_P1).Length();  }
inline Vector2	Vector2Lerp(Vector2 e_vStartPos,Vector2 e_vDestPos,float e_fLERPValue){return ((e_vDestPos-e_vStartPos)*e_fLERPValue)+e_vStartPos;}
#endif // _FM_VECTOR2_H_
