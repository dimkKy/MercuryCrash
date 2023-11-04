// by Dmitry Kolontay

export module Structures;
import ConstructibleBase;

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

};

template<>
class Structure<ST::Hull> : public ConstructibleBase
{

	//Structure<StructureType::Hull> hull_;


};