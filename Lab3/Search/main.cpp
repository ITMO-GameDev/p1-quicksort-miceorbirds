#include <iostream>
#include "search.h"

int main()
{
	Dictionary<std::string, int> npc;
	npc.put("health", 10);
	npc.put("armor", 20);
	npc.put("ammo", 5);

	for (auto it = npc.iterator(); it.hasNext(); it.next())
		std::cout << it.key() << " " << it.get() << std::endl;
}