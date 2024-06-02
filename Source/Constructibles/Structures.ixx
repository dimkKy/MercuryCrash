// by Dmitry Kolontay

export module Structures;
export import Structures.Types;

import ConstructibleBase;
import BalanceSettings;

import <tuple>;
import <memory>;

export template <StructureType Type> class Structure
{
	static_assert(sizeof(this) < 0, "specialization use is required");
};

#define DEFINE_ST_CTORS(_name_);							\
	template<typename... Args>								\
	constexpr Structure<(_name_)>(Args&&... args) :			\
	ConstructibleBase{ std::forward<Args>(args)... } {}		\
															\
	constexpr Structure<(_name_)>() :						\
	Structure<(_name_)>{									\
		BalanceSettings::MaxBuildRes<(_name_)>(),			\
			BalanceSettings::InitRes<(_name_)>() } {}

//do I even need that class
template<>
class Structure<ST::Battery> : public ConstructibleBase
{
	ContainerT<RT::Power> powerStorage_;

public:
	template<typename... Args>
	constexpr Structure<ST::Battery>(Args&&... args) :
		ConstructibleBase{ std::forward<Args>(args)... } {}
};
export using Battery = Structure<ST::Battery>;

template<>
class Structure<ST::SolarPanel> : public ConstructibleBase
{
	ContainerT<RT::Power> powerStorage_;

public:
	template<typename... Args>
	constexpr Structure<ST::SolarPanel>(Args&&... args) :
		ConstructibleBase{ std::forward<Args>(args)... } {}
};
export using SolarPanel = Structure<ST::SolarPanel>;

template<>
class Structure<ST::Cryochamber>
{
	ContainerT<RT::Heat> heatStorage_;
	ContainerT<RT::Power> powerStorage_;

	static inline float heatingRate{ 1.f };
	static inline float coolingRate{ 1.f };

public:
	template<typename... Args>
	constexpr Structure<ST::Cryochamber>(Args&&... args) :
		ConstructibleBase{ std::forward<Args>(args)... } {}

	void Tick(float deltatime)& {
		float avPower{ std::min(powerStorage_.GetRes(), deltatime * coolingRate) };
		powerStorage_.ChangeAmount(-1.f * avPower);

		float heatChange{ deltatime * heatingRate - avPower };
		float heatChanged{ heatStorage_.ChangeAmount(heatChange) };

		if (heatChanged > +0.f && heatChanged < heatChange) {
			//overheat
		}
	}
	//callback foo
	//add energy?
};
export using Cryochamber = Structure<ST::Cryochamber>;
//external?
//float Structure<ST::Cryochamber>::coolingRate{ 1.f };
//float Structure<ST::Cryochamber>::heatingRate{ 1.f };


template<>
class Structure<ST::Reactor> : public ConstructibleBase
{
public:
	DEFINE_ST_CTORS(ST::Reactor);
};
export using Reactor = Structure<ST::Reactor>;

template<>
class Structure<ST::Hull> : public ConstructibleBase
{

public:
	DEFINE_ST_CTORS(ST::Hull);
};
export using Hull = Structure<ST::Hull>;

enum class WorkerStatus
{
	Waiting,
	Changeover,
	Loading,
	Unloading,
	Malfunction,
};

class WorkOrder;

template<>
class Structure<ST::Worker>: public ConstructibleBase
{
	std::weak_ptr<WorkOrder> order_;
	std::weak_ptr<WorkOrder> nextOrder_;

	ContainerD storageInitState_;

	ContainerT<ResourceType::Time> timer_;

	constexpr static float workRate{ 1.f };

	WorkerStatus status_;
	//self container
	//on job finished
	void OnWorkCompletion();

	//returns time left
	float OnChangeover(float deltatime);
	//returns time left
	float LoadFrom(float deltatime);
	//returns time left
	float AddTo(float deltatime);

	void StartChangeover();
	void StartChangeover(float initTime);

public:
	bool AssignWorkOrder(const std::shared_ptr<WorkOrder>& order)&;
	void FinishWorking()&;

	void Tick(float deltatime)&;
};
export using Worker = Structure<ST::Worker>;