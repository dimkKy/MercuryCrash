// by Dmitry Kolontay

export module Game;

import Constructible;

import <list>;
import <string>;
import <vector>;


export class Ship {
	Structure<StructureType::Hull> hull_;
	Structure<StructureType::Reactor> reactor_;

	std::list<Structure<StructureType::SolarPanel>> solarPanels_;
	std::list<Structure<StructureType::Cryochamber>> cryochambers_;
	std::list<Structure<StructureType::Battery>> batteries_;

	std::list<Worker> workers_;
	std::list<WorkOrder> orders_;

public:
	//Ship();
};

export class TickOperator {

public:
	void Run() {
		while (true) {
			//tick all the workers


		}
	}

	void operator()() {
		Run();
	}
};