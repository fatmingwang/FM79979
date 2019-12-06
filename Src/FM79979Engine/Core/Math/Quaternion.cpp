/*
	Copyright (C) 2005-2009 Feeling Software Inc.
	Available only to licensees.
	Distribution of this file or its content is strictly prohibited.
*/

#include "Quaternion.h"
#include <float.h>
#include "../Common/CommonDefine.h"
//
// Force templatization of inlines
//
const Quaternion Quaternion::Zero = Quaternion((float) 0.0, (float) 0.0, (float) 0.0, (float) 0.0);
const Quaternion Quaternion::Identity = Quaternion((float) 0.0, (float) 0.0, (float) 0.0, (float) 1.0);

static void FMQuaternion_AngleApproach(float pval, float& val)
{
	while (val - pval > (float) MyMath::Pi) val -= (float) MyMath::TwoPi;
	while (val - pval < -(float) MyMath::Pi) val += (float) MyMath::TwoPi;
}

static void FMQuaternion_PatchEuler(float* pval, float* val)
{
	// Approach these Euler angles to the previous value.
	for (size_t i = 0; i < 3; ++i) FMQuaternion_AngleApproach(pval[i], val[i]);
	float distanceSq = (float) 0;
	for (size_t i = 0; i < 3; ++i) distanceSq += (val[i] - pval[i]) * (val[i] - pval[i]);

	// All quaternions can be expressed two ways. Check if the second way is better.
	float alternative[3];
	if (IsEquivalent(MyMath::Abs(val[1]), (float) MyMath::PiOverTwo))
	{
		// Compute the closest alternative, considering the particularities of the Gimbel Lock.
		float alpha = ((pval[0] - val[0]) + (pval[2] - val[2])) / (float) 2;
		alternative[0] = val[0] + alpha; 
		alternative[1] = val[1]; 
		alternative[2] = val[2] + MyMath::Sign(val[1]) * alpha;
	}
	else
	{
		// There is always two sets of Euler angles that represent the same rotation.
		alternative[0] = val[0] + MyMath::Pif;
		alternative[1] = MyMath::Pif - val[1];
		alternative[2] = val[2] + MyMath::Pif;
	}

	// Check the distance of the alternative.
	float alternateDistanceSq = (float) 0; 
	for (size_t i = 0; i < 3; ++i) 
	{
		FMQuaternion_AngleApproach(pval[i], alternative[i]);
		alternateDistanceSq += (alternative[i] - pval[i]) * (alternative[i] - pval[i]);
	}
	if (alternateDistanceSq < distanceSq)
	{
		// Pick the alternative
		for (size_t i = 0; i < 3; ++i) val[i] = alternative[i];
	}
}

//
// Quaternion
//


Quaternion::Quaternion(const float* values)
{
	if (values != nullptr)
	{
		x = (*values++);
		y = (*values++);
		z = (*values++);
		w = (*values);
	}
}


Quaternion::Quaternion(const Vector3& axis, const float& angle)
{
	float s = sinf(angle / (float) 2);
	x = axis.x * s;
	y = axis.y * s;
	z = axis.z * s;
	w = cosf(angle / (float) 2);
}


Quaternion Quaternion::operator*(const Quaternion& q) const
{
	Quaternion r;
	r.x = w * q.x + x * q.w + y * q.z - z * q.y;
	r.y = w * q.y + y * q.w + z * q.x - x * q.z;
	r.z = w * q.z + z * q.w + x * q.y - y * q.x;
	r.w = w * q.w - x * q.x - y * q.y - z * q.z;
	return r;
}


Vector3 Quaternion::operator*(const Vector3& v) const
{
	/*
	for a quicker way to do this see: http://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation
	given:
	quaternion z = a + bi + cj + dk (with |z| = 1) and a 
	vector v with elements v1, v2, and v3

	t2 =   a*b
	t3 =   a*c
	t4 =   a*d
	t5 =  -b*b
	t6 =   b*c
	t7 =   b*d
	t8 =  -c*c
	t9 =   c*d
	t10 = -d*d
	v1new = 2*((t8 + t10)*v1 + (t6 -  t4)*v2 + (t3 + t7)*v3) + v1
	v2new = 2*((t4 +  t6)*v1 + (t5 + t10)*v2 + (t9 - t2)*v3) + v2
	v3new = 2*((t7 -  t3)*v1 + (t2 +  t9)*v2 + (t5 + t8)*v3) + v3
	*/
	Quaternion out = ~(*this) * Quaternion(v.x, v.y, v.z, (float) 0) * (*this);
	return Vector3(out.x, out.y, out.z);
}


Quaternion Quaternion::Average(const Quaternion& _other, const float& t) const
{
	Quaternion other = _other;
	float dot = x * other.x + y * other.y + z * other.z + w * other.w;
	if (dot < (float) 0)
	{
		other.x *= -1; other.y *= -1; other.z *= -1; other.w *= -1;
		dot = x * other.x + y * other.y + z * other.z + w * other.w;
	}
	if (dot >= (float) 1) return *this;

	// algorithm taken from Shoemake's paper
	float half_theta = cosf(dot);
	if (half_theta == (float) 0) return *this;

	float st = sinf(half_theta);
	float sut = sinf(t * half_theta);
	float sout = sinf((1 - t) * half_theta);
	float interp1 = sout / st;
	float interp2 = sut / st;

	Quaternion result;
	result.x = interp1*x + interp2*other.x;
	result.y = interp1*y + interp2*other.y;
	result.z = interp1*z + interp2*other.z;
	result.w = interp1*w + interp2*other.w;

	result.NormalizeIt();
	return result;
}


void Quaternion::ToAngleAxis(Vector3& axis, float& angle) const
{
	angle = ((float) 2) * acosf(w);
	float s = asinf(angle / ((float) 2));
	if (!IsEquivalent(s, ((float) 0)))
	{
		axis.x = x / s;
		axis.y = y / s;
		axis.z = z / s;
		axis.NormalizeIt();
	}
	else
	{
		// If s == 0, then angle == 0 and there is no rotation: assign any axis.
		axis = Vector3::XAxis;
	}
}


Vector3 Quaternion::ToEulerWithDegree(const Vector3* previousAngles) const
{
	Vector3 l_vAngle = ToEuler(previousAngles);
	l_vAngle.x = D3DXToDegree(l_vAngle.x);
	l_vAngle.y = D3DXToDegree(l_vAngle.y);
	l_vAngle.z = D3DXToDegree(l_vAngle.z);
	return l_vAngle;
}

Vector3 Quaternion::ToEuler(const Vector3* previousAngles) const
{
	Vector3 angles;

	// Convert the quaternion into Euler angles.
	float siny = ((float) 2) * (x * z + y * w);
	if (siny > ((float) 1) - FLT_TOLERANCE) // singularity at north pole
	{ 
		angles.x = ((float) 2) * atan2f(x, w);
		angles.y = (float) MyMath::PiOverTwo;
		angles.z = ((float) 0);
	}
	else if (siny < ((float) -1) + FLT_TOLERANCE) // singularity at south pole
	{
		angles.x = ((float) -2) * atan2f(x, w);
		angles.y = (float) -MyMath::PiOverTwo;
		angles.z = ((float) 0);
	}
	else
	{
		// [GLaforte] Derived on 18-07-2007.
		angles.y = asinf(siny);
		angles.x = atan2f(((float) 2) * (x*w-y*z), ((float) 1) - ((float) 2) * (x*x+y*y));
		angles.z = atan2f(((float) 2) * (z*w-x*y), ((float) 1) - ((float) 2) * (y*y+z*z));
	}

	// Patch to the closest Euler angles.
	if (previousAngles != nullptr)
	{
		FMQuaternion_PatchEuler((float*) previousAngles, (float*) angles);
	}
	return angles;
}


bool Quaternion::operator==(const Quaternion& b) const
{
	// [GLaforte 04-04-2008] Quaternion duality implies two quaternion for one real rotation.
	// See page 168 in notebook for derivation.s
	return (IsEquivalent(x, b.x) && IsEquivalent(y, b.y) && IsEquivalent(z, b.z) && IsEquivalent(w, b.w))
	|| (IsEquivalent(x, -b.x) && IsEquivalent(y, -b.y) && IsEquivalent(z, -b.z) && IsEquivalent(w, -b.w)); 
}


void Quaternion::SetToMatrix(cMatrix44& m) const
{
	float s, xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz, den;

	if (*this == Quaternion::Identity) 
	{
		m = cMatrix44::Identity;
		return;		
	}
	// For unit q, just set s = 2.0; or or set xs = q.x + q.x, etc 
	den =  (x*x + y*y + z*z + w*w);
	if (den == (float) 0) {  s = (float) 1; }
	else s = ((float) 2) / den;

	xs = x * s;   ys = y * s;  zs = z * s;
	wx = w * xs;  wy = w * ys; wz = w * zs;
	xx = x * xs;  xy = x * ys; xz = x * zs;
	yy = y * ys;  yz = y * zs; zz = z * zs;

	m[0][0] = ((float) 1) - (yy +zz);
	m[1][0] = xy - wz; 
	m[2][0] = xz + wy; 

	m[0][1] = xy + wz; 
	m[1][1] = ((float) 1) - (xx +zz);
	m[2][1] = yz - wx; 

	m[0][2] = xz - wy; 
	m[1][2] = yz + wx; 
	m[2][2] = ((float) 1) - (xx + yy);
}


cMatrix44 Quaternion::ToMatrix() const
{
	cMatrix44 tm = cMatrix44::Identity;
	SetToMatrix(tm);
	return tm;
}


Quaternion Quaternion::EulerRotationQuaternion(const float& x, const float& y, const float& z)
{
	Quaternion qx(Vector3::XAxis, x);
	Quaternion qy(Vector3::YAxis, y);
	Quaternion qz(Vector3::ZAxis, z);
	return qx * qy * qz;
}
Quaternion Quaternion::EulerRotationQuaternion(Vector3 e_v)
{
	Quaternion qx(Vector3::XAxis, e_v.x);
	Quaternion qy(Vector3::YAxis, e_v.y);
	Quaternion qz(Vector3::ZAxis, e_v.z);
	return qx * qy * qz;
}

Quaternion::Quaternion(const cMatrix44& mat)
{
	float tr,s;
	//A rotation may be converted back to a quaternion through the use of
	//the following algorithm:
	//The process is performed in the following stages, which are as follows:
	//  Calculate the trace of the matrix T from the equation:
	//              2     2     2
	//    T = 4 - 4x  - 4y  - 4z
	//               2    2    2
	//      = 4( 1 -x  - y  - z )
	//      = mat[0] + mat[5] + mat[10] + 1
	//If the trace of the matrix is greater than zero, then
	//perform an "instant" calculation.
	//  S = 0.5 / sqrt(T)
	//  W = 0.25 / S
	//  X = ( mat[9] - mat[6] ) * S
	//  Y = ( mat[2] - mat[8] ) * S
	//  Z = ( mat[4] - mat[1] ) * S
	//If the trace of the matrix is less than or equal to zero
	//then identify which major diagonal element has the greatest
	//value.
	//Depending on this value, calculate the following:
	//  Column 0:
	//	S  = sqrt( 1.0 + mr[0] - mr[5] - mr[10] ) * 2;
	//	Qx = 0.5 / S;
	//	Qy = (mr[1] + mr[4] ) / S;
	//	Qz = (mr[2] + mr[8] ) / S;
	//	Qw = (mr[6] + mr[9] ) / S;
	//  Column 1:
	//	S  = sqrt( 1.0 + mr[5] - mr[0] - mr[10] ) * 2;
	//	Qx = (mr[1] + mr[4] ) / S;
	//	Qy = 0.5 / S;
	//	Qz = (mr[6] + mr[9] ) / S;
	//	Qw = (mr[2] + mr[8] ) / S;
	//  Column 2:
	//	S  = sqrt( 1.0 + mr[10] - mr[0] - mr[5] ) * 2;
	//	Qx = (mr[2] + mr[8] ) / S;
	//	Qy = (mr[6] + mr[9] ) / S;
	//	Qz = 0.5 / S;
	//	Qw = (mr[1] + mr[4] ) / S;
	// The quaternion is then defined as:

	tr = ((float) 1) + mat[0][0] + mat[1][1] + mat[2][2];
	if (tr > FLT_EPSILON)
	{
		s = MyMath::SquareRoot(tr) * ((float) 2);
		x = (mat[1][2] - mat[2][1]) / s;
		y = (mat[2][0] - mat[0][2]) / s;
		z = (mat[0][1] - mat[1][0]) / s;
		w = s * ((float) 0.25);
	}
	else if (mat[0][0] > mat[1][1])
	{
		s = MyMath::SquareRoot((float) 1 + mat[0][0] - mat[1][1] - mat[2][2]) * ((float) 2);
		x = ((float) 0.25) * s;
		y = (mat[0][1] + mat[1][0]) / s;
		z = (mat[2][0] + mat[0][2]) / s;
		w = (mat[1][2] - mat[2][1]) / s;
	}
	else if (mat[1][1] > mat[2][2])
	{
		s = MyMath::SquareRoot((float) 1 + mat[1][1] - mat[0][0] - mat[2][2]) * ((float) 2);
		x = (mat[0][1] + mat[1][0]) / s;
		y = ((float) 0.25) * s;
		z = (mat[1][2] + mat[2][1]) / s;
		w = (mat[2][0] - mat[0][2]) / s;
	}
	else
	{
		s  = MyMath::SquareRoot((float) 1 + mat[2][2] - mat[0][0] - mat[1][1]) * ((float) 2);
		x = (mat[2][0] + mat[0][2]) / s;
		y = (mat[1][2] + mat[2][1]) / s;
		z = ((float) 0.25) * s;
		w = (mat[0][1] - mat[1][0]) / s;
	}
}


Quaternion Quaternion::MatrixRotationQuaternion(const cMatrix44& mat)
{
	return Quaternion(mat);
}


Quaternion Quaternion::LookAtQuaternion(const Vector3& eye, const Vector3& target, const Vector3& up)
{
	// Might want to optimize this, someday...
	cMatrix44 lookAtMatrix = cMatrix44::LookAtMatrix(eye, target, up);
	return Quaternion::MatrixRotationQuaternion(lookAtMatrix);
}

Vector3 operator*(const Vector3& v, const Quaternion& q)
{
	Quaternion out = q * Quaternion(v.x, v.y, v.z, 0.0f) * ~q;
	return Vector3(out.x, out.y, out.z);
}



//void TrickLinkerQuaternion()
//{
//	const float x[4] = { (float)0, (float) 0, (float) 1, (float) 0 }; float f = (float) 0.2;
//	Quaternion q, q1(x), q2(q), q3(0, 0, 0, 0), q4(Vector3::XAxis, f);
//	(const float*) const_cast<const Quaternion&>(q); (float*) q1; q2 = x; q3.NormalizeIt(); q4 = q3 * q2; q4 = ~q2; q1 *= q;
//	q1 = q2; Vector3 v = q1.ToEuler(); q3.Average(q4); q2.ToAngleAxis(v, f);
//	cMatrix44 m = q3.ToMatrix(); q3.SetToMatrix(m); bool b = q2 == q3;
//	v = q2 * v; v = v * q2; Quaternion::EulerRotationQuaternion(f, f, b ? f : (float) 0.2);
//	q = Quaternion::MatrixRotationQuaternion(m); q2 = Quaternion::LookAtQuaternion(v, v);
//	q1.Normalize();
//}
//
//extern void TrickLinkerFMQuaternion()
//{
//	TrickLinkerQuaternion();
//	TrickLinkerQuaternion();
//}