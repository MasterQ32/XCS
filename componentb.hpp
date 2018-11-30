#ifndef COMPONENTB_HPP
#define COMPONENTB_HPP

#include "ecs.hpp"

class ComponentB : public ecs::Component
{
public:
	ComponentB();
};

ECS_REGISTER_COMPONENT("Komponente B", ComponentB)

#endif // COMPONENTB_HPP
