// by Dmitry Kolontay

export module Ship;

import Structures;
import Resources;
import BalanceSettings;
import WorkOrder;

import <list>;
import <string>;
import <vector>;
import <memory>;

constexpr auto maxfloat = std::numeric_limits<float>::max;

export class Ship {
	Hull hull_;
	Reactor reactor_;

	ContainerT<RT::Composite> composite_{ maxfloat(), 
		BalanceSettings::StorageInitState().GetRes<RT::Composite>() };
	ContainerT<RT::Conductor> conductors_{ maxfloat(),
		BalanceSettings::StorageInitState().GetRes<RT::Conductor>() };

	template<typename T>
	using ListType = std::list<std::shared_ptr<T>>;

	ListType<SolarPanel> solarPanels_;
	ListType<Cryochamber> cryochambers_;
	ListType<Battery> batteries_;

	ListType<Worker> workers_;
	ListType<WorkOrder> orders_;

	ContainerT<RT::Composite> mine_{ maxfloat(), BalanceSettings::MineInitState() };

public:
	Ship() {
		/*for (int i{ 0 }; i < state.workers_; ++i) {
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
		}*/
	}

	template<StructureType Type>
	std::weak_ptr<Structure<Type>> AddStructure()& {

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