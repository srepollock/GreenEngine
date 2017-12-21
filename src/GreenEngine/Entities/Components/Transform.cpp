	#include "Transform.h"
Transform::Transform()
{
    this->_position = this->_initialPosition = new Vector3();
	this->_rotation = new Vector3();
	this->_orientation = MakeQFromEulerAngles(Vector3(0, 0, 0));
    this->_scale = 1;
	this->_forward = Vector3(0, 0, 1);
	this->_right = Vector3(1, 0, 0);
	this->_up = Vector3(0, 1, 0);
	this->_distanceToParent = Vector3();
	adjustDirections();
};
Transform::Transform(Vector3 _position, Vector3 _rotation, GLfloat _scale)
{
    this->_position = this->_initialPosition = _position;
	this->_rotation = _rotation;
	this->_orientation = MakeQFromEulerAngles(_rotation);
    this->_scale = _scale;
	this->_forward = Vector3(0, 0, 1);
	this->_right = Vector3(1, 0, 0);
	this->_up = Vector3(0, 1, 0);
	this->_distanceToParent = Vector3();
	adjustDirections();
};
Transform::Transform(Vector3 *_position, Vector3 *_rotation, GLfloat _scale)
{
	this->_position = this->_initialPosition = _position;
	this->_rotation = *_rotation;
	this->_orientation = MakeQFromEulerAngles(*_rotation);
    this->_scale = _scale;
	this->_forward = Vector3(0, 0, 1);
	this->_right = Vector3(1, 0, 0);
	this->_up = Vector3(0, 1, 0);
	this->_distanceToParent = Vector3();
	adjustDirections();

};
Transform::Transform(const Transform &obj)
{
	this->_position = obj._position;
	this->_initialPosition = obj._initialPosition;
	this->_rotation = obj._rotation;
	this->_orientation = obj._orientation;
    this->_scale = obj._scale;
	this->_forward = obj._forward;
	this->_right = obj._right;
	this->_up = obj._up;
	this->_distanceToParent = obj._distanceToParent;
	adjustDirections();
};
void Transform::setPosition(Vector3 _position)
{
    this->_position = _position;
};
void Transform::setInitialPosition(Vector3 _initialPosition)
{
	this->_initialPosition = _initialPosition;
};
void Transform::setRotation(Vector3 _rotation)
{
    this->_rotation = _rotation;
	adjustDirections();
};
void Transform::setScale(GLfloat _scale)
{
    this->_scale = _scale;
};
void Transform::setDistanceToParent(Vector3 _distanceToParent)
{
	this->_distanceToParent = _distanceToParent;
};
Vector3 Transform::getPosition()
{
    return this->_position;
};
Vector3 Transform::getInitialPosition()
{
	return this->_initialPosition;
};
Vector3 Transform::getRotation()
{
    return this->_rotation;
};
GLfloat Transform::getScale()
{
    return this->_scale;
};
Vector3 Transform::getForward()
{
	return this->_forward;
};
Vector3 Transform::getDistanceToParent()
{
	return this->_distanceToParent;
};
void Transform::adjustDirections() 
{
	this->_forward = QVRotate(this->_orientation, Vector3(0, 0, 1)).normalize();
	this->_right = QVRotate(this->_orientation, Vector3(1, 0, 0)).normalize();
	this->_up = QVRotate(this->_orientation,Vector3(0, 1, 0)).normalize();
};

/**
* <summary>
* Rotate the object by a set amount. This rotation is in radians only,
* and is only ever increasing the rotation.
* Please specify positive/negative when calling. e.g., rotate(&go, -1.2);
* </summary>
*/
void Transform::rotateAround(Vector3 distance, Vector3 objectPos, Quaternion rotation) 
{
	/*Vector3 newDistance = distance.matrixMulti(glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z));
	this->_position = newDistance + objectPos;*/
	(*this)._position = QVRotate(rotation, distance) + objectPos;
	//return newDistance;
};
void Transform::rotate(Vector3 amount)
{
	this->_orientation += this->_orientation * MakeQFromEulerAngles(amount);
	this->_orientation.Normalize();
	this->adjustDirections();
};
void Transform::rotateQuat(Vector3 axis, GLfloat amount) 
{
	this->_orientation += this->_orientation * MakeQFromEulerAngles(axis * amount);
	this->_orientation.Normalize();
	this->adjustDirections();
};
void Transform::rotateX(GLfloat angle)
{
	adjustDirections();
	this->_rotation.x += angle;
};
void Transform::rotateY(GLfloat angle)
{
	adjustDirections();
	this->_rotation.y += angle;
};
void Transform::rotateZ(GLfloat angle)
{
	adjustDirections();
	this->_rotation.z += angle;
};

/**
*  <summary>
*  Move the game object in a direciton. The translation should be modified by the delta time.
*  </summary>
*/
void Transform::translate(Vector3 translation)
{
	this->_position += translation;
};

void Transform::translateForward(GLfloat num)
{
	this->_position += _forward * num;
};

void Transform::translateRight(GLfloat num) 
{
	this->_position += _right * num;
};
/**
*  <summary>
*  Move the game object in a direciton. Each axis should be modified by the delta time.
*  </summary>
*/
void Transform::translate(GLfloat x, GLfloat y, GLfloat z)
{
	this->_position += Vector3(x, y, z);
};