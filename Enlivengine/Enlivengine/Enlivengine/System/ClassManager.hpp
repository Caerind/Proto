#pragma once

#include <functional>
#include <unordered_map>
#include <iterator>

#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/System/Hash.hpp>
#include <Enlivengine/System/TypeTraits.hpp>
#include <Enlivengine/System/TypeInfo.hpp>
#include <Enlivengine/System/Meta.hpp>
#include <Enlivengine/System/MemoryAllocator.hpp>

namespace en
{

class ClassManager
{
public:
	ClassManager() = delete;

	static U32 GetRegisteredClassCount()
	{
		return static_cast<U32>(mClasses.size());
	}

	static const char* GetClassNameAtIndex(U32 index)
	{
		auto itr = mClasses.begin();
		std::advance(itr, index);
		if (itr != mClasses.end())
		{
			return itr->second.name;
		}
	}

	static U32 GetClassHashAtIndex(U32 index)
	{
		auto itr = mClasses.begin();
		std::advance(itr, index);
		if (itr != mClasses.end())
		{
			return itr->first;
		}
	}

	static void* CreateClassFromHash(U32 classHash)
	{
		const auto itr = mClasses.find(classHash);
		if (itr != mClasses.end())
		{
			return itr->second.factory();
		}
		else
		{
			return nullptr;
		}
	}

	static bool IsRegistered(U32 classHash)
	{
		return mClasses.find(classHash) != mClasses.end();
	}

	template <typename T>
	static bool IsRegistered()
	{
		return IsRegistered(TypeInfo<T>::GetHash());
	}

	template <typename T>
	static bool Register()
	{
		static_assert(Meta::IsRegistered<T>() && TypeInfo<T>::IsKnown());
		constexpr U32 hash = TypeInfo<T>::GetHash();
		mClasses[hash].name = TypeInfo<T>::GetName();
		mClasses[hash].size = TypeInfo<T>::GetSize();
		mClasses[hash].align = TypeInfo<T>::GetAlign();
		mClasses[hash].factory = []()
		{
#ifdef ENLIVE_ENABLE_DEBUG_MEMORY
			constexpr U32 stringStorageSize = StringLength("ClassManager::") + StringLength(TypeInfo<T>::GetName()) + 1;
			constexpr ConstexprStringStorage stringStorage = ConstexprStringStorage<stringStorageSize>("ClassManager::", TypeInfo<T>::GetName());
			constexpr const char* context = stringStorage.GetData();
#else
			constexpr const char* context = nullptr;
#endif // ENLIVE_ENABLE_DEBUG_MEMORY
			return (void*)enNew(T, context); 
		};
		return true;
	}

private:
	struct ClassInfo
	{
		const char* name;
		std::function<void* ()> factory;
		U32 size;
		U32 align;
	};
	static std::unordered_map<U32, ClassInfo> mClasses;
};

// Custom ImGui Editor
#define ENLIVE_META_CLASS_DEFAULT_TRAITS_VIRTUAL_IMGUI_EDITOR(className) \
	template <> \
	struct CustomImGuiEditor<className> \
	{ \
		static constexpr bool value = true; \
		static bool ImGuiEditor(className& object, const char* name) \
		{ \
			return object.ImGuiEditor(name); \
		} \
	};
#define ENLIVE_META_CLASS_DEFAULT_VIRTUAL_IMGUI_EDITOR() \
	virtual bool ImGuiEditor(const char* name) \
	{ \
		 return ObjectEditor::ImGuiEditor_Registered(*this, name); \
	}

// Custom Serialization
#define ENLIVE_META_CLASS_DEFAULT_TRAITS_VIRTUAL_SERIALIZATION(className) \
	template <> \
	struct CustomSerialization<className> \
	{ \
		static constexpr bool value = true; \
		static bool Serialize(DataFile& dataFile, const className& object, const char* name) \
		{ \
			return object.Serialize(dataFile, name); \
		} \
		static bool Deserialize(DataFile& dataFile, className& object, const char* name) \
		{ \
			return object.Deserialize(dataFile, name); \
		} \
	};
#define ENLIVE_META_CLASS_DEFAULT_VIRTUAL_SERIALIZATION() \
	virtual bool Serialize(DataFile& dataFile, const char* name) const \
	{ \
		 return dataFile.Serialize_Registered(*this, name); \
	} \
	virtual bool Deserialize(DataFile& dataFile, const char* name) \
	{ \
		 return dataFile.Deserialize_Registered(*this, name); \
	}


} // namespace en