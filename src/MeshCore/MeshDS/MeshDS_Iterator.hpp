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
 * File      : MeshDS_Iterator.hpp
 * Author    : Paweł Gilewicz
 * Date      : 27/10/2024
 */

#ifndef MeshDS_ITERATOR_HPP
#define MeshDS_ITERATOR_HPP

// Abstract class for iterators
template <typename VALUE>
class MeshDS_Iterator {
public:
	typedef VALUE value_type;

	/// Return true if and only if there are other object in this iterator
	virtual bool more() = 0;

	/// Return the current object and step to the next one
	virtual VALUE next() = 0;

	/// Delete the current element and step to the next one
	virtual void remove() { }

	/// Provide virtual destructor just for case if some derived iterator
	/// must have a destructor
	virtual ~MeshDS_Iterator() { }
};

#endif