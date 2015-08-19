//http://www.gamedev.net/community/forums/topic.asp?topic_id=341632
//
//When a plane is expressed as:
//
//ax+by+cz = d
//
//Or more commonly:
//
//p.n = d
//
//Then d can be viewed as the distance from the origin to the plane, in the direction of the plane normal.
//
//We can also write the equation as ax+by+cz+d = 0, but note that this d is the negative of the d in the first equation. So with this form, -d is the distance to the plane along the normal.
//
//Note that if the plane is not normalized, these distances are scaled by |n|.
//
//In your example, the point (0,-100,0) would be on the plane, which we can show by plugging it into the equation:
//
//0*0-1*100+0*0+100 = -100+100 = 0
//
//So you had the right idea in your post, just backwards.
//
//
//If p is a point
//If n.p + d = 0, the point is on the plane
//If n.p + d > 0, the point is in front of the plane
//If n.p + d < 0, the point is behind the plane
bool            PlaneEqual(Vector4 P1, Vector4 P2);
bool            PlaneNearEqual(Vector4 P1, Vector4 P2, Vector4 Epsilon);
bool            PlaneNotEqual(Vector4 P1, Vector4 P2);

bool            PlaneIsNaN(Vector4 P);
bool            PlaneIsInfinite(Vector4 P);
//plane visible a point,if <0 invisible if >0 visible 1 on the plane
float			PlaneDot(Vector4 P, Vector4 V);
float			PlaneDotCoord(Vector4 P, Vector4 V);
float			PlaneDotNormal(Vector4 P, Vector4 V);
//Vector4			PlaneNormalizeEst(Vector4 P);
Vector4			PlaneNormalize(Vector4 P);
Vector4			PlaneIntersectLine(Vector4 P, Vector4 LinePoint1, Vector4 LinePoint2);
void			PlaneIntersectPlane( Vector4* pLinePoint1,  Vector4* pLinePoint2, Vector4 P1, Vector4 P2);
Vector4			PlaneTransform(Vector4 P, cMatrix44 M);
//XMFLOAT4*       PlaneTransformStream(__out_bcount(sizeof(XMFLOAT4)+OutputStride*(PlaneCount-1)) XMFLOAT4* pOutputStream,
//                                       __in UINT OutputStride,
//                                       __in_bcount(sizeof(XMFLOAT4)+InputStride*(PlaneCount-1)) const XMFLOAT4* pInputStream,
//                                       __in UINT InputStride, __in UINT PlaneCount, CXMMATRIX M);

Vector4        PlaneFromPointNormal(Vector4 Point, Vector4 Normal);
Vector4        PlaneFromPoints(Vector4 Point1, Vector4 Point2, Vector4 Point3);