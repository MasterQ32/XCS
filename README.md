# XCS
xq's entity component system

## Example

```cpp
#include <xcs>

struct SomeComponent
{
	int a, b;
};
XCS_REGISTER_COMPONENT(SomeComponent)

namespace SomeSystem
{
	void update(xcs::universe & universe)
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

	auto & c = xcs::add_component<SomeComponent>(ent);
	c.a = 10;
	c.b = 20;

	std::cout << ent.id << ":SomeComponent => " << xcs::get_component<SomeComponent>(ent) << std::endl;

	SomeSystem::update(scene);
	return 0;
}
```
