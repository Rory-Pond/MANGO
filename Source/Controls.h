#pragma once

#include <iostream>
#include <vector>

#include "SDL2/SDL.h"
#define GLM_FORCE_RADIANS

#include "System.h"
#include "DrawGroup.h"

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;



//Handles all user input
//Returns True unless QUIT
bool HandleEvents(System& atoms);

void initUniforms();

glm::mat4 getModelMatrix();
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();
glm::mat4 getModelViewProjectionMatrix();
glm::vec3 getLightPos();
