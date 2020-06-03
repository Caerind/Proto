#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Dyma/Dyma.hpp>

namespace en
{
	
class MemoryAllocator
{
public:
	MemoryAllocator() = delete;

	static bool InitializeAllocator();

	static void* Allocate(U32 size);
	static bool Deallocate(void* ptr);

#ifdef ENLIVE_ENABLE_DEBUG_MEMORY
	class DebugAllocator : public dyma::Allocator
	{
	public:
		DebugAllocator(dyma::Allocator& allocatorToDebug);

		void* Allocate(std::size_t size) override final;
		bool Deallocate(void*& ptr) override final;
		bool Owns(const void* ptr) const override final;

		std::size_t GetAllocationCount() const { return mAllocationCount; }
		std::size_t GetDeallocationCount() const { return mDeallocationCount; }
		std::size_t GetUsedSize() const { return mUsedSize; }
		std::size_t GetPeakSize() const { return mPeakSize; }

	private:
		dyma::Allocator& mAllocator;

		std::size_t mAllocationCount;
		std::size_t mDeallocationCount;

		std::size_t mUsedSize;
		std::size_t mPeakSize;
	};

	static const DebugAllocator* GetDebugAllocator() { return static_cast<const DebugAllocator*>(sAllocator); }

#endif // ENLIVE_ENABLE_DEBUG_MEMORY

private:
	static dyma::Allocator* InitializeMainAllocator();

private:
	static dyma::Allocator* sAllocator;

#ifdef ENLIVE_ENABLE_DEBUG_MEMORY
	static dyma::Allocator* sAllocatorToDebug;
#endif // ENLIVE_ENABLE_DEBUG_MEMORY
};

} // namespace en