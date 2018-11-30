#ifndef COMPONENTA_HPP
#define COMPONENTA_HPP

#include "ecs.hpp"


class ComponentA : public ecs::Component
{
public:
	ComponentA();
};

ECS_REGISTER_COMPONENT("Komponente A", ComponentA)

#endif // COMPONENTA_HPP
