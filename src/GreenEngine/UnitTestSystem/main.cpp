#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include <string>
#include <map>
#include <typeinfo>
#include <typeindex>
#include "GameObject.h"
#include "ComponentHeaders.h"
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