/*
 * Copyright (C) 2024 Paweł Gilewicz
 *
 * This file is part of the Mesh Generating Tool. (https://github.com/PawelekPro/MeshGeneratingTool)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Mesh      : C++ implementation
 * File      : MeshDS_IteratorsAux.hpp
 * Author    : Paweł Gilewicz
 * Date      : 27/10/2024
 */

#ifndef MeshDS_ITERATORSAUX_HPP
#define MeshDS_ITERATORSAUX_HPP

#include "MeshDS_Iterator.hpp"

namespace MeshDS {

template <typename VALUE, typename VALUE_SET_ITERATOR>
struct SimpleAccessor {
	static VALUE value(VALUE_SET_ITERATOR it) { return (VALUE)*it; }
};

template <typename VALUE>
struct PassAllValueFilter {
	bool operator()(const VALUE& /*t*/) { return true; }
};

template <typename VALUE, typename VALUE_SET_ITERATOR>
struct ValueAccessor {
	static VALUE value(VALUE_SET_ITERATOR it) { return (VALUE)it->second; }
};

}

template <typename VALUE,
	typename VALUE_SET_ITERATOR,
	typename ACCESOR = MeshDS::SimpleAccessor<VALUE, VALUE_SET_ITERATOR>,
	typename VALUE_FILTER = MeshDS::PassAllValueFilter<VALUE>>
class MeshDS_SetIterator : public MeshDS_Iterator<VALUE> {
protected:
	VALUE_SET_ITERATOR _beg, _end;
	VALUE_FILTER _filter;

public:
	MeshDS_SetIterator(const VALUE_SET_ITERATOR& begin,
		const VALUE_SET_ITERATOR& end,
		const VALUE_FILTER& filter = VALUE_FILTER()) { init(begin, end, filter); }

	/// Initialization
	virtual void init(const VALUE_SET_ITERATOR& begin,
		const VALUE_SET_ITERATOR& end,
		const VALUE_FILTER& filter = VALUE_FILTER()) {
		_beg = begin;
		_end = end;
		_filter = filter;
		if (more() && !_filter(ACCESOR::value(_beg)))
			next();
	}
	/// Return true iff there are other object in this iterator
	virtual bool more() {
		return _beg != _end;
	}
	/// Return the current object and step to the next one
	virtual VALUE next() {
		VALUE ret = ACCESOR::value(_beg++);
		while (more() && !_filter(ACCESOR::value(_beg)))
			++_beg;
		return ret;
	}
};

template <typename M>
struct MeshDS_mapIterator : public MeshDS_SetIterator<typename M::mapped_type, typename M::const_iterator,
								MeshDS::ValueAccessor<typename M::mapped_type,
									typename M::const_iterator>> {
	typedef MeshDS_SetIterator<typename M::mapped_type, typename M::const_iterator,
		MeshDS::ValueAccessor<typename M::mapped_type, typename M::const_iterator>>
		parent_type;

	MeshDS_mapIterator(const M& m)
		: parent_type(m.begin(), m.end()) { }
};

template <typename M>
struct MeshDS_mapReverseIterator : public MeshDS_SetIterator<typename M::mapped_type,
									   typename M::const_reverse_iterator,
									   MeshDS::ValueAccessor<typename M::mapped_type,
										   typename M::const_reverse_iterator>> {
	typedef MeshDS_SetIterator<typename M::mapped_type, typename M::const_reverse_iterator,
		MeshDS::ValueAccessor<typename M::mapped_type, typename M::const_reverse_iterator>>
		parent_type;

	MeshDS_mapReverseIterator(const M& m)
		: parent_type(m.rbegin(), m.rend()) { }
};

#endif
