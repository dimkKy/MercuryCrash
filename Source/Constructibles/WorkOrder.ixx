export module WorkOrder;

import Resources;
import Utils;
import ConstructibleBase;

import <memory>;
import <cassert>;
import <algorithm>;
import <atomic>;
import <stdexcept>;

/*
* > Loading
* > unloading
*/

//build order and return order
template<ResourceType Type>
using ContainerPtrT = std::weak_ptr<ContainerT<Type>>;
using ContainerPtrD = std::weak_ptr<Container>;

//WAY TO CANCEL DEAD ORDER
export class WorkOrder {
protected:

	//throw?
	float AddTo(Container& to, float amount) const {
		assert(amount >= 0.f);
		return to.ChangeAmount(amount);
	}

	template<Utils::ChildOf<Container, true> Cont>
	float AddTo(const std::weak_ptr<Cont>& to, float amount) const {
		assert(amount >= 0.f);
		if (auto&& target{ to.lock() }) {
			return target->ChangeAmount(amount);
		}
		else {
			return 0.f;
		}
	}

public:
	//returns amount acquired (positive)
	virtual float RequestFromSource(float amount) & = 0;

	virtual float UnloadToTarget(float amount) const & = 0;
	virtual ~WorkOrder() = default;
};

export class BuildOrder : WorkOrder {

	const std::weak_ptr<ConstructibleBase> structure_;

	float Process(float amount) const {
		assert(amount >= 0.f);
		if (auto&& structure{ structure_.lock() }) {
			if (auto&& timer{ structure->VerifyForConstruction() }) {
				return AddTo(*timer, amount);
			}
		}
		return 0.f;
	}

public:
	BuildOrder(std::weak_ptr<ConstructibleBase>&& structure) :
		structure_{ structure }
	{

	}

	virtual float RequestFromSource(float amount) & override {
		return Process(amount);
	}

	virtual float UnloadToTarget(float amount) const & override {
		return Process(amount);
	}
};

export class TransferOrder : WorkOrder {
	std::atomic<float> requested_;

	const ContainerPtrD source_;
	const ContainerPtrD target_;
	const ContainerPtrD fallback_;

public:
	TransferOrder(ContainerPtrD&& source, ContainerPtrD&& target,
		ContainerPtrD&& fallback, float requested) noexcept(false) :
		source_{ source }, target_{ target }, fallback_{ fallback },
		requested_{ requested } 
	{
		if (requested_ <= 0.f) {
			throw std::invalid_argument{ "requestedAmount must be positive" };
		}
		
		if (auto&& src{ source_.lock() }) {
			ResourceType type{ src->GetType() };

			if (auto&& trgt{ target_.lock() }) {
				if (type != trgt->GetType()) {
					throw std::invalid_argument
						{ "target: containers are not of the same type" };
				}
			}
			else {
				throw std::invalid_argument{ "target is already expired" };
			}

			if (auto&& flbck{ fallback_.lock() }) {
				if (type != flbck->GetType()) {
					throw std::invalid_argument
						{ "fallback: containers are not of the same type" };
				}
			}
		}
		else {
			throw std::invalid_argument{ "source is already expired" };
		}

	}

	TransferOrder(ContainerPtrD&& source, ContainerPtrD&& target, float requested) 
		noexcept(noexcept(TransferOrder({}, {}, {}, 0.f))) :
		TransferOrder(std::move(source), std::move(target), {}, requested) {};

	float GetAmount() const& {
		return requested_.load(std::memory_order_relaxed);
	}

	virtual float RequestFromSource(float amount) & override {
		using namespace std;
		assert(amount >= 0.f);
		if (auto source{ source_.lock() }) {

			float workLeft{ GetAmount() };
			float amountLeft{ min(amount, workLeft) };

			while (!requested_.compare_exchange_weak(
				workLeft, workLeft - amountLeft)) {
				amountLeft = min(amount, workLeft);
			}

			float acquired{ -1.f * source->ChangeAmount(-1.f * amountLeft) };

			requested_.fetch_add(amountLeft - acquired, memory_order_acq_rel);
			return acquired;
		}
		else {
			return 0.f;
		}
	}

	virtual float UnloadToTarget(float amount) const & override {
		assert(amount >= 0.f);
		float unloaded{ AddTo(target_, amount) };
		unloaded += AddTo(source_, amount - unloaded);
		unloaded += AddTo(fallback_, amount - unloaded);
		return unloaded;
	}
};