// by Dmitry Kolontay

export module Structures;

import ConstructibleBase;

import <tuple>;
import <memory>;

export enum class StructureType {
	Battery,
	SolarPanel, //t
	Reactor, //t
	Cryochamber, //t
	Hull,
	Worker,
	ST_MAX,
};

export namespace ST {
	inline constexpr StructureType Battery{ StructureType::Battery };
	inline constexpr StructureType SolarPanel{ StructureType::SolarPanel };
	inline constexpr StructureType Reactor{ StructureType::Reactor };
	inline constexpr StructureType Cryochamber{ StructureType::Cryochamber };
	inline constexpr StructureType Hull{ StructureType::Hull };
	inline constexpr StructureType Worker{ StructureType::Worker };
}

export template <StructureType Type> class Structure
{
	//static_assert(sizeof(Type) < 0 && "specialization use is required");
	Structure() = default;
};

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

		//float p = get power(deltatime)
		//remove heat (p * coolingRate)

		//add heat (heating rate * deltatime)
		//check heat
	}

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
	//Structure<ST::Reactor>(const BasicResPack& current) :
		//ConstructibleBase(info, current) {}
	template<typename... Args>
	constexpr Structure<ST::Reactor>(Args&&... args) :
		ConstructibleBase{ std::forward<Args>(args)... } {}
};
export using Reactor = Structure<ST::Reactor>;

template<>
class Structure<ST::Hull> : public ConstructibleBase
{

public:
	//Structure<ST::Hull>(const BasicResPack& current) :
		//ConstructibleBase(info, current) {}
	template<typename... Args>
	constexpr Structure<ST::Hull>(Args&&... args) :
		ConstructibleBase{ std::forward<Args>(args)... } {}
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

	ContainerD storage_;

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