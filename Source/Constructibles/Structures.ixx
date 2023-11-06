// by Dmitry Kolontay

export module Structures;
import ConstructibleBase;

import <tuple>;

export enum class StructureType {
	Battery,
	SolarPanel, //t
	Reactor, //t
	Cryochamber, //t
	Hull,
};

export namespace ST {
	inline constexpr StructureType Battery{ StructureType::Battery };
	inline constexpr StructureType SolarPanel{ StructureType::SolarPanel };
	inline constexpr StructureType Reactor{ StructureType::Reactor };
	inline constexpr StructureType Cryochamber{ StructureType::Cryochamber };
	inline constexpr StructureType Hull{ StructureType::Hull };
}

export template <StructureType Type> class Structure
{
private:
	Structure() = default;
};

template<>
class Structure<ST::Battery> : public ConstructibleBase
{
	ContainerT<RT::Power> powerStorage_;
};

template<>
class Structure<ST::Cryochamber> : public ConstructibleBase
{
	ContainerT<RT::Heat> heatStorage_;
	ContainerT<RT::Power> powerStorage_;
};

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