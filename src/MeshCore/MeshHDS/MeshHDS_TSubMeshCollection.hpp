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
 * NetgenPlugin : C++ implementation
 * File      	: MeshHDS_TSubMeshCollection.hpp
 * Author    	: Paweł Gilewicz
 * Date      	: 27/10/2024
 */
#ifndef MeshHDS_TSUBMESHCOLLECTION_H
#define MeshHDS_TSUBMESHCOLLECTION_H

#include "MeshDS_Iterator.hpp"

#include <map>
#include <vector>

//=======================================================================
/*!
 * \brief A binder of a sub-mesh to its ID which can be negative.
 *        Provides fast access to a sub-mesh by its ID.
 */
//=======================================================================

template <class SUBMESH>
class MeshHDS_TSubMeshCollection {
	std::vector<SUBMESH*> myVec; // for ID >= 0
	std::map<int, SUBMESH*> myMap; // for ID < 0

public:
	~MeshHDS_TSubMeshCollection() {
		DeleteAll();
	}
	void Add(int id, SUBMESH* sm) {
		if (id < 0) {
			myMap[id] = sm;
		} else {
			if ((int)myVec.size() <= id)
				myVec.resize(id + 1, (SUBMESH*)NULL);
			myVec[id] = sm;
		}
	}
	SUBMESH* Get(int id) const {
		if (id < 0) {
			typename std::map<int, SUBMESH*>::const_iterator i2sm = myMap.find(id);
			return (SUBMESH*)(i2sm == myMap.end() ? NULL : i2sm->second);
		} else {
			return (SUBMESH*)(id >= (int)myVec.size() ? NULL : myVec[id]);
		}
	}
	void DeleteAll() {
		for (size_t i = 0; i < myVec.size(); ++i)
			if (SUBMESH* sm = myVec[i]) {
				myVec[i] = 0; // avoid access via Get(i)
				delete sm;
			}
		myVec.clear();

		typename std::map<int, SUBMESH*>::iterator i2sm = myMap.begin();
		for (; i2sm != myMap.end(); ++i2sm)
			if (SUBMESH* sm = i2sm->second) {
				i2sm->second = 0; // avoid access via Get(i)
				delete sm;
			}
		myMap.clear();
	}
	int GetMinID() const {
		return myMap.empty() ? 0 : myMap.begin()->first;
	}
	int GetMaxID() const {
		return myVec.empty() ? (myMap.empty() ? 0 : myMap.rbegin()->first) : myVec.size();
	}

	//-----------------------------------------------------------------------
	struct Iterator : public MeshDS_Iterator<SUBMESH*> {
		const MeshHDS_TSubMeshCollection<SUBMESH>* myHolder;
		SUBMESH* myNext;
		int myCurID, myEndID, myIDDelta;

		void init(const MeshHDS_TSubMeshCollection<SUBMESH>* holder,
			int firstID, int endID, int delta) {
			myHolder = holder;
			myNext = 0;
			myCurID = firstID;
			myEndID = endID;
			myIDDelta = delta;

			next();
		}

		bool more() {
			return myNext;
		}

		SUBMESH* next() {
			SUBMESH* res = myNext;
			myNext = 0;
			while (!myNext && myCurID != myEndID) {
				myNext = myHolder->Get(myCurID);
				myCurID += myIDDelta;
			}
			return res;
		}
		virtual ~Iterator() { }
	};
	//-----------------------------------------------------------------------

	MeshDS_Iterator<SUBMESH*>* GetIterator(const bool reverse = false) const {
		Iterator* iter = new Iterator;
		if (reverse)
			iter->init(this, GetMaxID(), GetMinID() - 1, -1);
		else
			iter->init(this, GetMinID(), GetMaxID() + 1, +1);
		return iter;
	}
};

#endif
