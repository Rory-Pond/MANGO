#version 130
//---------------------------
//----- Vertex Shader -------
//---------------------------

// In data
in vec3 vertex;
in vec3 normal;
in vec3 pos;
in vec3 spin;

// Uniforms
uniform mat4 VP;
uniform mat4 V;
uniform vec3 LightPos;

// Out data
out vec3 Color;
out vec3 vertexPos;
out vec3 Normal;
out mat4 M;

void main()
{
	float angle = acos(dot(vec3(0,1,0),spin));
	vec3 axis = cross(vec3(0,1,0),spin);
	vec4 Q = vec4(axis,angle);

	Q = normalize(Q);

	float qxx = (Q.x * Q.x);
	float qyy = (Q.y * Q.y);
	float qzz = (Q.z * Q.z);
	float qxz = (Q.x * Q.z);
	float qxy = (Q.x * Q.y);
	float qyz = (Q.y * Q.z);
	float qwx = (Q.w * Q.x);
	float qwy = (Q.w * Q.y);
	float qwz = (Q.w * Q.z);

	//mat4 RotationMatrix = quaternion::toMat4(quaternion);

	mat4 rotateQ = mat4(
		1 - 2 * (qyy +  qzz),	2 * (qxy + qwz),		2 * (qxz - qwy),		0,
		2 * (qxy - qwz),		1 - 2 * (qxx +  qzz),	2 * (qyz + qwx),		0,
		2 * (qxz + qwy),		2 * (qyz - qwx),		1 - 2 * (qxx +  qyy),	0,
		0,						0,						0,						1);

	mat4 translate = mat4(
	1.0,	0.0,	0.0,	0.0,
	0.0,	1.0,	0.0,	0.0,
	0.0,	0.0,	1.0,	0.0,
	pos.x,	pos.y,	pos.z,	1.0);

	mat4 scale = mat4(
	4.0,	0.0,	0.0,	0.0,
	0.0,	4.0,	0.0,	0.0,
	0.0,	0.0,	4.0,	0.0,
	0.0,	0.0,	0.0,	4.0);

	mat4 ModelMat = scale*translate*rotateQ;

	M = ModelMat;

	mat4 MVP = VP*ModelMat;
	gl_Position = MVP * vec4(vertex,1.0);

	//Vertex position
	vertexPos = (ModelMat * vec4(vertex,1.0)).xyz;

	//Vertex normal relative to camera
	//cameraNormal =normalize(( V * ModelMat * vec4(normal,0)).xyz);
	Normal = normal;

	const vec3 pallete[11] = vec3[11](
		vec3(0,				0,				0.564705882),
		vec3(0,				0.058823529,	1),
		vec3(0,				0.564705882,	1),
		vec3(0.058823529,	1,				0.933333333),
		vec3(0.564705882,	1,				0.439215686),
		vec3(1,				0.933333333,	0),
		vec3(1,				0.439215686,	0),
		vec3(0.933333333,	0,				0),
		vec3(0.498039216,	0,				0),
		vec3(0.498039216,	0,				0),
		vec3(0.498039216,	0,				0));

	const vec3 pallete2[7] = vec3[7](
		vec3(1,				0,				0),
		vec3(1,				1,				0),
		vec3(0,				1,				0),
		vec3(0,				1,				1),
		vec3(0,				0,				1),
		vec3(1,				0,				1),
		vec3(1,				0,				0));


	float red, green, blue;

	int index = int(floor((spin.z*0.5+0.5)*10));

	red   = pallete[index].r + ((spin.z*0.5+0.5)*10-index)*(pallete[index+1].r-pallete[index].r);
	green = pallete[index].g + ((spin.z*0.5+0.5)*10-index)*(pallete[index+1].g-pallete[index].g);
	blue  = pallete[index].b + ((spin.z*0.5+0.5)*10-index)*(pallete[index+1].b-pallete[index].b);


	float theta = (atan(spin.y,spin.x)+3.1415926)/(3.1415926*2);

	//index = int(floor((theta)*7));

	//red   = pallete2[index].r + (theta*7-index)*(pallete2[index+1].r-pallete2[index].r);
	//green = pallete2[index].g + (theta*7-index)*(pallete2[index+1].g-pallete2[index].g);
	//blue  = pallete2[index].b + (theta*7-index)*(pallete2[index+1].b-pallete2[index].b);


	if (LightPos.z == 0){
		red = 1;
		green =1;
		blue  =1;
	}

	if (index < 0){
		red = 1;
		green =1;
		blue  =1;
	}

//	if (spin.z>0.8){
//			red = 0.0;
//			green = 0.0;
//			blue = 1.0;
//		}
//		else if (spin.z >= 0.0){
//			red = 1.0 - spin.z*1.2;
//			green = 1.0 - spin.z*1.2;
//			blue = 1.0;
//		}
//		else if (spin.z >= -0.8){
//			red = 1.0;
//			green = 1.0 + spin.z*1.2;
//			blue = 1.0 + spin.z*1.2;
//		}
//		else if (spin.z<-0.8){
//			red = 1.0;
//			green = 0.0;
//			blue = 0.0;
//		}
//		else{
//			red = 1.0;
//			green = 1.0;
//			blue = 1.0;
//		}
//
//	if (blue<0.0) blue = 0.0;
//	if (red<0.0) red = 0.0;
//	if (green<0.0) green = 0.0;

	Color = vec3(red, green, blue);

}
