#pragma once

#include <Enlivengine/Config.hpp>
#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/System/Macros.hpp>

#include <Dyma/Dyma.hpp>

#ifdef ENLIVE_ENABLE_DEBUG_MEMORY
#include <vector>
#endif // ENLIVE_ENABLE_DEBUG_MEMORY

namespace en
{
	
class MemoryAllocator
{
public:
	MemoryAllocator() = delete;

	static bool InitializeEngineAllocator(dyma::Allocator* allocator);

	template <typename T>
	static T* TypedAllocate(U32 count = 1)
	{
		return (T*)Allocate(count * ENLIVE_SIZE_OF(T));
	}

	template <typename T = void>
	static bool TypedDeallocate(T*& ptr)
	{
		void* voidPtr = (void*)ptr;
		const bool result = Deallocate(voidPtr);
		ptr = (T*)voidPtr;
		return result;
	}

	static void* Allocate(U32 size);
	static bool Deallocate(void*& ptr);

private:
	static dyma::Allocator* sEngineAllocator;

#ifdef ENLIVE_ENABLE_DEBUG_MEMORY
public:
	class DebugAllocator : public dyma::Allocator
	{
	public:
		struct DebugMemoryBlock
		{
			void* ptr;
			U32 size;
			const char* file;
			U32 line;
		};

		DebugAllocator();

		void SetAllocatorToDebug(dyma::Allocator* allocator);
		dyma::Allocator* GetAllocatorToDebug();
		const dyma::Allocator* GetAllocatorToDebug() const;

		void* Allocate(std::size_t size) override final;
		bool Deallocate(void*& ptr) override final;
		bool Owns(const void* ptr) const override final;

		U32 GetAllocationCount() const { return mAllocationCount; }
		U32 GetDeallocationCount() const { return mDeallocationCount; }
		U32 GetUsedSize() const { return mUsedSize; }
		U32 GetPeakSize() const { return mPeakSize; }
		const std::vector<DebugMemoryBlock>& GetBlocks() const { return mBlocks; }

	private:
		dyma::Allocator* mAllocator;

		U32 mAllocationCount;
		U32 mDeallocationCount;

		U32 mUsedSize;
		U32 mPeakSize;

		std::vector<DebugMemoryBlock> mBlocks;
	};

	static const DebugAllocator& GetDebugAllocator() { return sDebugAllocator; }

private: 
	static DebugAllocator sDebugAllocator;
#endif // ENLIVE_ENABLE_DEBUG_MEMORY

#ifdef ENLIVE_ENABLE_DEFAULT_MALLOCATOR
private:
	static dyma::Mallocator sDefaultMallocator;
	static bool sDefaultMallocatorInitialized; // Trick to auto-register Mallocator
#endif // ENLIVE_ENABLE_DEFAULT_MALLOCATOR
};

} // namespace en