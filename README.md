# Unreal Engine 5 floating island generation
This project was created in a mix of blueprints and C++ for my university thesis, and covers the topic of floating island generation. It goes over the initial island creation with varied parameters, and then goes over generating levels comprised of islands, and decorating them with props.

This project was built with a design and runtime mindset, in that islands can be generated both at run time and design time, as well as the levels themselves. There are many improvements I hope to make in the future, such as adding ponds of water, varied island tops, mountains, and vines connecting islands. I would also like to rebuild the island actor class from the ground up (as it was written in Blueprints with a C++ scaffold while the level generation is pure C++ due to time constraints from personal circumstances) to add further modularity, so individual functions of the island generation can be replaced and improved.

![A screenshot of the project in engine featuring varied islands and a player.](https://github.com/jole-c/floating-island-gen/blob/main/thesis1.png?raw=true)
