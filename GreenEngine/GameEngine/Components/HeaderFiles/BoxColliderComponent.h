#pragma once
#include "ColliderComponent.h"
#include "FileHelper.h"
#include "OBJImport.h"
class BoxColliderComponent : public ColliderComponent
{
public:
	BoxColliderComponent(std::string filename);
	void setMinX(int _x);
	void setMinY(int _y);
	void setMinZ(int _z);
	void setMaxX(int _x);
	void setMaxY(int _y);
	void setMaxZ(int _z);
	int getMinX();
	int getMinY();
	int getMinZ();
	int getMaxX();
	int getMaxY();
	int getMaxZ();
private:
	int _minX, _minY, _minZ;
	int _maxX, _maxY, _maxZ;
	void setValuesAllZero();
	void buildBoxColliderFromOBJ(std::string filename);
};