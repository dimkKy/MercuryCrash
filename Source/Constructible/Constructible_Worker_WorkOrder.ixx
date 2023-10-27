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
export class WorkOrder {
	std::atomic<float> requestedAmount_;

	using ContainerPtr = std::weak_ptr<ResourceContainer>;
	const ContainerPtr source_;
	const ContainerPtr target_;
	const ContainerPtr fallback_;

	float UnloadTo(const ContainerPtr& to, float amount)& {
		assert(amount >= 0.f);
		if (auto target{ to.lock() }) {
			return target->ChangeAmount(amount);
		}
		else {
			return 0.f;
		}
	}

public:
	WorkOrder(ContainerPtr&& source, ContainerPtr&& target, float requestedAmount) noexcept(false) :
		WorkOrder(std::move(source), std::move(target), std::move(ContainerPtr{}), requestedAmount) {}

	WorkOrder(ContainerPtr&& source, ContainerPtr&& target,
		ContainerPtr&& fallback, float requestedAmount) noexcept(false) :
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
		}*/
	}

	//returns amount acquired (positive)
	float RequestFromSource(float amount)& {
		assert(amount >= 0.f);
		if (auto source{ source_.lock() }) {

			float workLeft{ GetAmount() };
			float availableAmount{ std::min(amount, workLeft) };

			while (!requestedAmount_.compare_exchange_weak(workLeft, workLeft - availableAmount)) {
				availableAmount = std::min(amount, workLeft);
			}

			float amountGot{ -1.f * source->ChangeAmount(-1.f * availableAmount) };

			requestedAmount_.fetch_add(availableAmount - amountGot, std::memory_order_acq_rel);
			return amountGot;
		}
		else {
			return 0.f;
		}
	}

	float UnloadToTarget(float amount, bool bAllowReturn)& {
		assert(amount >= 0.f);
		float unloadedAmount{ UnloadTo(target_, amount) };
		if (bAllowReturn) {
			unloadedAmount += UnloadTo(source_, amount - unloadedAmount);
			unloadedAmount += UnloadTo(fallback_, amount - unloadedAmount);
		}
		return unloadedAmount;
	}

	float GetAmount() const& {
		return requestedAmount_.load(std::memory_order_relaxed);
	}

	virtual ~WorkOrder() = default;
};

export class BuildOrder : WorkOrder {

};

export class TransferOrder : WorkOrder {

};