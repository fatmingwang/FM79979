//https://www.opengl.org/discussion_boards/showthread.php/167115-Creating-cylinder
float radius, halfLength;
int slices;
for(int i=0; i<slices; i++) 
{
	float theta = ((float)i)*2.0*M_PI;
	float nextTheta = ((float)i+1)*2.0*M_PI;
	glBegin(GL_TRIANGLE_STRIP);
	/*vertex at middle of end */ 
	glVertex3f(0.0, halfLength, 0.0);
	/*vertices at edges of circle*/ 
	glVertex3f(radius*cos(theta), halfLength, radius*sin(theta));
	glVertex3f (radius*cos(nextTheta), halfLength, radius*sin(nextTheta));
	/* the same vertices at the bottom of the cylinder*/
	glVertex3f (radius*cos(nextTheta), -halfLength, radius*sin(nextTheta));
	glVertex3f(radius*cos(theta), -halfLength, radius*sin(theta));
	glVertex3f(0.0, -halfLength, 0.0);
	glEnd();
}

//http://gamedev.stackexchange.com/questions/17214/create-vertices-indices-for-cylinder-with-triangle-strips
// draws a cylinder 'height' high on the y axis at x,y,z position
const float theta = 2. * M_PI / (float)sides;
    c = cos(theta),
    s = sin(theta);
// coordinates on top of the circle, on xz plane
float x2 = radius, z2 = 0;
// make the strip
glBegin(GL_TRIANGLE_STRIP);
for(int i=0; i<=sides; i++) {
    // texture coord
    const float tx = (float)i/sides;
    // normal
    const float nf = 1./math.sqrt(x2*x2+z2*z2),
        xn = x2*nf, zn = z2*nf;
    glNormal3f(xn,0,zn);
    glTexCoord2f(tx,0);
    glVertex3f(x+x2,y,z+z2)
    glNormal3f(xn,0,zn);
    glTexCoord2f(tx,1);
    glVertex3f(x+x2,y+height,z+z2);
    // next position
    const float x3 = x2;
    x2 = c * x2 - s * z2;
    z2 = s * x3 + c * z2;
}
glEnd();

float const bottom = .0005f * 0.f;
float const top    = .0005f * (segments-1.f);
for(GLuint n = 0; n <= segments; ++n)
{
    GLfloat const t0 = 2 * M_PI * (float)n / (float)segments;
    GLfloat const t1 = 2 * M_PI * (float)(n+1) / (float)segments;
    //quad vertex 0
    points[num++] = x + sin(t0) * r;
    points[num++] = bottom;
    points[num++] = y + cos(t0) * r;
    //quad vertex 1
    points[num++] = x + sin(t1) * r;
    points[num++] = bottom;
    points[num++] = y + cos(t1) * r;
    //quad vertex 2
    points[num++] = x + sin(t1) * r;
    points[num++] = top;
    points[num++] = y + cos(t1) * r;
    //quad vertex 3
    points[num++] = x + sin(t0) * r;
    points[num++] = top;
    points[num++] = y + cos(t0) * r;
}
//http://stackoverflow.com/questions/9711803/trying-draw-cylinder-in-directx-through-d3dxcreatecylinder
for( DWORD i = 0; i < Sides; i++ )
{
    FLOAT theta = ( 2 * D3DX_PI * i ) / ( Sides - 1 );

    pVertices[2 * i + 0].position = D3DXVECTOR3(radius*sinf( theta ), -height, radius*cosf( theta ) );
    pVertices[2 * i + 0].color = 0xffffffff;
    pVertices[2 * i + 0].tu = ( ( FLOAT )i ) / ( Sides - 1 );
    pVertices[2 * i + 0].tv = 1.0f;


    pVertices[2 * i + 1].position = D3DXVECTOR3( radius*sinf( theta ), height, radius*cosf( theta ) );
    pVertices[2 * i + 1].color = 0xff808080;
    pVertices[2 * i + 1].tu = ( ( FLOAT )i ) / ( Sides - 1 );
    pVertices[2 * i + 1].tv = 0.0f;

} 

//https://discourse.mcneel.com/t/how-to-build-a-spherical-mesh-and-a-cylindrical-mesh-c/20511
class cPNTVert{
public:
    Vector3 pos, norm;
    Vector2 tex;
};
 
inline float DegToRad(const float& deg){ return deg*(Pi/180.0f); }
 
//vector<cPNTVert>& v can contain vertices and in is the index buffer. They can already have data in them and you can use this function to append a cylinder onto the data already there
 
void GenCylinder(size_t lengthsegs, size_t heightsegs, vector<cPNTVert>& v, vector<uint16_t>& in, size_t indexstride){
    heightsegs+=1;// we need an extra one here!
    size_t startvertex = v.size();// if v is empty, the size is 0, so we are starting on an empty buffer, but if there is data, this will append it correctly
    size_t totalverts = lengthsegs*heightsegs;
    v.resize(startvertex + totalverts);
        vector<Vector3> verts(totalverts), norms(totalverts);
        vector<Vector2> texs(totalverts);
    float heightstride = 1.0f/static_cast<float>(lengthsegs);
    float height = -heightstride;
    float deg = 0;
    for(size_t i(0); i< lengthsegs; i++){
        height+=heightstride;
        for(size_t j(0); j < heightsegs; j++){
            verts[i*heightsegs + j] = Vector3(cos32(DegToRad(deg)), height, sin32(DegToRad(deg) ) );
            texs[i*heightsegs + j] = Vector2(static_cast<float>(j)/(static_cast<float>(heightsegs)-1.0f), static_cast<float>(i+1));
            deg += 360/(static_cast<float>(heightsegs-1));
        }
        deg = 0.0f;
    }
    size_t nind = (6*(lengthsegs-1)*(heightsegs));
    if(indexstride ==2 ){// two byte index stride
        size_t startindex = in.size();
        in.resize(nind + startindex);
        size_t off(0);
        for(size_t i(0); i< lengthsegs-1; i++){
            for(size_t j(0); j< heightsegs; j++){
                in[off     + startindex] = i*heightsegs + j;
                in[off + 1 + startindex] = (i+1)*heightsegs + j;
                in[off + 2 + startindex] = i*heightsegs + j + 1;
                in[off + 3 + startindex] = i*heightsegs + j + 1;
                in[off + 4 + startindex] = (i+1)*heightsegs + j;
                in[off + 5 + startindex] = (i+1)*heightsegs + j + 1;
                off+=6;
            }
        }
        ComputeNormals(&verts[0], totalverts, &in[startindex], nind, &norms[0]);
        for(size_t i(0); i< (lengthsegs-1)*heightsegs*6; i++) in[i +startindex] += static_cast<uint16_t>(startvertex);// remap the indices
    } else {// 4 byte index stride..
        size_t startindex = in.size();// beginning of our indices
        in.resize((nind*2) + startindex);// allocate twice as many
        uint32_t* indices = reinterpret_cast<uint32_t*>(&in[startindex]);
        size_t off(0);
        for(size_t i(0); i< lengthsegs-1; i++){
            for(size_t j(0); j< heightsegs; j++){
                indices[off    ] = i*heightsegs + j;
                indices[off + 1] = (i+1)*heightsegs + j;
                indices[off + 2] = i*heightsegs + j + 1;
                indices[off + 3] = i*heightsegs + j + 1;
                indices[off + 4] = (i+1)*heightsegs + j;
                indices[off + 5] = (i+1)*heightsegs + j + 1;
                off+=6;
            }
        }
        ComputeNormals(&verts[0], totalverts, indices, nind, &norms[0]);
        for(size_t i(0); i< (lengthsegs-1)*heightsegs*6; i++) indices[i +startindex] += startvertex;// remap the indices
    }
    for(size_t i(0); i<totalverts; i++){
        v[i + startvertex].pos = verts[i];
        v[i + startvertex].tex = texs[i];
        v[i + startvertex].norm = norms[i];
    }
}
 
template<class T>void ComputeNormals(Vector3* Vertices, size_t numverts, T* indices, size_t numindices, Vector3* normals){
    size_t i = 0;
    memset(normals, 0, sizeof(Vector3)* numverts);
    do{
        Vector3 v0(Vertices[indices[i    ]]);
        Vector3 v1(Vertices[indices[i + 1]]);
        Vector3 v2(Vertices[indices[i + 2]]);
        Vector3 normal(Cross(v1 - v0, v2 - v0));
        normal.normalize();
        normals[indices[i    ]] += normal;
        normals[indices[i + 1]] += normal;
        normals[indices[i + 2]] += normal;
        i += 3;
    } while(i < numindices);
    i=0;
    do{ normals[i].normalize(); } while(++i < numverts);
}