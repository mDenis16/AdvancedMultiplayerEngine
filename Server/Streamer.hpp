#pragma once
#include <Shared.hpp>

#include "Gridmap/Grid.hpp"


class Streamer
{
public:
	void Init(int threads, int distance);


	SAFE_PROP(Grid<Entity*>*, grid);
	VARIABLE_THREAD(HandleEntityStream);
	SAFE_PROP(std::queue<Player*>, EntityStreamQueue);

	void UpdateStream(Player* player);

};

