export module Structures.Types;

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
	inline constexpr StructureType MAX{ StructureType::ST_MAX };
}