#include "Vector3.h" 
Vector3::Vector3() 
{ 
    this->x = 0; 
    this->y = 0; 
    this->z = 0; 
}; 
Vector3::Vector3(GLfloat x, GLfloat y, GLfloat z) 
{ 
    this->x = x; 
    this->y = y; 
    this->z = z; 
}; 
Vector3::Vector3(const Vector3 &obj) 
{ 
    this->x = obj.x; 
    this->y = obj.y; 
    this->z = obj.z;
};