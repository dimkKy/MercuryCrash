// by Dmitry Kolontay

import <thread>;

import Resources;
//import Ship;
import CommandListener;

int main()
{
	//Ship ship;

	CommandListener listener;
	std::jthread listenerThread{ std::ref(listener) };

	return 0;
}