#include <xcs>

#include "componenta.hpp"
#include "componentb.hpp"

#include <iostream>

struct SomeComponent
{
	int a, b;
};

XCS_REGISTER_COMPONENT(SomeComponent)

struct SomeSystem
{
	xcs::universe & universe;

	SomeSystem(xcs::universe & uni) : universe(uni)
	{
	}

	void update()
	{
		for(auto element : xcs::get_components<SomeComponent>(universe))
		{
			std::cout
				<< element.entity.id
			    << " => ("
			    << element.component.a
			    << ", " << element.component.b
			    << ")"
			    << std::endl
			    ;
		}
	}
};

int main()
{
	xcs::universe scene;
	SomeSystem system(scene);

	xcs::entity ent = scene.create_entity();

	xcs::entity ent2 = *scene.create_entity_with_id(xcs::unique_id{42});

	xcs::entity ent3 = scene.create_entity();

	xcs::add_component(ent3, "SomeComponent");

	xcs::add_component<SomeComponent>(ent2).a = 42;

	auto & c = xcs::add_component<SomeComponent>(ent);
	c.a = 10;
	c.b = 20;

	std::cout << ent.id << ":SomeComponent => " << xcs::get_component<SomeComponent>(ent) << std::endl;

	system.update();

	c.b = 30;

	system.update();

	xcs::remove_component<SomeComponent>(ent);
	std::cout << ent.id << ":SomeComponent => " << xcs::get_component<SomeComponent>(ent) << std::endl;

	std::cout << "pre-delete:" << std::endl;
	system.update();

	scene.destroy_entity(ent2);

	std::cout << "post-delete:" << std::endl;
	system.update();

	return 0;
}
