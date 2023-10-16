// by Dmitry Kolontay

export module Constructible.Structure;
export import Constructible.ConstructibleBase;

export enum class StructureType {
	Battery,
	SolarPanel,
	Reactor,
	Cryochamber,
	Hull,
};

export template <StructureType Type> 
	class Structure : public ConstructibleBase
{
	
};