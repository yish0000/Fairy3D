/*
 * ------------------------------------------------------------------------
 *  Name:   FContainer.h
 *  Desc:   STL container wrapper.
 *  Author: Yish
 *  Date:   2014/1/28
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Fairy & Pear Studio. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_CONTAINER_H__
#define __FAIRY_CONTAINER_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FMemConfig.h"

///////////////////////////////////////////////////////////////////////////

namespace falloc {

#define STL_ALLOC_CATEGOTY_DEFAULT 0
#define STL_ALLOC_CATEGOTY_GENERAL 1
#define STL_ALLOC_CATEGOTY_TEMP 2

	// Allocator for all the containers
	template <class T, int CAT>
	class base_allocator
	{
	public:
		typedef T value_type;
		typedef value_type* pointer;
		typedef value_type& reference;
		typedef const value_type* const_pointer;
		typedef const value_type& const_reference;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;

		template <typename U, int CAT2>
		struct rebind
		{
			// convert an base_allocator<T> to an base_allocator<U>
			typedef base_allocator<U, CAT2> other;
		};

		// constructor
		explicit base_allocator() {}

		// copy constructor
		base_allocator(const base_allocator& rhs) {}

		// construct from a related allocator (do nothing)
		template <class U, int CAT2>
		base_allocator(const base_allocator<U, CAT2>& rhs) {}

		// destructor
		~base_allocator() {}

		// assign from a related allocator (do nothing)
		template <class U, int CAT2>
		base_allocator<T, CAT>& operator = (const base_allocator<U, CAT2>& rhs)
		{
			return *this;
		}

		// memory allocation
		pointer allocate(size_t size)
		{
			size_type sz = size * sizeof(T);

			pointer ptr = NULL;
			if (CAT == STL_ALLOC_CATEGOTY_DEFAULT)
				ptr = (pointer)malloc(sz);
			else if (CAT == STL_ALLOC_CATEGOTY_GENERAL)
				ptr = (pointer)F_MALLOC(sz);
			else if (CAT == STL_ALLOC_CATEGOTY_TEMP)
				ptr = (pointer)F_MALLOC_TEMP(sz);

			return ptr;
		}

		// memory allocation
		pointer allocate(size_t size, const void*)
		{
			return allocate(size);
		}

		// memory deallocation
		void deallocate(pointer ptr, size_type)
		{
			if (CAT == STL_ALLOC_CATEGOTY_DEFAULT)
				free(ptr);
			else if (CAT == STL_ALLOC_CATEGOTY_GENERAL)
				F_FREE(ptr);
			else if (CAT == STL_ALLOC_CATEGOTY_TEMP)
				F_FREE_TEMP(ptr);
		}

		// return address of mutable value
		pointer address(reference val)
		{
			return &val;
		}

		// return address of unmutable value
		const_pointer address(const_reference val)
		{
			return &val;
		}

		// get the max size we can allocate
		size_type max_size() const throw()
		{
			size_type cnt = (size_type)-1 / sizeof(T);
			return 0 < cnt ? cnt : 1;
		}

		// construct object at 'ptr' with value 'val'
		void construct(pointer ptr, const T& val)
		{
			void* p = static_cast<void*>(ptr);
			::new (p)T(val);
		}

		// destroy object at 'ptr'
		void destroy(pointer ptr)
		{
			ptr->~T();
		}
	};

	// test for allocator equality (always true)
	template <class T, class U, int CAT>
	bool operator == (const base_allocator<T, CAT>&, const base_allocator<U, CAT>&)
	{
		return true;
	}

	// test for allocator inequality (always false)
	template <class T, class U, int CAT>
	bool operator != (const base_allocator<T, CAT>&, const base_allocator<U, CAT>&)
	{
		return false;
	}

	// Allocator for stl containers.
	template <class T> class default_stl_alloc : public base_allocator<T, STL_ALLOC_CATEGOTY_DEFAULT> {};
	template <class T> class general_stl_alloc : public base_allocator<T, STL_ALLOC_CATEGOTY_GENERAL> {};
	template <class T> class temp_stl_alloc : public base_allocator<T, STL_ALLOC_CATEGOTY_TEMP> {};
}

///////////////////////////////////////////////////////////////////////////
// Methods for containers

#define IMPLEMENT_CONTAINER_COUT(c) \
	template <class T> \
	std::ostream& operator << (std::ostream& o, const c<T>& t) \
	{ \
		o << #c "(size:" << t.size() << ", content:("; \
		typename c<T>::const_iterator it; \
		for (it = t.begin(); it != t.end(); ++it) { \
			if (it == --t.end()) o << *it; \
			else o << *it << ","; \
		} \
		o << "))"; \
		return o; \
	}

#define IMPLEMENT_CONTAINER_COUT2(c) \
	template <class K, class P> \
	std::ostream& operator << (std::ostream& o, const c<K, P>& t) \
	{ \
		o << #c "(size:" << t.size() << ", content:("; \
		typename c<K, P>::const_iterator it; \
		for (it = t.begin(); it != t.end(); ++it) { \
			if (it == --t.end()) o << *it; \
			else o << *it << ","; \
		} \
		o << "))"; \
		return o; \
	}

#define IMPLEMENT_CONTAINER_MAP_COUT(c) \
	template <class K, class T, class P> \
	std::ostream& operator << (std::ostream& o, const c<K, T, P>& t) \
	{ \
		o << #c "(size:" << t.size() << ", content:("; \
		typename c<K, T, P>::const_iterator it; \
		for (it = t.begin(); it != t.end(); ++it) { \
			if (it == --t.end()) o << it->first << ":" << it->second; \
			else o << it->first << ":" << it->second << ", "; \
		} \
		o << "))"; \
		return o; \
	}

IMPLEMENT_CONTAINER_COUT(std::vector)
IMPLEMENT_CONTAINER_COUT(std::list)
IMPLEMENT_CONTAINER_COUT(std::deque)
IMPLEMENT_CONTAINER_COUT2(std::set)
IMPLEMENT_CONTAINER_COUT2(std::multiset)
IMPLEMENT_CONTAINER_COUT2(std::unordered_set)
IMPLEMENT_CONTAINER_MAP_COUT(std::map)
IMPLEMENT_CONTAINER_MAP_COUT(std::multimap)
IMPLEMENT_CONTAINER_MAP_COUT(std::unordered_map)

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FAIRY_CONTAINER_H__