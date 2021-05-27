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
#pragma once
#include <SofaBoundaryCondition/config.h>

#include <sofa/core/behavior/ProjectiveConstraintSet.h>
#include <SofaBaseTopology/TopologySubsetData.h>
#include <sofa/helper/vector.h>

namespace sofa::component::projectiveconstraintset
{

/// This class can be overridden if needed for additionnal storage within template specializations.
template <class DataTypes>
class FixedTranslationConstraintInternalData
{
};

/** Attach given particles to their initial positions.
*/
template <class DataTypes>
class FixedTranslationConstraint : public core::behavior::ProjectiveConstraintSet<DataTypes>
{
public:
    SOFA_CLASS(SOFA_TEMPLATE(FixedTranslationConstraint,DataTypes),SOFA_TEMPLATE(sofa::core::behavior::ProjectiveConstraintSet, DataTypes));

    using Index = sofa::Index;
    typedef typename DataTypes::VecCoord VecCoord;
    typedef typename DataTypes::VecDeriv VecDeriv;
    typedef typename DataTypes::MatrixDeriv MatrixDeriv;
    typedef typename DataTypes::Coord Coord;
    typedef typename DataTypes::Deriv Deriv;
    typedef typename MatrixDeriv::RowType MatrixDerivRowType;
    typedef typename MatrixDeriv::RowIterator MatrixDerivRowIterator;
    typedef Data<VecCoord> DataVecCoord;
    typedef Data<VecDeriv> DataVecDeriv;
    typedef Data<MatrixDeriv> DataMatrixDeriv;
    typedef helper::vector<Index> SetIndexArray;
    typedef sofa::component::topology::PointSubsetData< SetIndexArray > SetIndex;
protected:
    FixedTranslationConstraintInternalData<DataTypes> data;
    friend class FixedTranslationConstraintInternalData<DataTypes>;

public:
    SetIndex f_indices; ///< Indices of the fixed points
    Data<bool> f_fixAll; ///< filter all the DOF to implement a fixed object
    Data<SReal> _drawSize; ///< 0 -> point based rendering, >0 -> radius of spheres
    SetIndex f_coordinates; ///< Coordinates of the fixed points

    /// Link to be set to the topology container in the component graph.
    SingleLink<FixedTranslationConstraint<DataTypes>, sofa::core::topology::BaseMeshTopology, BaseLink::FLAG_STOREPATH | BaseLink::FLAG_STRONGLINK> l_topology;
protected:
    FixedTranslationConstraint();

    virtual ~FixedTranslationConstraint();
public:
    // methods to add/remove some indices
    void clearIndices();
    void addIndex(Index index);
    void removeIndex(Index index);

    // -- Constraint interface
    void init() override;

    void projectResponse(const core::MechanicalParams* mparams, DataVecDeriv& resData) override;
    void projectVelocity(const core::MechanicalParams* mparams, DataVecDeriv& vData) override;
    void projectPosition(const core::MechanicalParams* mparams, DataVecCoord& xData) override;
    void projectJacobianMatrix(const core::MechanicalParams* mparams, DataMatrixDeriv& cData) override;


    void draw(const core::visual::VisualParams* vparams) override;

    class FCPointHandler : public sofa::component::topology::TopologyDataHandler<core::topology::BaseMeshTopology::Point, SetIndexArray >
    {
    public:
        typedef typename FixedTranslationConstraint<DataTypes>::SetIndexArray SetIndexArray;
        typedef sofa::core::topology::Point Point;
        FCPointHandler(FixedTranslationConstraint<DataTypes>* _fc, sofa::component::topology::PointSubsetData<SetIndexArray>* _data)
            : sofa::component::topology::TopologyDataHandler<core::topology::BaseMeshTopology::Point, SetIndexArray >(_data), fc(_fc) {}



        void applyDestroyFunction(Index /*index*/, value_type& /*T*/);


        bool applyTestCreateFunction(Index /*index*/,
                const sofa::helper::vector< Index > & /*ancestors*/,
                const sofa::helper::vector< double > & /*coefs*/);
    protected:
        FixedTranslationConstraint<DataTypes> *fc;
    };

protected:
    template <class DataDeriv>
    void projectResponseT(const core::MechanicalParams* mparams, DataDeriv& dx);

    /// Handler for subset Data
    FCPointHandler* m_pointHandler;

};

#if  !defined(SOFA_COMPONENT_PROJECTIVECONSTRAINTSET_FIXEDTRANSLATIONCONSTRAINT_CPP)
extern template class SOFA_SOFABOUNDARYCONDITION_API FixedTranslationConstraint<defaulttype::Rigid3Types>;
extern template class SOFA_SOFABOUNDARYCONDITION_API FixedTranslationConstraint<defaulttype::Rigid2Types>;
extern template class SOFA_SOFABOUNDARYCONDITION_API FixedTranslationConstraint<defaulttype::Vec6Types>;

#endif

} // namespace sofa::component::projectiveconstraintset
