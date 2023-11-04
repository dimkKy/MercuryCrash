// by Dmitry Kolontay

export module Ship;

import Structures;
import Worker;
import Resources;

import <list>;
import <string>;
import <vector>;

//do I need ResourceInfo here

export struct ShipInitState {
	BasicResPack hullHealth_;
	BasicResPack reactorHealth_;
	BasicResPack storage_;

	int solarPanels_;
	int cryochambers_;
	int batteries_;

	int workers_;

	float mineAmount_;	
};

export class Ship {
	Structure<ST::Hull> hull_;
	Structure<ST::Reactor> reactor_;

	ContainerT<RT::Composite> composite_;
	ContainerT<RT::Conductor> conductors_;

	std::list<Structure<ST::SolarPanel>> solarPanels_;
	std::list<Structure<ST::Cryochamber>> cryochambers_;
	std::list<Structure<ST::Battery>> batteries_;

	std::list<Worker> workers_;
	std::list<WorkOrder> orders_;

	ContainerT<RT::Composite> mine_;

	template<StructureType Type>
	void AddStructure()& {

	}

public:
	/*Ship(const ShipInitState& state) :
		hull_{ state.hullHealth_ }, reactor_{ state.reactorHealth_ },
		composite_{ , state.storage_.GetAmount<RT::Composite>() },
		conductors_{ , state.storage_.GetAmount<RT::Conductor>() },
		mine_{ std::numeric_limits<decltype(mine_)::AmountType>::max(), state.mineAmount_ }
	{
		for (int i{ 0 }; i < state.workers_; ++i) {
			//add worker
			workers_.emplace_back();
		}

		for (int i{ 0 }; i < state.solarPanels_; ++i) {
			solarPanels_.emplace_back();
		}

		for (int i{ 0 }; i < state.cryochambers_; ++i) {
			cryochambers_.emplace_back();
		}

		for (int i{ 0 }; i < state.batteries_; ++i) {
			batteries_.emplace_back();
		}
	}*/

	void AddSolarPanel() & {

	}

	void AddBattery() & {

	}

	void AddWorker() & {

	}
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