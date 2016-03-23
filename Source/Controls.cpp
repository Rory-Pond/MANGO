#include "Controls.h"
#include "Shader.h"
#include "Output.h"
#include "Toggle.h"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;
glm::mat4 ModelMatrix;

//vec3 Positon = { 0, 0, 0 };
vec3 Translation = { 0, 0, 0 };
quat Orientation = { 0, 0, 0, 1 };

vec3 CameraPos;

glm::mat4 getModelMatrix() { return ModelMatrix; }
glm::mat4 getViewMatrix() { return ViewMatrix; }
glm::mat4 getProjectionMatrix() { return ProjectionMatrix; }
glm::mat4 getModelViewProjectionMatrix() { return ProjectionMatrix*ViewMatrix*ModelMatrix; }

GLint UniformMVP, UniformV, UniformLightPos;

void initUniforms()
{
	GLuint shaderProgram = getShader();
	// Set up MVP uniform

	UniformMVP = glGetUniformLocation(shaderProgram, "VP");
	UniformV = glGetUniformLocation(shaderProgram, "V");
	UniformLightPos = glGetUniformLocation(shaderProgram, "LightPos");
}

bool HandleEvents(System& sys)
{
	// Compute time difference between current and last frame
	static double lastTime = SDL_GetTicks();
	double currentTime = SDL_GetTicks();
	float deltaTime = float(currentTime - lastTime);
	lastTime = currentTime;
	static bool Mode = true;

	//Movement varibles
	glm::vec3 direction, right, up;
	float ParallelVel = 0;
	float PerpendicularVel = 0;
	static glm::vec3 position = glm::vec3(0, 0, -(glm::length(sys.sysInfo.dimensions)));

	//Keyboard
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);

	//Camera varibles
	float FoV = 45.0f;
	int xpos, ypos;
	static float horizontalAngle = 0.0f;
	static float verticalAngle = 0.0f;
	float mouseSpeed = 0.005f;
	float speed = glm::length(sys.sysInfo.dimensions)*0.001f;

	static glm::mat4 RotationMatrix = mat4_cast(Orientation);


	//Change in mouse
	SDL_GetRelativeMouseState(&xpos, &ypos);
	//is middle button down
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE)) {
		// Compute new orientation
		horizontalAngle += mouseSpeed * float(xpos);
		verticalAngle += mouseSpeed * float(ypos);
	}

	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
		// Compute new orientation
//		std::cout << RotationMatrix << std::endl;

		//if (xpos>0)	RotationMatrix = glm::rotate(RotationMatrix,float(1.0*xpos),up);

//		std::cout << RotationMatrix << std::endl;

		Orientation *= quat{cos(float(0.005)*float(xpos))	,sin( float(0.005)*float(xpos))	, 0 					  , 0 		};
		Orientation *= quat{cos(float(0.005)*float(ypos))	,0 							, sin( float(0.005))*float(ypos), 0 		};
	}

	//Direction : Spherical coordinates to Cartesian coordinates conversion
	direction = {
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	};
	// Right vector
	right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0.0,
		cos(horizontalAngle - 3.14f / 2.0f));
	// Up vector
	up = glm::cross(right, direction);

	static float frontCross=0.1;
	static float backCross=(glm::length(sys.sysInfo.dimensions));

	//Updates events
	SDL_Event event;
	SDL_PollEvent(&event);
	//SDL_PumpEvents();
	if (event.type == SDL_QUIT) 				return false;

	if (keystate[SDL_SCANCODE_T])				std::cout << deltaTime << std::endl; //Frame Rate

	if (keystate[SDL_SCANCODE_M])				SDL_SetRelativeMouseMode(SDL_FALSE); //Unlock Mouse
	
	if (keystate[SDL_SCANCODE_ESCAPE])			return false;  // Exit program
	
	if (keystate[SDL_SCANCODE_Y])				WindowDump(); //RAW Image


		//Keyboard press
	if (keystate[SDL_SCANCODE_LSHIFT])
	{
		if (Toggle_LEFT())							sys.decreaseStep();
		if (Toggle_RIGHT())							sys.increaseStep();

		if (keystate[SDL_SCANCODE_W])	Translation += vec3{ 1	, 0	, 0	}* deltaTime * speed;
		if (keystate[SDL_SCANCODE_S])	Translation += vec3{-1	, 0	, 0	}* deltaTime * speed;
		if (keystate[SDL_SCANCODE_A])	Translation += vec3{ 0	, 1	, 0	}* deltaTime * speed;
		if (keystate[SDL_SCANCODE_D])	Translation += vec3{ 0	,-1	, 0	}* deltaTime * speed;
		if (keystate[SDL_SCANCODE_Q])	Translation += vec3{ 0	, 0	, 1	}* deltaTime * speed;
		if (keystate[SDL_SCANCODE_E])	Translation += vec3{ 0	, 0	,-1	}* deltaTime * speed;
	}else
	{
		if (keystate[SDL_SCANCODE_LEFT])			sys.decreaseStep();
		if (keystate[SDL_SCANCODE_RIGHT])			sys.increaseStep();

		if (keystate[SDL_SCANCODE_W])	position +=  direction * deltaTime * speed;
		if (keystate[SDL_SCANCODE_S])	position +=-(direction * deltaTime * speed);
		if (keystate[SDL_SCANCODE_A])	position +=-(right 	   * deltaTime * speed);
		if (keystate[SDL_SCANCODE_D])	position +=  right 	   * deltaTime * speed;
	}


	if (keystate[SDL_SCANCODE_Z])				frontCross+=1;
	if (keystate[SDL_SCANCODE_X])				frontCross-=1;

	if (keystate[SDL_SCANCODE_C])				backCross+=1;
	if (keystate[SDL_SCANCODE_V])				backCross-=1;

	//Toggle switchs

	if (Toggle_SPACE())							Mode = !Mode;


	if (Toggle_1()) 							sys.visble(0);
	if (Toggle_2()) 							sys.visble(1);


	if (keystate[SDL_SCANCODE_I])			Orientation *= quat{cos( 0.05)	,sin( 0.05)	, 0 		, 0 		}; //Roll
	if (keystate[SDL_SCANCODE_O])			Orientation *= quat{cos(-0.05)	,sin(-0.05)	, 0 		, 0 		};
	if (keystate[SDL_SCANCODE_L])			Orientation *= quat{cos( 0.05)	,0 			, sin( 0.05), 0 		}; //Pitch
	if (keystate[SDL_SCANCODE_K])			Orientation *= quat{cos(-0.05)	,0 			, sin(-0.05), 0 		};
	if (keystate[SDL_SCANCODE_N])			Orientation *= quat{cos( 0.05)	,0 			, 0 		, sin( 0.05)}; //Yaw
	if (keystate[SDL_SCANCODE_M])			Orientation *= quat{cos(-0.05)	,0 			, 0 	 	, sin(-0.05)};

	//Ratation, Translation, and Scale
	
	glm::mat4 TranslationMatrix = translate(mat4(), Translation);
	glm::mat4 ScalingMatrix = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));

	float halfdiagonanl = 30+ glm::length(sys.sysInfo.dimensions)/2.0;

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	if (Mode){
		ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, frontCross, backCross);
	}else{
		ProjectionMatrix = glm::ortho(-halfdiagonanl, halfdiagonanl, halfdiagonanl, -halfdiagonanl, 0.1f, 500.0f);
	}
	//(left,right,bottom,top,zNear,zFar)		
	RotationMatrix = mat4_cast(Orientation);
	// Camera matrix : Position, Viewing direction, Up direction
	ViewMatrix = glm::lookAt(position, position + direction, up);
	// Model Matrix: Translation, Rotation (quaternion), scale
	ModelMatrix = TranslationMatrix * RotationMatrix * ScalingMatrix;

	//Set Uniforms
	glm::mat4 MVP = getModelViewProjectionMatrix();
	glUniformMatrix4fv(UniformMVP, 1, GL_FALSE, &MVP[0][0]);

	glm::mat4 V = getViewMatrix();
	glUniformMatrix4fv(UniformV, 1, GL_FALSE, &V[0][0]);

	glm::vec3 LightPos = glm::vec3(0, 0, (glm::length(sys.sysInfo.dimensions)));//getLightPos();
	glUniform3fv(UniformLightPos, 1, &LightPos[0]);

	if (event.type == SDL_QUIT)
		return false;
	return true;
}
