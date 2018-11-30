#include "componenta.hpp"
#include "componentb.hpp"

#include <iostream>

int main()
{
	std::cout << "Components registered:" << std::endl;
	for(auto const * it = ecs::MetaComponent::first(); it != nullptr; it = it->next)
	{
		std::cout << "[@" << it->id << "] = '" << it->name << "'" << std::endl;
	}

	ecs::Entity ent;

	ent.AddComponent<ComponentA>();
	ent.AddComponent<ComponentB>();
	ent.RemoveComponent<ComponentA>();
	ent.AddComponent<ComponentA>();

	std::cout << ".A => " << ent.GetComponent<ComponentA>() << std::endl;
	std::cout << ".B => " << ent.GetComponent<ComponentB>() << std::endl;

	return 0;
}
