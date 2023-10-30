// by Dmitry Kolontay

export module Game;

import Constructible;
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
	Structure<StructureType::Hull> hull_;
	Structure<StructureType::Reactor> reactor_;

	ContainerT<RT::Composite> composite_;
	ContainerT<RT::Conductor> conductors_;

	std::list<Structure<StructureType::SolarPanel>> solarPanels_;
	std::list<Structure<StructureType::Cryochamber>> cryochambers_;
	std::list<Structure<StructureType::Battery>> batteries_;

	std::list<Worker> workers_;
	std::list<WorkOrder> orders_;

	ContainerT<ResourceType::Composite> mine_;
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