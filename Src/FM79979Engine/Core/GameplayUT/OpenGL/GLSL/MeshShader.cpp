//=======================
//for mesh
//=======================
namespace FATMING_CORE
{
	const char*g_strMyMeshVS =
	R"(
		attribute vec3 VSPosition;
		attribute vec3 VSNormal;
		attribute vec2 VSTexcoord;
		uniform mat4 matVP;
		uniform mat4 matW;
		varying vec2 PSTexcoord;
		varying vec3 PSNormal;
		void main()
		{
			gl_Position = matVP*matW*vec4(VSPosition,1);
			PSTexcoord = VSTexcoord;
			PSNormal = VSNormal;
		}
	)";
	const char*g_strMyMeshWithVertexColorVS =
	R"(
		attribute vec3 VSPosition;
		attribute vec3 VSNormal;
		attribute vec4 VSColor;
		attribute vec2 VSTexcoord;
		uniform mat4 matVP;
		uniform mat4 matW;
		varying vec2 PSTexcoord;
		varying vec3 PSNormal;
		varying vec4 PSColor;
		void main()
		{
			gl_Position = matVP*matW*vec4(VSPosition,1);
			PSTexcoord = VSTexcoord;
			PSNormal = VSNormal;
			PSColor = VSColor;
		}
	)";





#if defined(WIN32) && !defined(UWP)
	const char*g_strMyMeshWithVertexColorFS =
	R"(
		uniform sampler2D texSample;
		uniform vec4 PSColor;
		varying vec2 PSTexcoord;
		varying vec3 PSNormal;
		void main()
		{
			gl_FragColor = texture2D(texSample, PSTexcoord)+(PSColor-vec4(1,1,1,1));
		}
	)";
	const char*g_strMyMeshFS =
	R"(
		uniform sampler2D texSample;
		varying vec2 PSTexcoord;
		varying vec3 PSNormal;
		void main()
		{
			gl_FragColor = texture2D(texSample, PSTexcoord);
		}
	)";
	const char*g_strMySkinningMeshWithVertexColorVS = 
	R"(
		attribute	vec3	VSPosition;
		attribute	vec3	VSNormal;
		attribute	vec4	VSColor;
		attribute	vec2	VSTexcoord;
		attribute	vec4	VSWeights;
		attribute	vec4	VSInflunceBoneIndices;
		uniform		mat4	matVP;
		uniform		mat4	matW;
		uniform		mat4	matBones[200];
		varying		vec2	PSTexcoord;
		varying		vec3	PSNormal;
		varying		vec4	PSColor;
		void main()
		{
			gl_Position = vec4(0,0,0,0);
			PSNormal = vec3(0,0,0);
			vec4 curIndex = VSInflunceBoneIndices;
			vec4 curWeight = VSWeights;
			for( int i=0;i<4;i++ )
			{
					mat4 m44 = matBones[int(curIndex.x)];
					gl_Position += m44 * vec4(VSPosition,1) * curWeight.x;
					mat3 m33 = mat3(m44[0].xyz,
									m44[1].xyz,
									m44[2].xyz);
					PSNormal += m33 * VSNormal * curWeight.x;
					curIndex = curIndex.yzwx;
					curWeight = curWeight.yzwx;
			}
			gl_Position = matVP *matW* gl_Position;
			PSTexcoord = VSTexcoord;
			PSNormal = VSNormal;
			PSColor = VSColor;
		}
	)";
	const char*g_strMySkinningMeshWithVertexColorFS =
	R"(
		uniform sampler2D texSample;
		uniform vec4 PSColor;
		varying vec2 PSTexcoord;
		varying vec3 PSNormal;
		void main()
		{
			gl_FragColor = texture2D(texSample, PSTexcoord)+(PSColor-vec4(1,1,1,1));
		}
	)";
	const char*g_strMySkinningMeshVS =
	R"(
		attribute	vec3	VSPosition;
		attribute	vec3	VSNormal;
		attribute	vec2	VSTexcoord;
		attribute	vec4	VSWeights;
		attribute	vec4	VSInflunceBoneIndices;
		uniform		mat4	matVP;
		uniform		mat4	matW;
		uniform		mat4	matBones[200];
		varying		vec2	PSTexcoord;
		varying		vec3	PSNormal;
		void main()
		{
			gl_Position = vec4(0,0,0,0);
			PSNormal = vec3(0,0,0);
			vec4 curIndex = VSInflunceBoneIndices;
			vec4 curWeight = VSWeights;
			for( int i=0;i<4;i++ )
			{
					mat4 m44 = matBones[int(curIndex.x)];
					gl_Position += m44 * vec4(VSPosition,1) * curWeight.x;
					mat3 m33 = mat3(m44[0].xyz,
									m44[1].xyz,
									m44[2].xyz);
					PSNormal += m33 * VSNormal * curWeight.x;
					curIndex = curIndex.yzwx;
					curWeight = curWeight.yzwx;
			}
			gl_Position = matVP *matW* gl_Position;
			PSTexcoord = VSTexcoord;
			PSNormal = VSNormal;
		}
	)";
	const char*g_strMySkinningMeshFS =
	R"(
		uniform sampler2D texSample;
		varying vec2 PSTexcoord;
		varying vec3 PSNormal;
		void main()
		{
			gl_FragColor = texture2D(texSample, PSTexcoord);
		}
	)";
#else
	const char*g_strMyMeshWithVertexColorFS =
	R"(
		uniform sampler2D texSample;
		varying lowp vec4 PSColor;
		varying lowp vec2 PSTexcoord;
		varying lowp vec3 PSNormal;
		void main()
		{
			gl_FragColor = texture2D(texSample, PSTexcoord)*PSColor;
		}
	)";

	const char*g_strMyMeshFS =
	R"(
		uniform sampler2D texSample;
		varying lowp vec2 PSTexcoord;
		varying lowp vec3 PSNormal;
		void main()
		{
			gl_FragColor = texture2D(texSample, PSTexcoord);
		}
	)";
	//matBones size will be different on different device...
	//because some device bufferis not enough so 32 is a safe range
	const char*g_strMySkinningMeshVS =
	R"(
		attribute	vec3	VSPosition;
		attribute	vec3	VSNormal;
		attribute	vec2	VSTexcoord;
		attribute	vec4	VSWeights;
		attribute	vec4	VSInflunceBoneIndices;
		uniform		mat4	matVP;
		uniform		mat4	matW;
		uniform		mat4	matBones[32];
		varying		vec2	PSTexcoord;
		varying		vec3	PSNormal;
		void main()
		{
			gl_Position = vec4(0,0,0,0);
			PSNormal = vec3(0,0,0);
			vec4 curIndex = VSInflunceBoneIndices;
			vec4 curWeight = VSWeights;
			for( int i=0;i<4;i++ )
			{
					mat4 m44 = matBones[int(curIndex.x)];
					gl_Position += m44 * vec4(VSPosition,1) * curWeight.x;
					mat3 m33 = mat3(m44[0].xyz,
									m44[1].xyz,
									m44[2].xyz);
					PSNormal += m33 * VSNormal * curWeight.x;
					curIndex = curIndex.yzwx;
					curWeight = curWeight.yzwx;
			}
			gl_Position = matVP *matW* gl_Position;
			PSTexcoord = VSTexcoord;
			PSNormal = VSNormal;
		}
	)";

	const char*g_strMySkinningMeshFS = 
	R"(
		uniform sampler2D texSample;
		varying lowp vec2 PSTexcoord;
		varying lowp vec3 PSNormal;
		void main()
		{
			gl_FragColor = texture2D(texSample, PSTexcoord);
		}
	)";
	//because some device bufferis not enough so 32 is a safe range
	const char*g_strMySkinningMeshWithVertexColorVS =
	R"(
		attribute	vec3	VSPosition;
		attribute	vec3	VSNormal;
		attribute	vec4	VSColor;
		attribute	vec2	VSTexcoord;
		attribute	vec4	VSWeights;
		attribute	vec4	VSInflunceBoneIndices;
		uniform		mat4	matVP;
		uniform		mat4	matW;
		uniform		mat4	matBones[32];
		varying		vec2	PSTexcoord;
		varying		vec3	PSNormal;
		varying		vec4	PSColor;
		void main()
		{
			gl_Position = vec4(0,0,0,0);
			PSNormal = vec3(0,0,0);
			vec4 curIndex = VSInflunceBoneIndices;
			vec4 curWeight = VSWeights;
			for( int i=0;i<4;i++ )
			{
					mat4 m44 = matBones[int(curIndex.x)];
					gl_Position += m44 * vec4(VSPosition,1) * curWeight.x;
					mat3 m33 = mat3(m44[0].xyz,
									m44[1].xyz,
									m44[2].xyz);
					PSNormal += m33 * VSNormal * curWeight.x;
					curIndex = curIndex.yzwx;
					curWeight = curWeight.yzwx;
			}
			gl_Position = matVP *matW* gl_Position;
			PSTexcoord = VSTexcoord;
			PSNormal = VSNormal;
			PSColor = VSColor;
		}
	)";
	const char*g_strMySkinningMeshWithVertexColorFS =
	R"(
		uniform sampler2D texSample;								\
		varying lowp vec4 PSColor;									\
		varying lowp vec2 PSTexcoord;								\
		varying lowp vec3 PSNormal;									\
																	\
		void main()													\
		{															\
			gl_FragColor = texture2D(texSample, PSTexcoord)*PSColor;\
		}
	)";
#endif
}