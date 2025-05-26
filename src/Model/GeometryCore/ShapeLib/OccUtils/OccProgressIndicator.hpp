/*
 * Copyright (C) 2024 Krystian Fudali
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
 */

#ifndef OCCPROGRESSINDICATOR_HPP 
#define OCCPROGRESSINDICATOR_HPP 

#include <Message_ProgressIndicator.hxx>
#include "ModelEvents.hpp"
#include "ProgressIndicator.hpp"
#include <sstream>

class OccProgressIndicator : public Message_ProgressIndicator{
        
DEFINE_STANDARD_RTTIEXT(OccProgressIndicator, Message_ProgressIndicator);
	public:
	Standard_EXPORT OccProgressIndicator(
		const ProgressIndicator& aProgressIndicator
	);

	Standard_EXPORT void Show(
		const Message_ProgressScope& aScope, 
		const Standard_Boolean force = Standard_True) Standard_OVERRIDE;

	private:
	
	std::stringstream progressMessage(const Message_ProgressScope& aScope);
    const ProgressIndicator& _progressIndicator;
};



#endif