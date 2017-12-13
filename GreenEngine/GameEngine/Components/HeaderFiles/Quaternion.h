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
#define PI 3.14159265f
#define ATANLIMIT 0.0001f
#include "Vector3.h"

class Quaternion
{
public:
	Quaternion();
	Quaternion(float n, float x, float y, float z);
	Quaternion(float n, Vector3 vec);
	~Quaternion();
	GLfloat Magnitude();
	Vector3 getVector();
	GLfloat getScalar();
	Quaternion operator+=(Quaternion q);
	Quaternion operator-=(Quaternion q);
	Quaternion operator*=(GLfloat s);
	Quaternion operator/=(GLfloat s);
	Quaternion operator~(void) { return Quaternion(_n, -_v.x, -_v.y, -_v.z); };
	Quaternion operator+(Quaternion q1);

	Quaternion operator-(Quaternion q1);
	Quaternion operator*(Quaternion q);
	Quaternion operator*(GLfloat s);
	Quaternion operator/(GLfloat s);
	GLfloat QGetAngle();
	Vector3 QGetAxis();
	Quaternion& MakeQFromEulerAngles(float x, float y, float z);
	Vector3 MakeEulerAnglesFromQ();

	Quaternion& CreateFromAxisAngle(Vector3 vec, GLfloat angle);

	Quaternion & Normalize();
	GLfloat _n;
	Vector3 _v;
private:

};

inline Quaternion::Quaternion()
{
	_n = 1;
	_v = Vector3(0, 0, 0);
};

inline Quaternion::Quaternion(float n, float x, float y, float z)
{
	_n = n;
	_v = Vector3(x, y, z);
};

inline Quaternion::Quaternion(float n, Vector3 vec) {
	_n = n;
	_v = vec;
}

inline Quaternion::~Quaternion()
{
};

inline GLfloat Quaternion::Magnitude()
{
	return (GLfloat)sqrtf((_n*_n) + (_v.x*_v.x) + (_v.y*_v.y) + (_v.z*_v.z));
};

inline Vector3 Quaternion::getVector() 
{
	return _v;
};

inline GLfloat Quaternion::getScalar() 
{
	return _n;
};

inline Quaternion Quaternion::operator+=(Quaternion q)
{
	_n += q._n;
	_v.x += q._v.x;
	_v.y += q._v.y;
	_v.z += q._v.z;
	return *this;
};

inline Quaternion Quaternion::operator-=(Quaternion q)
{
	_n -= q._n;
	_v.x -= q._v.x;
	_v.y -= q._v.y;
	_v.z -= q._v.z;
	return *this;
};

inline Quaternion Quaternion::operator*=(GLfloat s)
{
	_n *= s;
	_v.x *= s;
	_v.y *= s;
	_v.z *= s;
	return *this;
};

inline Quaternion Quaternion::operator/=(GLfloat s)
{
	_n /= s;
	_v.x /= s;
	_v.y /= s;
	_v.z /= s;
	return *this;
};

inline Quaternion Quaternion::operator+(Quaternion q1)
{
	return Quaternion((this->_n + q1._n), (this->_v.x + q1._v.x), (this->_v.y + q1._v.y), (this->_v.z + q1._v.z));
};

inline Quaternion Quaternion::operator-(Quaternion q1)
{
	return Quaternion((this->_n - q1._n), (this->_v.x - q1._v.x), (this->_v.y - q1._v.y), (this->_v.z - q1._v.z));
};

inline Quaternion Quaternion::operator*(Quaternion q)
{
	return Quaternion((this->_n * q._n - this->_v.x * q._v.x - this->_v.y * q._v.y - this->_v.z * q._v.z),
		(this->_n * q._v.x + this->_v.x * q._n + this->_v.y * q._v.z - this->_v.z * q._v.y),
		(this->_n * q._v.y + this->_v.y * q._n + this->_v.z * q._v.x - this->_v.x * q._v.z),
		(this->_n * q._v.z + this->_v.z * q._n + this->_v.x * q._v.y - this->_v.y * q._v.x));
};

inline Quaternion Quaternion::operator*(GLfloat s) 
{
	return Quaternion(this->_n * s, this->_v.x * s, this->_v.y * s, this->_v.z * s);
};

inline Quaternion operator*(Quaternion q, Vector3 v)
{
	Vector3 vec = q.getVector();
	return Quaternion(-(vec.x * v.x + vec.y * v.y + vec.z * v.z),
		(q.getScalar() * v.x + vec.y * v.z - vec.z * v.y),
		(q.getScalar() * v.y + vec.z * v.x - vec.x * v.z),
		(q.getScalar() * v.z + vec.x * v.y - vec.y * v.x));
};
inline Quaternion operator*(Vector3 v, Quaternion q)
{
	Vector3 vec = q.getVector();
	return Quaternion(-(vec.x * v.x + vec.y * v.y + vec.z * v.z),
		(q.getScalar() * v.x + vec.z * v.y - vec.y * v.z),
		(q.getScalar() * v.y + vec.x * v.z - vec.z * v.x),
		(q.getScalar() * v.z + vec.y * v.x - vec.x * v.y));
};

inline Quaternion Quaternion::operator/(GLfloat s)
{
	return Quaternion(this->_n / s, this->_v.x / s, this->_v.y / s, this->_v.z / s);
};

inline GLfloat Quaternion::QGetAngle() {
	return (GLfloat)(2 * acosf(this->_n));
};

inline Vector3 Quaternion::QGetAxis() {
	Vector3 v;
	float m;
	v = this->getVector();
	m = v.magnitude();

	if (m <= TOL) {
		return Vector3();
	}
	else {
		return v / m;
	}
};

inline Quaternion QRotate(Quaternion q1, Quaternion q2) 
{
	return q1 * q2 * (~q1);
};

inline Vector3 QVRotate(Quaternion q, Vector3 v) 
{
	Quaternion t;
	t = q * v * ~(q);
	return t.getVector();
};

inline Quaternion& Quaternion::MakeQFromEulerAngles(float x, float y, float z)
{
	
	double pitch = x; // pitch = y
	double yaw = y; // yaw = z
	double roll = z; // roll = x

	double cyaw, cpitch, croll, syaw, spitch, sroll;
	double cyawcpitch, syawspitch, cyawspitch, syawcpitch;

	cyaw = cos(0.5f * yaw);
	cpitch = cos(0.5f * pitch);
	croll = cos(0.5f * roll);
	syaw = sin(0.5f * yaw);
	spitch = sin(0.5f * pitch);
	sroll = sin(0.5f * roll);

	cyawcpitch = cyaw * cpitch;
	syawspitch = syaw * spitch;
	cyawspitch = cyaw * spitch;
	syawcpitch = syaw * cpitch;

	this->_n = (GLfloat)(cyawcpitch * croll + syawspitch * sroll);
	this->_v.x = (GLfloat)(syawcpitch * sroll + cyawspitch * croll);
	this->_v.y = (GLfloat)(syawcpitch * croll - cyawspitch * sroll);
	this->_v.z = (GLfloat)(cyawcpitch * sroll - syawspitch * croll);
	return this->Normalize();
};

inline Quaternion& MakeQFromEulerAngles(Vector3 vec)
{
	Quaternion q;
	double yaw = vec.y; // yaw = z
	double pitch = vec.x; // pitch = y
	double roll = vec.z; // roll = x

	double cyaw, cpitch, croll, syaw, spitch, sroll;
	double cyawcpitch, syawspitch, cyawspitch, syawcpitch;

	cyaw = cos(0.5f * yaw);
	cpitch = cos(0.5f * pitch);
	croll = cos(0.5f * roll);
	syaw = sin(0.5f * yaw);
	spitch = sin(0.5f * pitch);
	sroll = sin(0.5f * roll);

	cyawcpitch = cyaw * cpitch;
	syawspitch = syaw * spitch;
	cyawspitch = cyaw * spitch;
	syawcpitch = syaw * cpitch;

	q._n = (GLfloat)(cyawcpitch * croll + syawspitch * sroll);
	q._v.x = (GLfloat)(syawcpitch * sroll + cyawspitch * croll);
	q._v.y = (GLfloat)(syawcpitch * croll - cyawspitch * sroll);
	q._v.z = (GLfloat)(cyawcpitch * sroll - syawspitch * croll);
	return q.Normalize();
};

inline Vector3 Quaternion::MakeEulerAnglesFromQ() 
{
	Vector3 u;

	GLfloat r13, r21, r22, r23, r31, r32, r33;
	GLfloat q00, q11, q22, q33;
	q00 = this->_n * this->_n;
	q11 = this->_v.x * this->_v.x;
	q22 = this->_v.y * this->_v.y;
	q33 = this->_v.z * this->_v.z;

	r22 = q00 - q11 + q22 - q33;
	r21 = 2 * (this->_v.x * this->_v.y + this->_n * this->_v.z);
	r23 = 2 * (this->_v.y * this->_v.z - this->_n * this->_v.x);
	r31 = 2 * (this->_v.x * this->_v.z + this->_n * this->_v.y);


	r33 = q00 - q11 - q22 + q33;
	float tmp = fabs(r23);
	if (tmp > 0.999999)
	{
		r13 = 2 * (this->_v.z*this->_v.x - this->_n*this->_v.y);
		r32 = 2 * (this->_v.z*this->_v.y + this->_n*this->_v.x);
		u.y = (float)atan2(-r13, -r23*r32);
		u.x = (float)(-(PI / 2) * r23 / tmp);
		u.z = 0.0f;
		return u;
	}
	u.x = asin(-r23);
	u.y = atan2(r31, r33);
	u.z = atan2(r21,r22);
	return u;
};

inline Quaternion& Quaternion::CreateFromAxisAngle(Vector3 vec, GLfloat angle)
{
	GLfloat s = sinf(angle / 2.0f);
	this->_v.x = vec.x * s;
	this->_v.y = vec.y * s;
	this->_v.z = vec.z * s;
	this->_n = cosf(angle / 2.0f);
	return this->Normalize();
}

inline Quaternion& Quaternion::Normalize()
{
	GLfloat mag = this->Magnitude();
	this->_n /= mag;
	this->_v.x /= mag;
	this->_v.y /= mag;
	this->_v.z /= mag;
	return *this;
}