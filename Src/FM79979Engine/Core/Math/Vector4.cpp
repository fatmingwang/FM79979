#include "Vector4.h"
#include "../Common/StringToStructure.h"
#include "../Common/Utility.h"


const Vector4 Vector4::Zero(0.0f, 0.0f, 0.0f, 0.0f);
const Vector4 Vector4::One(1.0f, 1.0f, 1.0f, 1.0f);
const Vector4 Vector4::AlphaOne(0.0f, 0.0f, 0.0f, 1.0f);
const Vector4 Vector4::Red(1.f,0.f,0.f,1.f);
const Vector4 Vector4::Green(0.f,1.f,0.f,1.f);
const Vector4 Vector4::Blue(0.f,0.f,1.f,1.f);
const Vector4 Vector4::HalfDark(0.5f,0.5f,0.5f,1.f);
const Vector4 Vector4::HalfBright(1.5f,1.5f,1.5f,1.f);
const Vector4 Vector4::HalfBrightWithHalfAlpha(0.5f,0.5f,0.5f,0.5f);
const Vector4 Vector4::Yellow(1.f, 1.f, 0.f, 1.f);



Vector4::Vector4(int e_Color)
{
	this->x = (e_Color&0xff000000)/255.f;
	this->y = (e_Color&0x00ff0000)/255.f;
	this->z = (e_Color&0x0000ff00)/255.f;
	this->w = (e_Color&0x000000ff)/255.f;
}

Vector4::Vector4(const char*e_str)
{
	*this = FATMING_CORE::GetVector4(e_str);
}
Vector4::Vector4(const wchar_t*e_str)
{
	*this = Vector4(UT::WcharToChar(e_str).c_str());
}

Vector4 VectorNearEqual(Vector4 V1,Vector4 V2,Vector4 Epsilon)
{

    float fDeltax, fDeltay, fDeltaz, fDeltaw;
    Vector4 Control;

    fDeltax = V1.x-V2.x;
    fDeltay = V1.y-V2.y;
    fDeltaz = V1.z-V2.z;
    fDeltaw = V1.w-V2.w;

    fDeltax = fabsf(fDeltax);
    fDeltay = fabsf(fDeltay);
    fDeltaz = fabsf(fDeltaz);
    fDeltaw = fabsf(fDeltaw);

    Control.u[0] = (fDeltax <= Epsilon.x) ? 0xFFFFFFFFU : 0;
    Control.u[1] = (fDeltay <= Epsilon.y) ? 0xFFFFFFFFU : 0;
    Control.u[2] = (fDeltaz <= Epsilon.z) ? 0xFFFFFFFFU : 0;
    Control.u[3] = (fDeltaw <= Epsilon.w) ? 0xFFFFFFFFU : 0;

    return Control;
}

bool Vector4NearEqual(Vector4 V1,Vector4 V2,Vector4 Epsilon)
{
    float dx, dy, dz, dw;

    dx = fabsf(V1.x-V2.x);
    dy = fabsf(V1.y-V2.y);
    dz = fabsf(V1.z-V2.z);
    dw = fabsf(V1.w-V2.w);
    return (((dx <= Epsilon.x) &&
            (dy <= Epsilon.y) &&
            (dz <= Epsilon.z) &&
            (dw <= Epsilon.w)) != 0);
}
//is vector a number?
//http://en.wikipedia.org/wiki/NaN
bool Vector4IsNaN(Vector4 V)
{
    return (ISNAN(V.x) ||
            ISNAN(V.y) ||
            ISNAN(V.z) ||
            ISNAN(V.w));
}
//is vector infinitine?
bool Vector4IsInf(Vector4 V)
{
return (ISINF(V.x) ||
        ISINF(V.y) ||
        ISINF(V.z) ||
        ISINF(V.w));
}
///** A dynamically-sized array of 4D vectors or points. */
//typedef fm::vector<Vector4> Vector4List;
Vector4 VectorReciprocalEst(Vector4 V)
{
    Vector4 Result;
    unsigned int	i;
    for (i = 0; i < 4; i++)
    {
        if (ISINF(V.v[i]))
        {
            Result.v[i] = (V.v[i] < 0.0f) ? -0.0f : 0.0f;
        }
        else if (V.v[i] == -0.0f)
        {
            Result.u[i] = 0xFF800000;
        }
        else if (V.v[i] == 0.0f)
        {
            Result.u[i] = 0x7F800000;
        }
        else
        {
            Result.v[i] = 1.0f / V.v[i];
        }
    }

    return Result;
}


Vector4 Vector4Select
(
    Vector4 V1, 
    Vector4 V2, 
    Vector4 Control
)
{
    Vector4 Result;

    Result.u[0] = (V1.u[0] & ~Control.u[0]) | (V2.u[0] & Control.u[0]);
    Result.u[1] = (V1.u[1] & ~Control.u[1]) | (V2.u[1] & Control.u[1]);
    Result.u[2] = (V1.u[2] & ~Control.u[2]) | (V2.u[2] & Control.u[2]);
    Result.u[3] = (V1.u[3] & ~Control.u[3]) | (V2.u[3] & Control.u[3]);

    return Result;
}

Vector4 Vector4Cross(Vector4 V1,Vector4 V2,Vector4 V3)
{
    Vector4 Result;
    Result.x = (((V2.z*V3.w)-(V2.w*V3.z))*V1.y)-(((V2.y*V3.w)-(V2.w*V3.y))*V1.z)+(((V2.y*V3.z)-(V2.z*V3.y))*V1.w);
    Result.y = (((V2.w*V3.z)-(V2.z*V3.w))*V1.x)-(((V2.w*V3.x)-(V2.x*V3.w))*V1.z)+(((V2.z*V3.x)-(V2.x*V3.z))*V1.w);
    Result.z = (((V2.y*V3.w)-(V2.w*V3.y))*V1.x)-(((V2.x*V3.w)-(V2.w*V3.x))*V1.y)+(((V2.x*V3.y)-(V2.y*V3.x))*V1.w);
    Result.w = (((V2.z*V3.y)-(V2.y*V3.z))*V1.x)-(((V2.z*V3.x)-(V2.x*V3.z))*V1.y)+(((V2.y*V3.x)-(V2.x*V3.y))*V1.z);
    return Result;
}

Vector4 Vector4Dot(Vector4 V1,Vector4 V2)
{
    Vector4 Result;

    Result.v[0] =
    Result.v[1] =
    Result.v[2] =
    Result.v[3] = V1.v[0] * V2.v[0] + V1.v[1] * V2.v[1] + V1.v[2] * V2.v[2] + V1.v[3] * V2.v[3];
    return Result;
}

Vector4 Vector3Dot(Vector3 V1,Vector3 V2)
{
    float fValue = V1*V2;
    Vector4 vResult(fValue,fValue,fValue,fValue);
    return vResult;
}


Vector4 Vector4LengthSq(Vector4 V)
{
    return Vector4Dot(V, V);
}

Vector4 VectorReciprocalSqrtEst(Vector4 V)
{
    Vector4 Result;
    unsigned int  i;

    // Avoid C4701
    Result.v[0] = 0.0f;

    for (i = 0; i < 4; i++)
    {
        if (V.v[i] == 0.0f)
        {
            Result.u[i] = 0x7F800000;
        }
        else if (V.v[i] == -0.0f)
        {
            Result.u[i] = 0xFF800000;
        }
        else if (V.v[i] < 0.0f)
        {
            Result.u[i] = 0x7FFFFFFF;
        }
        else if (ISINF(V.v[i]))
        {
            Result.v[i] = 0.0f;
        }
        else
        {
            Result.v[i] = 1.0f / sqrtf(V.v[i]);
        }
    }

    return Result;
}

Vector4 VectorEqualInt(Vector4	V1,Vector4 V2)
{
    Vector4 Control;
    Control.u[0] = (V1.u[0] == V2.u[0]) ? 0xFFFFFFFF : 0;
    Control.u[1] = (V1.u[1] == V2.u[1]) ? 0xFFFFFFFF : 0;
    Control.u[2] = (V1.u[2] == V2.u[2]) ? 0xFFFFFFFF : 0;
    Control.u[3] = (V1.u[3] == V2.u[3]) ? 0xFFFFFFFF : 0;

    return Control;
}

Vector4	VectorEqual(Vector4 V1,Vector4 V2)
{
    Vector4	Control;

    Control.u[0] = (V1.v[0] == V2.v[0]) ? 0xFFFFFFFF : 0;
    Control.u[1] = (V1.v[1] == V2.v[1]) ? 0xFFFFFFFF : 0;
    Control.u[2] = (V1.v[2] == V2.v[2]) ? 0xFFFFFFFF : 0;
    Control.u[3] = (V1.v[3] == V2.v[3]) ? 0xFFFFFFFF : 0;

    return Control;
}

Vector4 VectorSqrt(Vector4 V)
{
    Vector4 Zero;
    Vector4 VEqualsInfinity, VEqualsZero;
    Vector4 Select;
    Vector4 Result;

    // if (x == +Infinity)  sqrt(x) = +Infinity
    // if (x == +0.0f)      sqrt(x) = +0.0f
    // if (x == -0.0f)      sqrt(x) = -0.0f
    // if (x < -0.0f)       sqrt(x) = QNaN

    Result = VectorReciprocalSqrtEst(V);
	Zero = Vector4::Zero;
    VEqualsInfinity = VectorEqualInt(V, g_XMInfinity.v);
    VEqualsZero = VectorEqual(V, Zero);
    Result = Vector4Multiply(V, Result);
    Select = VectorEqualInt(VEqualsInfinity, VEqualsZero);
    Result = Vector4Select(V, Result, Select);

    return Result;
}

Vector4 Vector4Length(Vector4 V)
{
    Vector4 Result;
    Result = Vector4LengthSq(V);
    Result = VectorSqrt(Result);
    return Result;
}

Vector4 Vector3Cross(Vector4 V1,Vector4 V2)
{
    Vector4 vResult = Vector4(
        (V1.y * V2.z) - (V1.z * V2.y),
        (V1.z * V2.x) - (V1.x * V2.z),
        (V1.x * V2.y) - (V1.y * V2.x),
        0.0f);
    return vResult;
}

Vector4 VectorSplatX(Vector4 V)
{
    Vector4 vResult;
    vResult.x = V.x;
    vResult.y = V.x;
    vResult.z = V.x;
    vResult.w = V.x;
    return vResult;
}

Vector4 VectorSplatY(Vector4 V)
{
    Vector4 vResult;
    vResult.x = V.y;
    vResult.y = V.y;
    vResult.z = V.y;
    vResult.w = V.y;
    return vResult;
}

Vector4 VectorSplatZ(Vector4 V)
{
    Vector4 vResult;
    vResult.x = V.z;
    vResult.y = V.z;
    vResult.z = V.z;
    vResult.w = V.z;
    return vResult;
}

Vector4 VectorSplatW(Vector4 V)
{
    Vector4 vResult;
    vResult.x = V.w;
    vResult.y = V.w;
    vResult.z = V.w;
    vResult.w = V.w;
    return vResult;
}

Vector4	VectorReciprocal(Vector4	V)
{
    return VectorReciprocalEst(V);
}

bool	Vector4LessOrEqual(Vector4 V1,Vector4 V2)
{
    return (((V1.x <= V2.x) && (V1.y <= V2.y) && (V1.z <= V2.z) && (V1.w <= V2.w)) != 0);
}

Vector4	VectorLessOrEqual(Vector4	V1,Vector4	V2)
{
    Vector4	Control;
    Control.u[0] = (V1.v[0] <= V2.v[0]) ? 0xFFFFFFFF : 0;
    Control.u[1] = (V1.v[1] <= V2.v[1]) ? 0xFFFFFFFF : 0;
    Control.u[2] = (V1.v[2] <= V2.v[2]) ? 0xFFFFFFFF : 0;
    Control.u[3] = (V1.v[3] <= V2.v[3]) ? 0xFFFFFFFF : 0;
    return Control;
}

Vector4	Vector3Normalize(Vector4 V)
{
    float fLengthSq;
    Vector4 vResult;

    fLengthSq = sqrtf((V.x*V.x)+(V.y*V.y)+(V.z*V.z));
    // Prevent divide by zero
    if (fLengthSq) {
        fLengthSq = 1.0f/fLengthSq;
    }
    
    vResult.x = V.x*fLengthSq;
    vResult.y = V.y*fLengthSq;
    vResult.z = V.z*fLengthSq;
    vResult.w = V.w*fLengthSq;
    return vResult;
}

Vector4 Vector3LengthSq(Vector3 V)
{
    return Vector3Dot(V, V);
}

Vector4 VectorSqrtEst(Vector4 V)
{
    Vector4 Select;

    Vector4 Result = VectorReciprocalSqrtEst(V);
	Vector4 Zero = Vector4::Zero;
    Vector4 VEqualsInfinity = VectorEqualInt(V, g_XMInfinity.v);
    Vector4 VEqualsZero = VectorEqual(V, Zero);
    Result = Vector4Multiply(V, Result);
    Select = VectorEqualInt(VEqualsInfinity, VEqualsZero);
    Result = Vector4Select(V, Result, Select);
    return Result;
}


Vector3 Vector3LengthEst(Vector3 V)
{

    Vector4 Result;

    Result = Vector3LengthSq(V);
    Result = VectorSqrtEst(Result);

    return Result;
}

Vector4 VectorMax(Vector4 V1,Vector4 V2)
{
	return Vector4(V1.x>V2.x?V1.x:V2.x,
		V1.y>V2.y?V1.y:V2.y,
		V1.z>V2.z?V1.z:V2.z,
		V1.w>V2.w?V1.w:V2.w);
}