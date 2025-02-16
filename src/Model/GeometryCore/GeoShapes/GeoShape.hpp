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

#ifndef GEOSHAPE_HPP
#define GEOSHAPE_HPP

#include <TDF_Label.hxx>
#include <string>

#include "BaseShape.hpp"

class GeoShape : public BaseShape {
   
    friend class ShapeFactory;
    public:

        GeoShape(const GeoShape& aShape) = delete;
        GeoShape& operator=(const GeoShape& aShape) = delete;

        virtual ~GeoShape();      

        TopoDS_Shape shape() const override;
        TDF_Label ocafLabel() const;
        const ShapeId id() const override;

    private:

        GeoShape(const ShapeId& aShapeId,
                 const TopoDS_Shape& aShape,
                 const TDF_Label& aLabel);
    
    protected:
        
        TopoDS_Shape _shape;
        const ShapeId _ShapeId;
        const TDF_Label _ocafLabel;
};
#endif
