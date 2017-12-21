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
#define TOL 0.0001
class Vector3 
{ 
public: 
  Vector3(); 
  Vector3(GLfloat x, GLfloat y, GLfloat z);
  Vector3(const Vector3 &obj); 
  ~Vector3();
  GLfloat x; 
  GLfloat y; 
  GLfloat z; 
  Vector3& operator=(const Vector3* obj);
  Vector3& operator=(const Vector3& obj);
  Vector3& operator+=(const Vector3* obj);
  Vector3& operator+=(const Vector3& obj);
  Vector3& operator-=(const Vector3* obj);
  Vector3& operator-=(const Vector3& obj);
  Vector3& operator*=(const Vector3* obj);
  Vector3& operator*=(const Vector3& obj);
  Vector3& operator/=(const Vector3* obj);
  Vector3& operator/=(const Vector3& obj);
  bool operator==(const Vector3& vec) const;
  Vector3 operator*(const float_t& num);
  Vector3 operator/(const float_t& num);
  Vector3 operator+(const Vector3* obj);
  Vector3 operator+(const Vector3& obj);
  Vector3 operator-(const Vector3* obj);
  Vector3 operator-(const Vector3& obj);
  Vector3 operator-();
  GLfloat magnitude();
  Vector3 matrixMulti(glm::mat3x3 matrix);
  Vector3 normalize();
  GLfloat dotProduct(Vector3 vec);
  Vector3 crossProduct(Vector3 vec);
};
inline Vector3::~Vector3(){};

inline Vector3::Vector3()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
};
inline Vector3::Vector3(GLfloat x, GLfloat y, GLfloat z)
{
	this->x = x;
	this->y = y;
	this->z = z;
};
inline Vector3::Vector3(const Vector3& obj)
{
	this->x = obj.x;
	this->y = obj.y;
	this->z = obj.z;
};
inline Vector3& Vector3::operator=(const Vector3* obj)
{
	this->x = obj->x;
	this->y = obj->y;
	this->z = obj->z;
	return *this;
};
inline Vector3& Vector3::operator=(const Vector3& obj)  
  { 
    this->x = obj.x; 
    this->y = obj.y; 
    this->z = obj.z; 
    return *this; 
  };
inline Vector3& Vector3::operator+=(const Vector3* obj)
{
	this->x += obj->x;
	this->y += obj->y;
	this->z += obj->z;
	return *this;
};
inline Vector3& Vector3::operator+=(const Vector3& obj)
{
	this->x += obj.x;
	this->y += obj.y;
	this->z += obj.z;
	return *this;
};
inline Vector3& Vector3::operator-=(const Vector3* obj)
{
	this->x -= obj->x;
	this->y -= obj->y;
	this->z -= obj->z;
	return *this;
};
inline Vector3& Vector3::operator-=(const Vector3& obj)
{
	this->x -= obj.x;
	this->y -= obj.y;
	this->z -= obj.z;
	return *this;
};
inline Vector3& Vector3::operator*=(const Vector3* obj) 
{
	this->x *= obj->x;
	this->y *= obj->y;
	this->z *= obj->z;
	return *this;
};
inline Vector3& Vector3::operator*=(const Vector3& obj)
{
	this->x *= obj.x;
	this->y *= obj.y;
	this->z *= obj.z;
	return *this;
};
inline Vector3& Vector3::operator/=(const Vector3* obj)
{
	this->x /= obj->x;
	this->y /= obj->y;
	this->z /= obj->z;
	return *this;
};
inline Vector3& Vector3::operator/=(const Vector3& obj)
{
	this->x /= obj.x;
	this->y /= obj.y;
	this->z /= obj.z;
	return *this;
};
inline bool Vector3::operator==(const Vector3& vec) const
{
	return  x == vec.x &&
            y == vec.y &&
            z == vec.z;
};
inline Vector3 Vector3::operator*(const float_t& num)
{
	return Vector3(this->x * num, this->y * num, this->z * num);
};
inline Vector3 Vector3::operator/(const float_t& num)
{
	return Vector3(this->x / num, this->y / num, this->z / num);
};
inline Vector3 Vector3::operator+(const Vector3* vec)
{
	return Vector3(this->x + vec->x , this->y + vec->y, this->z + vec->z);
};
inline Vector3 Vector3::operator+(const Vector3& vec)
{
	return Vector3(this->x + vec.x, this->y + vec.y, this->z + vec.z);
};
inline Vector3 Vector3::operator-(const Vector3* vec)
{
	return Vector3(this->x - vec->x, this->y - vec->y, this->z - vec->z);
};
inline Vector3 Vector3::operator-(const Vector3& vec)
{
	return Vector3(this->x - vec.x, this->y - vec.y, this->z - vec.z);
};
inline Vector3 Vector3::operator-() 
{
	return Vector3(-this->x, -this->y, -this->z);
};
inline GLfloat Vector3::magnitude()
{
	return (GLfloat)sqrtf(powf(this->x, 2) + powf(this->y, 2) + powf(this->z, 2));
};
inline Vector3 Vector3::matrixMulti(glm::mat3x3 matrix) 
{
	glm::vec3 temp = glm::vec3(this->x, this->y, this->z) * matrix;
	return Vector3(temp.x, temp.y, temp.z);
};
inline Vector3 Vector3::normalize() 
{
	float num = sqrtf(powf(this->x, 2) + powf(this->y, 2) + powf(this->z, 2));
	if (num <= TOL) num = 1;
	return Vector3(this->x / num, this->y / num, this->z / num);
};
inline GLfloat Vector3::dotProduct(Vector3 vec) 
{
	return this->x * vec.x + this->y * vec.y + this->z * vec.z;
};
inline Vector3 Vector3::crossProduct(Vector3 vec)
{
	// a(x y z | x)
	// b(x y z | x)
	return Vector3(
		(this->y * vec.z) - (vec.y * this->z),
		(this->z * vec.x) - (vec.z * this->x),
		(this->x * vec.y) - (vec.x * this->y)
	);
};