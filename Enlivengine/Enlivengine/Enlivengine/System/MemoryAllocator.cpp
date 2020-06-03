#include <Enlivengine/System/MemoryAllocator.hpp>

namespace en
{

dyma::Allocator* MemoryAllocator::sAllocator = nullptr;
#ifdef ENLIVE_ENABLE_DEBUG_MEMORY
dyma::Allocator* MemoryAllocator::sAllocatorToDebug = nullptr;
#endif // ENLIVE_ENABLE_DEBUG_MEMORY

bool MemoryAllocator::InitializeAllocator()
{
#ifdef ENLIVE_ENABLE_DEBUG_MEMORY
	sAllocatorToDebug = InitializeMainAllocator();
	sAllocator = new MemoryAllocator::DebugAllocator(*sAllocatorToDebug);
#else
	sAllocator = InitializeMainAllocator();
#endif // ENLIVE_ENABLE_DEBUG_MEMORY
	return false;
}

void* MemoryAllocator::Allocate(U32 size)
{
	return sAllocator->Allocate(static_cast<std::size_t>(size));
}

bool MemoryAllocator::Deallocate(void* ptr)
{
	return sAllocator->Deallocate(ptr);
}

#ifdef ENLIVE_ENABLE_DEBUG_MEMORY

MemoryAllocator::DebugAllocator::DebugAllocator(dyma::Allocator& allocatorToDebug)
	: mAllocator(allocatorToDebug)
	, mAllocationCount(0)
	, mDeallocationCount(0)
	, mUsedSize(0)
	, mPeakSize(0)
{
}

void* MemoryAllocator::DebugAllocator::Allocate(std::size_t size)
{
	void* ptr = mAllocator.Allocate(size);
	if (ptr != nullptr)
	{
		mAllocationCount++;
		mUsedSize++;
		if (mUsedSize > mPeakSize)
		{
			mPeakSize = mUsedSize;
		}
	}
	return ptr;
}

bool MemoryAllocator::DebugAllocator::Deallocate(void*& ptr)
{
	const void* ptrBeforeDeallocation = ptr;
	const bool result = mAllocator.Deallocate(ptr);
	if (result)
	{
		mDeallocationCount++;
		//mUsedSize -= blockSize;
	}
	return result;
}

bool MemoryAllocator::DebugAllocator::Owns(const void* ptr) const
{
	return mAllocator.Owns(ptr);
}

#endif // ENLIVE_ENABLE_DEBUG_MEMORY

dyma::Allocator* MemoryAllocator::InitializeMainAllocator()
{
	return new dyma::Mallocator();
}

} // namespace en