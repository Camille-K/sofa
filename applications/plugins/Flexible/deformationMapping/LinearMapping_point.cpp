/******************************************************************************
*                 SOFA, Simulation Open-Framework Architecture                *
*                    (c) 2006 INRIA, USTL, UJF, CNRS, MGH                     *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#define SOFA_COMPONENT_MAPPING_LINEARMAPPING_point_CPP

#include <Flexible/config.h>
#include "LinearMapping.h"
#include <sofa/core/ObjectFactory.h>

#include <sofa/defaulttype/VecTypes.h>
#include "../types/DeformationGradientTypes.h"
#include "../types/AffineTypes.h"

#include "BaseDeformationMapping.inl"

namespace sofa
{
namespace component
{
namespace mapping
{

using namespace defaulttype;

// Register in the Factory
int LinearMappingClass_point = core::RegisterObject("Map child positions as a linear combination of parents.")

        .add< LinearMapping< Vec3Types, Vec3Types > >(true)
        .add< LinearMapping< Vec3Types, F331Types > >()
        .add< LinearMapping< Vec3Types, F332Types > >()
        .add< LinearMapping< Vec3Types, F321Types > >()
        .add< LinearMapping< Vec3Types, F311Types > >()
        .add< LinearMapping< Vec2Types, Vec2Types > >()
        .add< LinearMapping< Vec2Types, F221Types > >()
        .add< LinearMapping< Vec3Types, Affine3Types > >()
        ;

template class SOFA_Flexible_API LinearMapping< Vec3Types, Vec3Types >;
template class SOFA_Flexible_API LinearMapping< Vec3Types, F331Types >;
template class SOFA_Flexible_API LinearMapping< Vec3Types, F332Types >;
template class SOFA_Flexible_API LinearMapping< Vec3Types, F321Types >;
template class SOFA_Flexible_API LinearMapping< Vec3Types, F311Types >;
template class SOFA_Flexible_API LinearMapping< Vec2Types, Vec2Types >;
template class SOFA_Flexible_API LinearMapping< Vec2Types, F221Types >;
template class SOFA_Flexible_API LinearMapping< Vec3Types, Affine3Types >;

} // namespace mapping
} // namespace component
} // namespace sofa

