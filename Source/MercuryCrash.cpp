// by Dmitry Kolontay

import <thread>;

import Resources;
import Ship;
import CommandListener;

int main()
{
	//Ship ship;
	//CommandListener listener{ ship };
	//ResoursePack pack;
	//std::jthread listenerThread{ std::ref(listener) };
	ResoursePack<RT::Composite, RT::Conductor> pack1{ 10.f, 11.f };
	ResoursePack pack2{ pack1 };

	ResoursePack<RT::Composite, RT::Conductor, RT::Heat> pack3{ pack1 };
	ResoursePack<RT::Composite, RT::Conductor, RT::Heat, RT::Power> pack4{ pack3 };



	return 0;
}