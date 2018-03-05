#pragma once
#include "../Utils/Macros.hpp"
#include <memory>

namespace Amber3D
{
	template<typename T>
	union PoolChunk
	{
		T value;
		PoolChunk<T>* nextPoolChunk;

		PoolChunk() {}
		~PoolChunk() {}
	};

	template<typename T, typename Allocator = std::allocator<PoolChunk<T> > >
	class PoolAllocator
	{
	private:
		static const size_t POOL_ALLOCATOR_DEFAULT_SIZE = 1024;

		size_t m_openAllocations = 0;

		size_t m_size = 0;

		PoolChunk<T>* m_data = nullptr;
		PoolChunk<T>* m_head = nullptr;

		Allocator* m_parentAllocator = nullptr;
		bool m_needsToDeleteParentAllocator = false;

	public:
		explicit PoolAllocator(size_t size = POOL_ALLOCATOR_DEFAULT_SIZE, Allocator* parentAllocator = nullptr)
			:m_size(size)
			,m_parentAllocator(parentAllocator)
		{
			if (parentAllocator == nullptr)
			{
				m_parentAllocator = new Allocator();
				m_needsToDeleteParentAllocator = true;
			}

			m_data = m_parentAllocator->allocate(m_size);
			m_head = m_data;

			for (size_t i = 0; i < m_size - 1; i++)
			{
				m_data[i].nextPoolChunk = std::addressof(m_data[i + 1]);
			}
			m_data[m_size - 1].nextPoolChunk = nullptr;
		}

		~PoolAllocator()
		{
			if (m_openAllocations)
			{
				//logger::logError<const char*>("In PoolAllocator: Not all allocations were successfully deallocated!\n");
			}

			m_parentAllocator->deallocate(m_data, m_size);
			if (m_needsToDeleteParentAllocator)
			{
				delete m_parentAllocator;
			}

			m_data = nullptr;
			m_head = nullptr;
		}

		PoolAllocator(const PoolAllocator& other)				= delete; //copy constructor
		PoolAllocator(PoolAllocator&& other)					= delete; //move constructor
		PoolAllocator& operator=(const PoolAllocator& other)	= delete; //copy Assignment
		PoolAllocator& operator=(PoolAllocator&& other)			= delete; //move Assignment

		template<typename... arguments>
		T* allocate(arguments&&...  args) //variable number of arguments for T
		{
			if (m_head == nullptr)
			{
				//logger::logError<const char*>("In PoolAllocator: PoolAllocator is full!\n");
				return nullptr;
			}

			m_openAllocations++;

			PoolChunk<T>* poolChunk = m_head;
			m_head = m_head->nextPoolChunk;
			T* retVal = new (std::addressof(poolChunk->value)) T(std::forward<arguments>(args)...);
			return retVal;
		}

		void deallocate(T* data)
		{
			m_openAllocations--;

			data->~T();
			PoolChunk<T>* poolChunk = reinterpret_cast<PoolChunk<T>*>(data); //only allowed if the pointer was a PoolChunk<T> before
			poolChunk->nextPoolChunk = m_head;
			m_head = poolChunk;
		}

	};
} // namespace Amber3D