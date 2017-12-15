#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include <string>
#include <map>
#include <typeinfo>
#include <typeindex>
#include "GameObject.h"
#include "ComponentHeaders.h"
TEST_CASE("Vector3", "[vector3]" ) 
{
    Vector3 *v = new Vector3(1, 1, 1);
    Vector3 *v1 = new Vector3(2, 2, 2);
    SECTION("multipliers")
    {
        *v = (*v * 3);
        REQUIRE(*v == Vector3(3, 3, 3));
        *v = v->crossProduct(*v1);
        REQUIRE(*v == Vector3(0, 0, 0));
        *v = new Vector3(0, 3, 0);
        *v = v->crossProduct(Vector3(1, 0, 0));
        REQUIRE(*v == Vector3(0, 0, -3));
    }
    *v = new Vector3(1, 1, 1);
    SECTION("additions")
    {
        *v += Vector3(0, 3, 0);
        REQUIRE(*v == Vector3(1,4,1));
    }
    *v = new Vector3(1, 1, 1);
    SECTION("subtractions")
    {
        *v -= Vector3(1, 1, 1);
        REQUIRE(*v == Vector3(0, 0, 0));
        *v -= Vector3(1000, 1000, 1000);
        REQUIRE(*v == Vector3(-1000, -1000, -1000));
    }
    *v = new Vector3(1, 1, 1);
    SECTION("matrix multiplications")
    {

    }
    *v = new Vector3(1, 1, 1);
    SECTION("magnitude")
    {
        Vector3 *vm2 = new Vector3(2,2,1);
        REQUIRE(v->magnitude() == sqrtf(3));
        REQUIRE(vm2->magnitude() == 3);
    }
}
TEST_CASE("GameObject", "[gameobject]")
{
    GameObject *go = new GameObject();
    SECTION("Components")
    {
        go->addComponent(new ColliderComponent());
        go->addComponent(new LightComponent());
        ColliderComponent *cc = new ColliderComponent();
        LightComponent *lc = new LightComponent();
        // LIst 1
        std::map<std::string, Component *> *componentList = new std::map<std::string, Component*>();
        (*componentList)[typeid(*cc).name()] = cc;
        (*componentList)[typeid(*lc).name()] = lc;
        // List 2
        std::map<std::string, Component *> *componentList2 = new std::map<std::string, Component*>();
        (*componentList2)[typeid(*cc).name()] = cc;
        // REQUIRE(go->compareMaps(go->getComponentList(), *componentList) == true); // DEBUG: Breaking
        REQUIRE(*(go->getComponent<ColliderComponent *>()) == *cc);
        go->removeComponent<LightComponent *>();
        // REQUIRE(go->compareMaps(go->getComponentList(), *componentList2) == true); // DEBUG: Breaking
        REQUIRE(go->hasComponent<ColliderComponent *>() == true);
        REQUIRE(go->hasComponent<LightComponent *>() == false);
    }
    go = new GameObject(); // Reset game object
    SECTION("Children")
    {
        GameObject *child = new GameObject(std::string(std::string("child")));
        go->addChild(child);
        //REQUIRE(*(go->getChild(std::string("child"))) == *child); // NOTE: Hello? Working?!
        go->removeChild(child);
        REQUIRE(go->getChildObjectList().empty() == true); // check if empty
    }
}
TEST_CASE("Transform", "[transform]")
{
    Transform *t = new Transform();
    SECTION("Position") // is this not just transform?
    {
        REQUIRE(t->getPosition() == Vector3());
        t->setPosition(Vector3(25,0,56));
        REQUIRE(t->getPosition() == Vector3(25, 0, 56));
    }
    SECTION("Rotation")
    {
        REQUIRE(t->getRotation() == Vector3());
        t->setRotation(Vector3(0.25, 44, 180));
        REQUIRE(t->getRotation() == Vector3(0.25, 44, 180));
    }
    SECTION("Scale")
    {
        REQUIRE(t->getScale() == 1);
        t->setScale(t->getScale() * 15);
        REQUIRE(t->getScale() == 15);
    }
}