#pragma once

class Client {
public:


	std::add_pointer_t<void() noexcept> OnTick;
	std::add_pointer_t<void() noexcept> OnFrameStage;

};