#pragma once 
#ifdef __APPLE__
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <SDL2/SDL.h> 
#include <SDL2/SDL_opengl.h> 
#elif defined _WIN32 || defined _WIN64
#include <glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtx\euler_angles.hpp>
#include <SDL.h> 
#endif 
class Vector3 
{ 
public: 
  Vector3(); 
  Vector3(GLfloat x, GLfloat y, GLfloat z); 
  Vector3(const Vector3 &obj); 
  Vector3& operator=(const Vector3* obj)  
  { 
    this->x = obj->x; 
    this->y = obj->y; 
    this->z = obj->z; 
    return *this; 
  }; 
  Vector3& operator=(const Vector3& obj)  
  { 
    this->x = obj.x; 
    this->y = obj.y; 
    this->z = obj.z; 
    return *this; 
  };
  Vector3& operator+=(const Vector3* obj)
  {
    this->x = this->x + obj->x;
    this->y = this->y + obj->y;
    this->z = this->z + obj->z;
	return *this;
  };
  Vector3& operator+=(const Vector3& obj)
  {
    this->x = this->x + obj.x;
    this->y = this->y + obj.y;
    this->z = this->z + obj.z;
	return *this;
  };
  Vector3& operator-=(const Vector3* obj)
  {
    this->x = this->x - obj->x;
    this->y = this->y - obj->y;
    this->z = this->z - obj->z;
	return *this;
  };
  Vector3& operator-=(const Vector3& obj)
  {
    this->x = this->x - obj.x;
    this->y = this->y - obj.y;
    this->z = this->z - obj.z;
	return *this;
  };
  Vector3& operator*(const float_t& mult)
  {
	  this->x = this->x * mult;
	  this->y = this->y * mult;
	  this->z = this->z * mult;
	  return *this;
  };
  Vector3& operator*(Vector3 vec)
  {
	  this->x *= vec.x;
	  this->y *= vec.y;
	  this->z *= vec.z;
	  return *this;
  }
  GLfloat magnitude()
  {
	  float x = powf(this->x, 2);
	  float y = powf(this->y, 2);
	  float z = powf(this->z, 2);
	  return (GLfloat)sqrtf(x + y + z);
  }
  Vector3& matrixMulti(glm::mat4x4 matrix) {
	  glm::vec4 temp = glm::vec4(this->x, this->y, this->z, 1) * matrix;
	  this->x = temp.x; this->y = temp.y, this->z = temp.z;
	  return *this;
  }
  GLfloat x; 
  GLfloat y; 
  GLfloat z; 
};