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
	inline constexpr ResourceType Heat{ ResourceType::Heat };
	inline constexpr ResourceType Composite{ ResourceType::Composite };
	inline constexpr ResourceType Conductor{ ResourceType::Conductor };
	inline constexpr ResourceType Power{ ResourceType::Power };
	inline constexpr ResourceType Time{ ResourceType::Time };
}