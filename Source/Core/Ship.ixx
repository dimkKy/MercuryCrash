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

template<typename T>
concept Tickable = requires (T t, float f) { t.Tick(f); };

export template<Tickable T>
class TickOperator {
	std::list<int> list;
	using Iter = decltype(list)::iterator;
public:
	void Run() {
		while (true /*and not empty*/) {
			//const std::lock_guard<std::mutex> lock(g_i_mutex);
			// 
			//tick all the workers
	
			//mutex +
			//check not empty
			Iter begin = list.begin();
			auto elem = *begin;

			for (auto&& elem : list) {
				//elem.Tick
			}
			//mutex -
			//process elem

			//get next

		}
	}

	void operator()() {
		Run();
	}
};

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
		for (int i{ 0 }; i < BalanceSettings::InitStructureCount<ST::SolarPanel>(); ++i) {
			//AddStructure<ST::SolarPanel>();
		}

		/*for (int i{ 0 }; i < state.solarPanels_; ++i) {
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

	constexpr void Start();
};

