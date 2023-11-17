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
	Hull hull_{ BalanceSettings::MaxBuildRes<ST::Hull>(), 
		BalanceSettings::InitRes<ST::Hull>() };
	Reactor reactor_;

	ContainerT<RT::Composite> composite_;
	ContainerT<RT::Conductor> conductors_;

	template<typename T>
	using ListType = std::list<std::shared_ptr<T>>;

	ListType<SolarPanel> solarPanels_;
	ListType<Cryochamber> cryochambers_;
	ListType<Battery> batteries_;

	ListType<Worker> workers_;
	ListType<WorkOrder> orders_;

	ContainerT<RT::Composite> mine_;

public:
	/*Ship() :
		hull_{ BalanceSettings:: state.hullInfo_, state.hullHealthInit_ }, 
		reactor_{ state.reactorInfo_, state.reactorHealthInit_ },
		composite_{ maxfloat(), state.storageInitState_.GetRes<RT::Composite>() },
		conductors_{ maxfloat(), state.storageInitState_.GetRes<RT::Conductor>() },
		mine_{ maxfloat(), state.mineAmount_ }
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

	std::weak_ptr<ConstructibleBase> AddSolarPanel() & {

	}

	std::weak_ptr<ConstructibleBase> AddBattery() & {

	}

	std::weak_ptr<ConstructibleBase> AddWorker() & {

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