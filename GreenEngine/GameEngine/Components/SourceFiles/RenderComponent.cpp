#include "RenderComponent.h"
RenderComponent::RenderComponent()
{
    this->_modelName = "";
    this->_albedoName = "";
    this->_normalName = "";
    this->_smoothness = 0;
};
RenderComponent::RenderComponent(std::string _modelName, std::string _albedoName, std::string _normalName, GLfloat _smoothness)
{
    this->_modelName = _modelName;
    this->_albedoName = _albedoName;
    this->_normalName = _normalName;
    this->_smoothness = _smoothness;
};
RenderComponent::RenderComponent(const RenderComponent &obj)
{
    this->_modelName = obj._modelName;
    this->_albedoName = obj._albedoName;
    this->_normalName = obj._normalName;
    this->_smoothness = obj._smoothness;
};
void RenderComponent::setModelName(std::string _name)
{
    this->_modelName = _name;
};
void RenderComponent::setAlbedoName(std::string _name)
{
    this->_albedoName = _name;
};
void RenderComponent::setNormalName(std::string _name)
{
    this->_normalName = _name;
};
void RenderComponent::setSmoothness(GLfloat _value)
{
    this->_smoothness = _value;
};
std::string RenderComponent::getModelName()
{
    return this->_modelName;
};
std::string RenderComponent::getAlbedoName()
{
    return this->_albedoName;
};
std::string RenderComponent::getNormalName()
{
    return this->_normalName;
};
GLfloat RenderComponent::getSmoothness() 
{
    return this->_smoothness;
};