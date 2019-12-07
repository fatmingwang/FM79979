#include "Glh.h"
//	void gluLookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez,
//				   GLfloat centerx, GLfloat centery, GLfloat centerz,
//				   GLfloat upx, GLfloat upy, GLfloat upz)
//	{
//		GLfloat m[16];
//		GLfloat x[3], y[3], z[3];
//		GLfloat mag;
//		
//		/* Make rotation matrix */
//		
//		/* Z vector */
//		z[0] = eyex - centerx;
//		z[1] = eyey - centery;
//		z[2] = eyez - centerz;
//		mag = sqrt(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
//		if (mag) {          /* mpichler, 19950515 */
//			z[0] /= mag;
//			z[1] /= mag;
//			z[2] /= mag;
//		}
//		
//		/* Y vector */
//		y[0] = upx;
//		y[1] = upy;
//		y[2] = upz;
//		
//		/* X vector = Y cross Z */
//		x[0] = y[1] * z[2] - y[2] * z[1];
//		x[1] = -y[0] * z[2] + y[2] * z[0];
//		x[2] = y[0] * z[1] - y[1] * z[0];
//		
//		/* Recompute Y = Z cross X */
//		y[0] = z[1] * x[2] - z[2] * x[1];
//		y[1] = -z[0] * x[2] + z[2] * x[0];
//		y[2] = z[0] * x[1] - z[1] * x[0];
//		
//		/* mpichler, 19950515 */
//		/* cross product gives area of parallelogram, which is < 1.0 for
//		 * non-perpendicular unit-length vectors; so normalize x, y here
//		 */
//		
//		mag = sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
//		if (mag) {
//			x[0] /= mag;
//			x[1] /= mag;
//			x[2] /= mag;
//		}
//		
//		mag = sqrt(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);
//		if (mag) {
//			y[0] /= mag;
//			y[1] /= mag;
//			y[2] /= mag;
//		}
//		
//#define M(row,col)  m[col*4+row]
//		M(0, 0) = x[0];
//		M(0, 1) = x[1];
//		M(0, 2) = x[2];
//		M(0, 3) = 0.0;
//		M(1, 0) = y[0];
//		M(1, 1) = y[1];
//		M(1, 2) = y[2];
//		M(1, 3) = 0.0;
//		M(2, 0) = z[0];
//		M(2, 1) = z[1];
//		M(2, 2) = z[2];
//		M(2, 3) = 0.0;
//		M(3, 0) = 0.0;
//		M(3, 1) = 0.0;
//		M(3, 2) = 0.0;
//		M(3, 3) = 1.0;
//#undef M
//		glMultMatrixf(m);
//		
//		/* Translate Eye to Origin */
//		glTranslatef(-eyex, -eyey, -eyez);
//		
//	}


/*
	* This code was contributed by Marc Buffat (buffat@mecaflu.ec-lyon.fr).
	* Thanks Marc!!!
	*/



/* implementation de gluProject et gluUnproject */
/* M. Buffat 17/2/95 */



/*
	* Transform a point (column vector) by a 4x4 matrix.  I.e.  out = m * in
	* Input:  m - the 4x4 matrix
	*         in - the 4x1 vector
	* Output:  out - the resulting 4x1 vector.
	*/
void	transform_point(float out[4], const float m[16], const float in[4])
{
#define M(row,col)  m[col*4+row]
	out[0] =
		M(0, 0) * in[0] + M(0, 1) * in[1] + M(0, 2) * in[2] + M(0, 3) * in[3];
	out[1] =
		M(1, 0) * in[0] + M(1, 1) * in[1] + M(1, 2) * in[2] + M(1, 3) * in[3];
	out[2] =
		M(2, 0) * in[0] + M(2, 1) * in[1] + M(2, 2) * in[2] + M(2, 3) * in[3];
	out[3] =
		M(3, 0) * in[0] + M(3, 1) * in[1] + M(3, 2) * in[2] + M(3, 3) * in[3];
#undef M
}




/*
	* Perform a 4x4 matrix multiplication  (product = a x b).
	* Input:  a, b - matrices to multiply
	* Output:  product - product of a and b
	*/
void	matmul(float * product, const float * a, const float * b)
{
	/* This matmul was contributed by Thomas Malik */
	float temp[16];
	GLint i;

#define A(row,col)  a[(col<<2)+row]
#define B(row,col)  b[(col<<2)+row]
#define T(row,col)  temp[(col<<2)+row]

	/* i-te Zeile */
	for (i = 0; i < 4; i++) {
		T(i, 0) =
		A(i, 0) * B(0, 0) + A(i, 1) * B(1, 0) + A(i, 2) * B(2, 0) + A(i,
										3) *
		B(3, 0);
		T(i, 1) =
		A(i, 0) * B(0, 1) + A(i, 1) * B(1, 1) + A(i, 2) * B(2, 1) + A(i,
										3) *
		B(3, 1);
		T(i, 2) =
		A(i, 0) * B(0, 2) + A(i, 1) * B(1, 2) + A(i, 2) * B(2, 2) + A(i,
										3) *
		B(3, 2);
		T(i, 3) =
		A(i, 0) * B(0, 3) + A(i, 1) * B(1, 3) + A(i, 2) * B(2, 3) + A(i,
										3) *
		B(3, 3);
	}

#undef A
#undef B
#undef T
	memcpy(product, temp, 16 * sizeof(float));
}



/*
	* Compute inverse of 4x4 transformation matrix.
	* Code contributed by Jacques Leroy jle@star.be
	* Return GL_TRUE for success, GL_FALSE for failure (singular matrix)
	*/
GLboolean	invert_matrix(const float * m, float * out)
{
/* NB. OpenGL Matrices are COLUMN major. */
#define SWAP_ROWS(a, b) { float *_tmp = a; (a)=(b); (b)=_tmp; }
#define MAT(m,r,c) (m)[(c)*4+(r)]

	float wtmp[4][8];
	float m0, m1, m2, m3, s;
	float *r0, *r1, *r2, *r3;

	r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];

	r0[0] = MAT(m, 0, 0), r0[1] = MAT(m, 0, 1),
		r0[2] = MAT(m, 0, 2), r0[3] = MAT(m, 0, 3),
		r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,
		r1[0] = MAT(m, 1, 0), r1[1] = MAT(m, 1, 1),
		r1[2] = MAT(m, 1, 2), r1[3] = MAT(m, 1, 3),
		r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,
		r2[0] = MAT(m, 2, 0), r2[1] = MAT(m, 2, 1),
		r2[2] = MAT(m, 2, 2), r2[3] = MAT(m, 2, 3),
		r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,
		r3[0] = MAT(m, 3, 0), r3[1] = MAT(m, 3, 1),
		r3[2] = MAT(m, 3, 2), r3[3] = MAT(m, 3, 3),
		r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;

	/* choose pivot - or die */
	if (fabs(r3[0]) > fabs(r2[0]))
		SWAP_ROWS(r3, r2);
	if (fabs(r2[0]) > fabs(r1[0]))
		SWAP_ROWS(r2, r1);
	if (fabs(r1[0]) > fabs(r0[0]))
		SWAP_ROWS(r1, r0);
	if (0.0 == r0[0])
		return GL_FALSE;

	/* eliminate first variable     */
	m1 = r1[0] / r0[0];
	m2 = r2[0] / r0[0];
	m3 = r3[0] / r0[0];
	s = r0[1];
	r1[1] -= m1 * s;
	r2[1] -= m2 * s;
	r3[1] -= m3 * s;
	s = r0[2];
	r1[2] -= m1 * s;
	r2[2] -= m2 * s;
	r3[2] -= m3 * s;
	s = r0[3];
	r1[3] -= m1 * s;
	r2[3] -= m2 * s;
	r3[3] -= m3 * s;
	s = r0[4];
	if (s != 0.0) {
		r1[4] -= m1 * s;
		r2[4] -= m2 * s;
		r3[4] -= m3 * s;
	}
	s = r0[5];
	if (s != 0.0) {
		r1[5] -= m1 * s;
		r2[5] -= m2 * s;
		r3[5] -= m3 * s;
	}
	s = r0[6];
	if (s != 0.0) {
		r1[6] -= m1 * s;
		r2[6] -= m2 * s;
		r3[6] -= m3 * s;
	}
	s = r0[7];
	if (s != 0.0) {
		r1[7] -= m1 * s;
		r2[7] -= m2 * s;
		r3[7] -= m3 * s;
	}

	/* choose pivot - or die */
	if (fabs(r3[1]) > fabs(r2[1]))
		SWAP_ROWS(r3, r2);
	if (fabs(r2[1]) > fabs(r1[1]))
		SWAP_ROWS(r2, r1);
	if (0.0 == r1[1])
		return GL_FALSE;

	/* eliminate second variable */
	m2 = r2[1] / r1[1];
	m3 = r3[1] / r1[1];
	r2[2] -= m2 * r1[2];
	r3[2] -= m3 * r1[2];
	r2[3] -= m2 * r1[3];
	r3[3] -= m3 * r1[3];
	s = r1[4];
	if (0.0 != s) {
		r2[4] -= m2 * s;
		r3[4] -= m3 * s;
	}
	s = r1[5];
	if (0.0 != s) {
		r2[5] -= m2 * s;
		r3[5] -= m3 * s;
	}
	s = r1[6];
	if (0.0 != s) {
		r2[6] -= m2 * s;
		r3[6] -= m3 * s;
	}
	s = r1[7];
	if (0.0 != s) {
		r2[7] -= m2 * s;
		r3[7] -= m3 * s;
	}

	/* choose pivot - or die */
	if (fabs(r3[2]) > fabs(r2[2]))
		SWAP_ROWS(r3, r2);
	if (0.0 == r2[2])
		return GL_FALSE;

	/* eliminate third variable */
	m3 = r3[2] / r2[2];
	r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
		r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6], r3[7] -= m3 * r2[7];

	/* last check */
	if (0.0 == r3[3])
		return GL_FALSE;

	s = 1.f / r3[3];		/* now back substitute row 3 */
	r3[4] *= s;
	r3[5] *= s;
	r3[6] *= s;
	r3[7] *= s;

	m2 = r2[3];			/* now back substitute row 2 */
	s = 1.f / r2[2];
	r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
		r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
	m1 = r1[3];
	r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
		r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
	m0 = r0[3];
	r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
		r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;

	m1 = r1[2];			/* now back substitute row 1 */
	s = 1.f / r1[1];
	r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
		r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
	m0 = r0[2];
	r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
		r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;

	m0 = r0[1];			/* now back substitute row 0 */
	s = 1.f / r0[0];
	r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
		r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);

	MAT(out, 0, 0) = r0[4];
	MAT(out, 0, 1) = r0[5], MAT(out, 0, 2) = r0[6];
	MAT(out, 0, 3) = r0[7], MAT(out, 1, 0) = r1[4];
	MAT(out, 1, 1) = r1[5], MAT(out, 1, 2) = r1[6];
	MAT(out, 1, 3) = r1[7], MAT(out, 2, 0) = r2[4];
	MAT(out, 2, 1) = r2[5], MAT(out, 2, 2) = r2[6];
	MAT(out, 2, 3) = r2[7], MAT(out, 3, 0) = r3[4];
	MAT(out, 3, 1) = r3[5], MAT(out, 3, 2) = r3[6];
	MAT(out, 3, 3) = r3[7];

	return GL_TRUE;

#undef MAT
#undef SWAP_ROWS
}



/* projection du point (objx,objy,obz) sur l'ecran (winx,winy,winz) */
GLint 
gluProject(GLfloat  objx, GLfloat  objy, GLfloat  objz,
		const GLfloat  model[16], const GLfloat  proj[16],
		const GLint viewport[4],
		GLfloat  * winx, GLfloat  * winy, GLfloat  * winz)
{
	/* matrice de transformation */
	float in[4], out[4];

	/* initilise la matrice et le vecteur a transformer */
	in[0] = objx;
	in[1] = objy;
	in[2] = objz;
	in[3] = 1.0;
	transform_point(out, model, in);
	transform_point(in, proj, out);

	/* d'ou le resultat normalise entre -1 et 1 */
	if (in[3] == 0.0)
		return GL_FALSE;

	in[0] /= in[3];
	in[1] /= in[3];
	in[2] /= in[3];

	/* en coordonnees ecran */
	*winx = viewport[0] + (1 + in[0]) * viewport[2] / 2;
	*winy = viewport[1] + (1 + in[1]) * viewport[3] / 2;
	/* entre 0 et 1 suivant z */
	*winz = (1 + in[2]) / 2;
	return GL_TRUE;
}



/* transformation du point ecran (winx,winy,winz) en point objet */
GLint 
gluUnProject(float winx, float winy, float winz,
			const float model[16], const float proj[16],
			const GLint viewport[4],
			float * objx, float * objy, float * objz)
{
	/* matrice de transformation */
	float m[16], A[16];
	float in[4], out[4];

	/* transformation coordonnees normalisees entre -1 et 1 */
	in[0] = (winx - viewport[0]) * 2 / viewport[2] - 1.f;
	in[1] = (winy - viewport[1]) * 2 / viewport[3] - 1.f;
	in[2] = 2 * winz - 1.f;
	in[3] = 1.f;

	/* calcul transformation inverse */
	matmul(A, proj, model);
	invert_matrix(A, m);

	/* d'ou les coordonnees objets */
	transform_point(out, m, in);
	if (out[3] == 0.0)
		return GL_FALSE;
	*objx = out[0] / out[3];
	*objy = out[1] / out[3];
	*objz = out[2] / out[3];
	return GL_TRUE;
}


/*
	* New in GLU 1.3
	* This is like gluUnProject but also takes near and far DepthRange values.
	*/
GLint 
gluUnProject4(float winx, float winy, float winz, float clipw,
			const float modelMatrix[16],
			const float projMatrix[16],
			const GLint viewport[4],
			float nearZ, float farZ,
			float * objx, float * objy, float * objz,
			float * objw)
{
	/* matrice de transformation */
	float m[16], A[16];
	float in[4], out[4];
	float z = nearZ + winz * (farZ - nearZ);

	/* transformation coordonnees normalisees entre -1 et 1 */
	in[0] = (winx - viewport[0]) * 2 / viewport[2] - 1.f;
	in[1] = (winy - viewport[1]) * 2 / viewport[3] - 1.f;
	in[2] = 2.f * z - 1.f;
	in[3] = clipw;

	/* calcul transformation inverse */
	matmul(A, projMatrix, modelMatrix);
	invert_matrix(A, m);

	/* d'ou les coordonnees objets */
	transform_point(out, m, in);
	if (out[3] == 0.0)
		return GL_FALSE;
	*objx = out[0] / out[3];
	*objy = out[1] / out[3];
	*objz = out[2] / out[3];
	*objw = out[3];
	return GL_TRUE;
}

//void gluPerspective(  float fovy,  float aspect,  float zNear,  float zFar)
//{
//	float	l_fMatrix[16];
//	glhPerspectivef2(l_fMatrix,(float)fovy,(float)aspect,(float)zNear,(float)zFar);
//	glMultMatrixf(l_fMatrix);
//}