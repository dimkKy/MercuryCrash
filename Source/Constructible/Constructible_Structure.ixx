// by Dmitry Kolontay

export module Constructible.Structure;
import Constructible.ConstructibleBase;

export enum class StructureType {
	Battery,
	SolarPanel, //t
	Reactor, //t
	Cryochamber, //t
	Hull,
};

export template <StructureType Type> 
	class Structure : public ConstructibleBase
{
	
};

template<>
class Structure<StructureType::Battery> : public ConstructibleBase
{
	ContainerT<RT::Power> powerStorage_;
};

template<>
class Structure<StructureType::Cryochamber> : public ConstructibleBase
{
	ContainerT<RT::Heat> heatStorage_;
	ContainerT<RT::Power> powerStorage_;
};

template<>
class Structure<StructureType::Reactor> : public ConstructibleBase
{

};

template<>
class Structure<StructureType::Hull> : public ConstructibleBase
{

	//Structure<StructureType::Hull> hull_;


};