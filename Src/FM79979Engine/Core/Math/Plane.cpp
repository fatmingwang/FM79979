#include "../stdafx.h"
#include "Plane.h"
bool PlaneEqual
(
    Vector4 P1,
    Vector4 P2
)
{
    return P1==P2;
}

//------------------------------------------------------------------------------

bool PlaneNearEqual
(
    Vector4 P1,
    Vector4 P2,
    Vector4 Epsilon
)
{
    Vector4 NP1 = PlaneNormalize(P1);
    Vector4 NP2 = PlaneNormalize(P2);	
    return Vector4NearEqual(NP1, NP2, Epsilon);
}

//------------------------------------------------------------------------------

bool PlaneNotEqual
(
    Vector4 P1,
    Vector4 P2
)
{
    return (((P1.x != P2.x) || (P1.y != P2.y) || (P1.z != P2.z) || (P1.w != P2.w)) != 0);
}

//------------------------------------------------------------------------------

bool PlaneIsNaN
(
    Vector4 P
)
{
    return Vector4IsNaN(P);
}

//------------------------------------------------------------------------------

bool PlaneIsInfinite
(
    Vector4 P
)
{
    return Vector4IsInf(P);
}

//------------------------------------------------------------------------------
// Computation operations
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

float PlaneDot(Vector4 P,Vector4 V)
{

    return P*V;
}

//------------------------------------------------------------------------------

float PlaneDotNormal(Vector4 P,Vector4 V)
{
    return P*V;
}

//------------------------------------------------------------------------------
// PlaneNormalizeEst uses a reciprocal estimate and
// returns QNaN on zero and infinite vectors.

//Vector4 PlaneNormalizeEst
//(
//    Vector4 P
//)
//{
//    Vector4 Result;
//    Result = Vector43ReciprocalLength(P);
//    Result = Vector4Multiply(P, Result);
//    return Result;
//}

//------------------------------------------------------------------------------

Vector4 PlaneNormalize(Vector4 P)
{

    FLOAT fLengthSq = sqrtf((P.x*P.x)+(P.y*P.y)+(P.z*P.z));
    // Prevent divide by zero
    if (fLengthSq)
	{
        fLengthSq = 1.0f/fLengthSq;
    }
    Vector4 vResult = Vector4(P.x*fLengthSq,P.y*fLengthSq,P.z*fLengthSq,P.w*fLengthSq);
    return vResult;
}

float	PlaneDotCoord(Vector4 P, Vector4 V)
{

    //XMVECTOR V3;
    //XMVECTOR Result;
    //V3 = XMVectorSelect(g_XMOne.v, V, g_XMSelect1110.v);
    //Result = XMVector4Dot(P, V3);
	// Result = P[0] * V[0] + P[1] * V[1] + P[2] * V[2] + P[3]
	return P.x*V.x+P.y*V.y+P.z*V.z;
}
//------------------------------------------------------------------------------

Vector4 PlaneIntersectLine
(
    Vector4 P,
    Vector4 LinePoint1,
    Vector4 LinePoint2
)
{
    Vector4 V1;
    Vector4 V2;
    Vector4 D;
    Vector4 ReciprocalD;
    Vector4 VT;
    Vector4 Point;
    Vector4 Zero;
    Vector4 Control;
    Vector4 Result;
    V1 = Vector4Multiply(P,LinePoint1);
	V2 = Vector4Multiply(P,LinePoint2);
    D = V1-V2;

    ReciprocalD = VectorReciprocalEst(D);
	float	l_fVT = PlaneDotCoord(P, LinePoint1);
    VT = Vector4(l_fVT,l_fVT,l_fVT,l_fVT);
	float	l_fVTReciprocalD = VT*ReciprocalD;
    VT = Vector4(l_fVTReciprocalD,l_fVTReciprocalD,l_fVTReciprocalD,l_fVTReciprocalD);

    Point = LinePoint2-LinePoint1;
    Point = Vector4Multiply(Point,VT)+LinePoint1;

	Zero = Vector4::Zero;
    Control = VectorNearEqual(D, Zero, g_XMEpsilon.v);

    Result = Vector4Select(Point, g_XMQNaN.v, Control);

    return Result;
}

//------------------------------------------------------------------------------

void PlaneIntersectPlane(Vector4* pLinePoint1,Vector4* pLinePoint2,Vector4  P1,Vector4  P2)
{
    Vector4 V1;
    Vector4 V2;
    Vector4 V3;
    Vector4 LengthSq;
    Vector4 RcpLengthSq;
    Vector4 Point;
    Vector4 P1W;
    Vector4 P2W;
    Vector4 Control;
    Vector4 LinePoint1;
    Vector4 LinePoint2;

    V1 = Vector3Cross(P2, P1);

    LengthSq = V1;

    V2 = Vector3Cross(P2, V1);

    P1W = VectorSplatW(P1);
    Point = Vector4Multiply(V2, P1W);

    V3 = Vector3Cross(V1, P1);

    P2W = VectorSplatW(P2);
    Point = Vector4Multiply(V3, P2W)+Point;

    RcpLengthSq = VectorReciprocal(LengthSq);
    LinePoint1 = Vector4Multiply(Point, RcpLengthSq);

    LinePoint2 = LinePoint1+V1;

    Control = VectorLessOrEqual(LengthSq, g_XMEpsilon.v);
    *pLinePoint1 = Vector4Select(LinePoint1,g_XMQNaN.v, Control);
    *pLinePoint2 = Vector4Select(LinePoint2,g_XMQNaN.v, Control);
}

//------------------------------------------------------------------------------

Vector4 PlaneTransform
(
    Vector4 P,
    cMatrix44 M
)
{
    Vector4 X;
    Vector4 Y;
    Vector4 Z;
    Vector4 W;
    Vector4 Result;

    W = VectorSplatW(P);
    Z = VectorSplatZ(P);
    Y = VectorSplatY(P);
    X = VectorSplatX(P);

    Result = Vector4Multiply(W, M.r[3]);
    Result = Vector4Multiply(Z, M.r[2])+ Result;
    Result = Vector4Multiply(Y, M.r[1])+ Result;
    Result = Vector4Multiply(X, M.r[0])+ Result;

    return Result;
}

//------------------------------------------------------------------------------

//XMFLOAT4* PlaneTransformStream
//(
//    XMFLOAT4*       pOutputStream,
//    UINT            OutputStride,
//    const XMFLOAT4* pInputStream,
//    UINT            InputStride,
//    UINT            PlaneCount,
//    CXMMATRIX     M
//)
//{
//    return Vector44TransformStream(pOutputStream,
//                                    OutputStride,
//                                    pInputStream,
//                                    InputStride,
//                                    PlaneCount,
//                                    M);
//}

//------------------------------------------------------------------------------
// Conversion operations
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

Vector4 PlaneFromPointNormal
(
    Vector4 Point,
    Vector4 Normal
)
{

    Vector4 W;
    Vector4 Result;

    W = Vector4Dot(Point, Normal);
    W *= -1;
    Result = Vector4Select(W, Normal, g_XMSelect1110.v);

    return Result;
}

//------------------------------------------------------------------------------

Vector4 PlaneFromPoints
(
    Vector4 Point1,
    Vector4 Point2,
    Vector4 Point3
)
{
    Vector4 N;
    Vector4 D;
    Vector4 V21;
    Vector4 V31;
    Vector4 Result;

    V21 = Point1-Point2;
    V31 = Point1-Point3;

    N = Vector3Cross(V21, V31);
    N = Vector3Normalize(N);

    float	l_fD = PlaneDotNormal(N, Point1);
	D = Vector4(l_fD,l_fD,l_fD,l_fD);
    D *= -1;

    Result = Vector4Select(D, N, g_XMSelect1110.v);

    return Result;
}
