// by Dmitry Kolontay

export module Resources.Types;

export enum class ResourceType {
	Heat,
	Composite,
	Conductor,
	Power,
	Time,
};

export namespace RT {
	inline constexpr const ResourceType Heat{ ResourceType::Heat };
	inline constexpr const ResourceType Composite{ ResourceType::Composite };
	inline constexpr const ResourceType Conductor{ ResourceType::Conductor };
	inline constexpr const ResourceType Power{ ResourceType::Power };
	inline constexpr const ResourceType Time{ ResourceType::Time };
}