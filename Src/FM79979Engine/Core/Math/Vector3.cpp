#include "../stdafx.h"
//#include "Float.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "../XML/StringToStructure.h"
//
//// Vector constants
const Vector2 Vector2::Zero(0.0f, 0.0f);
const Vector2 Vector2::Origin = Vector2::Zero;
const Vector2 Vector2::XAxis(1.0f, 0.0f);
const Vector2 Vector2::YAxis(0.0f, 1.0f);
const Vector3 Vector3::Zero(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::XAxis(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::YAxis(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::ZAxis(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::Origin = Vector3::Zero;
const Vector3 Vector3::One(1.0f, 1.0f, 1.0f);

Vector2::Vector2(const char*e_str)
{
	*this = FATMING_CORE::GetVector2(e_str);
}
Vector2::Vector2(const wchar_t*e_str)
{
	*this = Vector2(UT::WcharToChar(e_str).c_str());
}

Vector3::Vector3(const char*e_str)
{
	*this = FATMING_CORE::GetVector3(e_str);
}
Vector3::Vector3(const wchar_t*e_str)
{
	*this = Vector3(UT::WcharToChar(e_str).c_str());
}

float Vector2::Rand()
{
	double fRange = y - x;
	double fVal = ( (double)::rand() * fRange ) / (double)RAND_MAX;
	return (float)fVal + x;
}

Vector3::Vector3(const Vector4& vect4)
:	x(vect4.x), y(vect4.y), z(vect4.z)
{
}

// Read in the vector from a source
Vector3::Vector3(const float* source, int startIndex)
{
	x = source[startIndex];
	y = source[startIndex + 1];
	z = source[startIndex + 2];
}

// Read in the vector from a source
Vector3::Vector3(const double* source, int startIndex)
{
	x = (float)source[startIndex];
	y = (float)source[startIndex + 1];
	z = (float)source[startIndex + 2];
}

Vector4 Vector4::FromHSVColor(float hue, float saturation, float value)
{
	// [GLaforte - 15-04-2007]
	// Algorithm inspired from http://www.cs.rit.edu/~ncs/color/t_convert.html
	// Written by Nan C. Schaller, Rochester Institute of Technology, Computer Science Department
	if (!IsEquivalent(saturation, 0.0f)) 
	{
		hue *= 6.0f;						// sector 0 to 5
		float sector = floor(hue);
		float f = hue - sector;				// factorial part of h
		float p = value * (1.0f - saturation);
		float q = value * (1.0f - saturation * f);
		float t = value * (1.0f - saturation * (1.0f - f));
		switch ((unsigned long) sector)
		{
			case 0: return Vector4(value, t, p, 1.0f);
			case 1: return Vector4(q, value, p, 1.0f);
			case 2: return Vector4(p, value, t, 1.0f);
			case 3: return Vector4(p, q, value, 1.0f);
			case 4: return Vector4(t, p, value, 1.0f);
			case 5:
			default: return Vector4(value, p, q, 1.0f);
		}
	}
	else return Vector4(value, value, value, 1.0f); // Achromatic (grey)
}

Vector3 Vector4::ToHSVColor()
{
	// [GLaforte - 15-04-2007]
	// Algorithm inspired from http://www.cs.rit.edu/~ncs/color/t_convert.html
	// Written by Nan C. Schaller, Rochester Institute of Technology, Computer Science Department

	Vector3 hsv;
	float smallest = min(min(x, y), z);
	float largest = max(max(x, y), z);
	hsv.z = largest;				// v
	float delta = largest - smallest;
	if (!IsEquivalent(largest, 0.0f))
	{
		hsv.y = delta / largest;	// s
		if (IsEquivalent(x, largest))			hsv.x = (y-z) / delta;			// between yellow & magenta
		else if (IsEquivalent(y, largest))		hsv.x = 2.0f + (z-x) / delta;	// between cyan & yellow
		else									hsv.x = 4.0f + (x-y) / delta;	// between magenta & cyan
		hsv.x /= 6.0f;				// convert to [0,1]
		if (hsv.x < 0.0f) hsv.x += 1.0f;
	}
	else hsv.x = hsv.y = 0; // black
	return hsv;
}
//===============================
//
//===============================
Vector3	Vector3Reflect
(
    Vector3	Incident, 
    Vector3	Normal
)
{
    Vector3	Result;

    // Result = Incident - (2 * dot(Incident, Normal)) * Normal
    float l_fDot = Incident*Normal*2;
    Result = Incident-(l_fDot*Normal);

    return Result;
}
//===============================
//
//===============================
	float	GetDirectionOffset(Vector3 e_vDir)
	{
		Vector3	l_vRight = Vector3(1.0f,0.0f,0.0f);
		Vector3	l_vDirection = Vector3(0.0f,0.0f,-1.0f);
		e_vDir.NormalizeIt();
		float	l_vAngle = e_vDir*l_vDirection;
		float	l_fAngle =  acos(l_vAngle);
		//test invisible if invisible *-1
		if( e_vDir*l_vRight<=EPSIONAL )
			l_fAngle = -l_fAngle;
		return l_fAngle;
	}
	float		GetDirectionOffset(Vector2 e_vDir)
	{
		Vector3	l_vDir(e_vDir.x,0,e_vDir.y);
		Vector3	l_vRight = Vector3(1.0f,0.0f,0.0f);
		Vector3	l_vDirection = Vector3(0.0f,0.0f,-1.0f);
		e_vDir.NormalizeIt();
		float	l_vAngle = l_vDir*l_vDirection;
		float	l_fAngle =  acos(l_vAngle);
		//test invisible if invisible *-1
		if( l_vDir*l_vRight<=EPSIONAL )
			l_fAngle = -l_fAngle;
		return l_fAngle;	
	}
//===============================
//
//===============================
	float	GetUpOffset(Vector3 e_vUp)
	{
		Vector3	l_vUp = Vector3(0.0f,1.0f,0.0f);
		Vector3	l_vDirection = Vector3(0.0f,0.0f,-1.0f);
		e_vUp.NormalizeIt();
		float	l_vAngle = e_vUp*l_vUp;
		float	l_fAngle =  acos(l_vAngle);
		//test invisible if invisible *-1
		if( e_vUp*l_vDirection<=EPSIONAL )
			l_fAngle = -l_fAngle;
		return l_fAngle;
	}


void CalculateTangentArray(long vertexCount, const float *vertex, const float *normal,
        const float *texcoord, long triangleCount, const unsigned int*triangleIndices,
		float *tangent,float *bitangents)
{
    float *tan1 = new float[vertexCount * 2* 3];
    float *tan2 = tan1 + (vertexCount*3);
	memset(tan1,0,sizeof(float)*vertexCount * 2* 3);
    
    for (long a = 0; a < triangleCount; a++)
    {
        long i1 = triangleIndices[0];
        long i2 = triangleIndices[1];
        long i3 = triangleIndices[2];
        
        const float *v1 = &vertex[i1*3];
        const float *v2 = &vertex[i2*3];
        const float *v3 = &vertex[i3*3];
        
        const float *w1 = &texcoord[i1*2];
        const float *w2 = &texcoord[i2*2];
        const float *w3 = &texcoord[i3*2];
        
        float x1 = v2[0] - v1[0];
        float x2 = v3[0] - v1[0];
        float y1 = v2[1] - v1[1];
        float y2 = v3[1] - v1[1];
        float z1 = v2[2] - v1[2];
        float z2 = v3[2] - v1[2];
        
        float s1 = w2[0] - w1[0];
        float s2 = w3[0] - w1[0];
        float t1 = w2[1] - w1[1];
        float t2 = w3[1] - w1[1];
        
        float r = 1.0F / (s1 * t2 - s2 * t1);
		float sdir[3] = 
		{
			(t2 * x1 - t1 * x2) * r,
			(t2 * y1 - t1 * y2) * r,
			(t2 * z1 - t1 * z2) * r	
		};

		float  tdir[3]  = 
		{
			(s1 * x2 - s2 * x1) * r,
			(s1 * y2 - s2 * y1) * r,
			(s1 * z2 - s2 * z1) * r
		};

		for( int i=0;i<3;++i )
		{
			tan1[i1+i] += sdir[i];
			tan1[i2+i] += sdir[i];
			tan1[i3+i] += sdir[i];
			tan2[i1+i] += tdir[i];
			tan2[i2+i] += tdir[i];
			tan2[i3+i] += tdir[i];
		}        
        triangleIndices+=3;//xyz skip
    }
    


    for (long a = 0; a < vertexCount; a++)
    {
		int	l_iIndex = a*3;
        Vector3 n(normal[l_iIndex],normal[l_iIndex+1],normal[l_iIndex+2]);
		Vector3 t(tan1[l_iIndex],tan1[l_iIndex+1],tan1[l_iIndex+2]);
        Vector3	l_vTangent((t - n * (n*t)).Normalize());
        // Gram-Schmidt orthogonalize
		tangent[l_iIndex] = l_vTangent.x;
		tangent[l_iIndex+1] = l_vTangent.y;
		tangent[l_iIndex+2] = l_vTangent.z;
		// Calculate handedness
		Vector3	l_vTan2(tan2[l_iIndex],tan2[l_iIndex+1],tan2[l_iIndex+2]);
		Vector3	l_vCross = n^t;
		float	l_fDot = l_vCross*l_vTan2;
        float	l_fTagentw = ( l_fDot < 0.0F) ? -1.0F : 1.0F;
        
		Vector3	l_vNormal(n[l_iIndex],n[l_iIndex+1],n[l_iIndex+2]);
		Vector3	l_vBiTangent(l_vNormal^l_vTangent*l_fTagentw);
		bitangents[l_iIndex] = l_vBiTangent.x;
		bitangents[l_iIndex+1] = l_vBiTangent.y;
		bitangents[l_iIndex+2] = l_vBiTangent.z;
    }
    
    delete[] tan1;
}

Vector2	VectorToInteger(Vector2 e_Vec3)
{
	e_Vec3.x = (float)(int)e_Vec3.x;
	e_Vec3.y = (float)(int)e_Vec3.y;
	return e_Vec3;
}
Vector3	VectorToInteger(Vector3 e_Vec3)
{
	e_Vec3.x = (float)(int)e_Vec3.x;
	e_Vec3.y = (float)(int)e_Vec3.y;
	e_Vec3.z = (float)(int)e_Vec3.z;
	return e_Vec3;
}
Vector4	VectorToInteger(Vector4 e_Vec3)
{
	e_Vec3.x = (float)(int)e_Vec3.x;
	e_Vec3.y = (float)(int)e_Vec3.y;
	e_Vec3.z = (float)(int)e_Vec3.z;
	e_Vec3.w = (float)(int)e_Vec3.w;
	return e_Vec3;
}

std::vector<Vector3>	Vector2ToVector3(std::vector<Vector2>*e_Vector2Vector,float e_fZValue)
{
	int	l_iNumPoint = (int)e_Vector2Vector->size();
	std::vector<Vector3>l_AllPositionData;l_AllPositionData.resize(l_iNumPoint);
	for(int i=0;i<l_iNumPoint;++i)
	{
		Vector2	l_vPos = (*e_Vector2Vector)[i];
		l_AllPositionData[i] = Vector3(l_vPos.x,l_vPos.y,e_fZValue);
	}
	return l_AllPositionData;
}