// by Dmitry Kolontay

import <thread>;

import Resources;
import Ship;
import CommandListener;

int main()
{
	Ship ship;
	CommandListener listener{ ship };
	//ResoursePack pack;
	std::jthread listenerThread{ std::ref(listener) };

	return 0;
}