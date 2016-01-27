#include "Controls.h"
#include "Shader.h"
#include "Output.h"

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

	//Updates events
	SDL_Event event;
	SDL_PollEvent(&event);

	//Change in mouse
	SDL_GetRelativeMouseState(&xpos, &ypos);
	//is middle button down
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE)) {
		// Compute new orientation
		horizontalAngle += mouseSpeed * float(xpos);
		verticalAngle += mouseSpeed * float(ypos);
	}

	//Direction : Spherical coordinates to Cartesian coordinates conversion
	direction = {
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	};
	// Right vector
	right = {
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	};
	// Up vector
	up = glm::cross(right, direction);

	//Keyboard press
	if (keystate[SDL_SCANCODE_W])//Forwards
		position += direction * deltaTime *  speed;
	if (keystate[SDL_SCANCODE_S])//Backwards
		position += -(direction * deltaTime *  speed);
	if (keystate[SDL_SCANCODE_A])//Left
		position += -(right * deltaTime * speed);
	if (keystate[SDL_SCANCODE_D])//Right
		position += right * deltaTime * speed;
	if (keystate[SDL_SCANCODE_E])//Display frame time
		std::cout << deltaTime << std::endl;
	if (keystate[SDL_SCANCODE_M])//Unlock Mouse
		SDL_SetRelativeMouseMode(SDL_FALSE);
	if (keystate[SDL_SCANCODE_Q])//Quit
		return false;
	if (keystate[SDL_SCANCODE_Y])//RAW Image
		WindowDump();

	static int spinStep = 0;
	std::vector<glm::vec3> TempData;
	if (event.type == SDL_KEYDOWN)
	{
		if (event.key.keysym.sym == SDLK_LEFT)
		{
			sys.decreaseStep();
		}
		if (event.key.keysym.sym == SDLK_RIGHT)
		{
			sys.increaseStep();
		}
		if (event.key.keysym.sym == SDLK_1)
		{
			sys.visble(0);
		}
		if (event.key.keysym.sym == SDLK_2)
		{
			sys.visble(1);
		}
		if (event.key.keysym.sym == SDLK_3)
		{
			sys.visble(2);
		}
		if (event.key.keysym.sym == SDLK_4)
		{
			sys.visble(3);
		}
		if (event.key.keysym.sym == SDLK_5)
		{
			sys.visble(4);
		}
		if (event.key.keysym.sym == SDLK_6)
		{
			sys.visble(5);
		}
	}

	//Tweak Bar values
	glm::mat4 RotationMatrix = mat4_cast(Orientation);
	glm::mat4 TranslationMatrix = translate(mat4(), Translation);
	glm::mat4 ScalingMatrix = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 1000.0f);
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
