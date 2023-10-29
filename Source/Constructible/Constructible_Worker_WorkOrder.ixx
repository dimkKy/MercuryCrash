export module Constructible.Worker.WorkOrder;

import Resources;

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
using ContainerPtrT = std::weak_ptr<ResourceContainerT<Type>>;
using ContainerPtrD = std::weak_ptr<ResourceContainer>;

export class WorkOrder {
protected:
	float AddTo(const ContainerPtrD& to, float amount)& {
		assert(amount >= 0.f);
		if (auto target{ to.lock() }) {
			return target->ChangeAmount(amount);
		}
		else {
			return 0.f;
		}
	}

public:
	/*WorkOrder(ContainerPtrD&& source, ContainerPtrD&& target, float requestedAmount) noexcept(false) :
		WorkOrder(std::move(source), std::move(target), std::move(ContainerPtrD{}), requestedAmount) {}

	WorkOrder(ContainerPtrD&& source, ContainerPtrD&& target,
		ContainerPtrD&& fallback, float requestedAmount) noexcept(false) :
		source_{ source }, target_{ target }, fallback_{ fallback },
		requestedAmount_{ requestedAmount }
	{
		if (requestedAmount_ <= 0.f) {
			throw std::invalid_argument{ "requestedAmount must be positive" };
		}
		/*if (!source_.operator bool() || source_.expired() || !target_ || target_.expired()) {
			throw std::invalid_argument{ "containers are not of the same type" };
		}
		if (source_->GetType() != target_->GetType() != fallback_->GetType()) {
			throw std::invalid_argument{ "containers are not of the same type" };
		}*
	}*/

	//returns amount acquired (positive)
	virtual float RequestFromSource(float amount) & = 0;

	virtual float UnloadToTarget(float amount, bool bAllowReturn) & = 0;
	virtual ~WorkOrder() = default;
};

export class BuildOrder : WorkOrder {
	const ContainerPtrT<ResourceType::Time> timer_;
	const ContainerPtrT<ResourceType::Conductor> conductor_;
	const ContainerPtrT<ResourceType::Composite> composite_;


	//TODO
public:
	virtual float RequestFromSource(float amount) & override {

	}

	virtual float UnloadToTarget(float amount, bool bAllowReturn) & override {

	}
};

export class TransferOrder : WorkOrder {
	std::atomic<float> requestedAmount_;

	const ContainerPtrD source_;
	const ContainerPtrD target_;
	const ContainerPtrD fallback_;

public:

	float GetAmount() const& {
		return requestedAmount_.load(std::memory_order_relaxed);
	}

	virtual float RequestFromSource(float amount) & override {
		using namespace std;
		assert(amount >= 0.f);
		if (auto source{ source_.lock() }) {

			float workLeft{ GetAmount() };
			float amountLeft{ min(amount, workLeft) };

			while (!requestedAmount_.compare_exchange_weak(
				workLeft, workLeft - amountLeft)) {
				amountLeft = min(amount, workLeft);
			}

			float amountGot{ -1.f * source->ChangeAmount(-1.f * amountLeft) };

			requestedAmount_.fetch_add(amountLeft - amountGot, memory_order_acq_rel);
			return amountGot;
		}
		else {
			return 0.f;
		}
	}

	virtual float UnloadToTarget(float amount, bool bAllowReturn) & override {
		assert(amount >= 0.f);
		float unloadedAmount{ AddTo(target_, amount) };
		if (bAllowReturn) {
			unloadedAmount += AddTo(source_, amount - unloadedAmount);
			unloadedAmount += AddTo(fallback_, amount - unloadedAmount);
		}
		return unloadedAmount;
	}
};