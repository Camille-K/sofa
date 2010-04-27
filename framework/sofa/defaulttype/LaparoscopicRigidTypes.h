/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 4      *
*                (c) 2006-2009 MGH, INRIA, USTL, UJF, CNRS                    *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to the Free Software Foundation,     *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.          *
*******************************************************************************
*                              SOFA :: Framework                              *
*                                                                             *
* Authors: M. Adam, J. Allard, B. Andre, P-J. Bensoussan, S. Cotin, C. Duriez,*
* H. Delingette, F. Falipou, F. Faure, S. Fonteneau, L. Heigeas, C. Mendoza,  *
* M. Nesme, P. Neumann, J-P. de la Plata Alcade, F. Poyer and F. Roy          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFA_DEFAULTTYPE_LAPAROSCOPICRIGIDTYPES_H
#define SOFA_DEFAULTTYPE_LAPAROSCOPICRIGIDTYPES_H


#include <sofa/defaulttype/RigidTypes.h>
#include <sofa/defaulttype/SparseConstraintTypes.h>
#include <sofa/defaulttype/Vec.h>
#include <sofa/core/objectmodel/BaseContext.h>
#include <sofa/core/componentmodel/behavior/Mass.h>
#include <sofa/helper/vector.h>
#include <iostream>
#ifdef SOFA_SMP
#include <sofa/defaulttype/SharedTypes.h>
#endif /* SOFA_SMP */
#include <sofa/defaulttype/DataTypeInfo.h>

namespace sofa
{

namespace defaulttype
{

#ifndef SOFA_SMP
using sofa::helper::vector;
#endif /* SOFA_SMP */

class LaparoscopicRigid3Types
{
public:

    typedef SReal Real;

    class Deriv
    {
    private:
        Real vTranslation;
        Vector3 vOrientation;
    public:
        typedef Real value_type;
        typedef Real Pos;
        typedef Vector3 Rot;
        friend class Coord;

        Deriv (const Real &velTranslation, const Vector3 &velOrient)
            : vTranslation(velTranslation), vOrientation(velOrient) {}
        Deriv () { clear(); }

        void clear() { vTranslation = 0; vOrientation.clear(); }

        void operator +=(const Deriv& a)
        {
            vTranslation += a.vTranslation;
            vOrientation += a.vOrientation;
        }

        Deriv operator + (const Deriv& a) const
        {
            Deriv d;
            d.vTranslation = vTranslation + a.vTranslation;
            d.vOrientation = vOrientation + a.vOrientation;
            return d;
        }

        void operator*=(Real a)
        {
            vTranslation *= a;
            vOrientation *= a;
        }

        Deriv operator*(Real a) const
        {
            Deriv r = *this;
            r*=a;
            return r;
        }

        Deriv operator - () const
        {
            return Deriv(-vTranslation, -vOrientation);
        }

        /// dot product
        Real operator*(const Deriv& a) const
        {
            return vTranslation*a.vTranslation
                    +vOrientation[0]*a.vOrientation[0]+vOrientation[1]*a.vOrientation[1]
                    +vOrientation[2]*a.vOrientation[2];
        }

        Real& getVTranslation (void) { return vTranslation; }
        Vector3& getVOrientation (void) { return vOrientation; }
        const Real& getVTranslation (void) const { return vTranslation; }
        const Vector3& getVOrientation (void) const { return vOrientation; }
        inline friend std::ostream& operator << (std::ostream& out, const Deriv& v )
        {
            out<<v.getVTranslation();
            out<<" "<<v.getVOrientation();
            return out;
        }
        inline friend std::istream& operator >> (std::istream& in, Deriv& v )
        {
            in>>v.vTranslation;
            in>>v.vOrientation;
            return in;
        }

        enum { static_size = 1 };
        Real* ptr() { return &vTranslation; }
        const Real* ptr() const { return &vTranslation; }

        static unsigned int size() { return 4; }
        Real& operator[](int i)
        {
            if (i < 1) return this->vTranslation;
            else       return this->vOrientation(i-1);
        }
        const Real& operator[](int i) const
        {
            if (i < 1) return this->vTranslation;
            else       return this->vOrientation(i-1);
        }

    };

    class Coord
    {

    private:
        Real translation;
        Quat orientation;
    public:
        typedef Real value_type;
        typedef Real Pos;
        typedef Quat Rot;
        Coord (const Real &posTranslation, const Quat &orient)
            : translation(posTranslation), orientation(orient) {}
        Coord () { clear(); }

        void clear() { translation = 0; orientation.clear(); }

        void operator +=(const Deriv& a)
        {
            translation += a.getVTranslation();
            orientation.normalize();
            Quat qDot = orientation.vectQuatMult(a.getVOrientation());
            for (int i = 0; i < 4; i++)
                orientation[i] += qDot[i] * (SReal)0.5;
            orientation.normalize();
        }

        Coord operator + (const Deriv& a) const
        {
            Coord c = *this;
            c.translation += a.getVTranslation();
            c.orientation.normalize();
            Quat qDot = c.orientation.vectQuatMult(a.getVOrientation());
            for (int i = 0; i < 4; i++)
                c.orientation[i] += qDot[i] * (SReal)0.5;
            c.orientation.normalize();
            return c;
        }

        void operator +=(const Coord& a)
        {
// 			std::cout << "+="<<std::endl;
            translation += a.getTranslation();
            //orientation += a.getOrientation();
            //orientation.normalize();
        }

        void operator*=(Real a)
        {
// 			std::cout << "*="<<std::endl;
            translation *= a;
            //orientation *= a;
        }

        Coord operator*(Real a) const
        {
            Coord r = *this;
            r*=a;
            return r;
        }

        /// dot product (FF: WHAT????  )
        Real operator*(const Coord& a) const
        {
            return translation*a.translation
                    +orientation[0]*a.orientation[0]+orientation[1]*a.orientation[1]
                    +orientation[2]*a.orientation[2]+orientation[3]*a.orientation[3];
        }

        Real& getTranslation () { return translation; }
        Quat& getOrientation () { return orientation; }
        const Real& getTranslation () const { return translation; }
        const Quat& getOrientation () const { return orientation; }
        inline friend std::ostream& operator << (std::ostream& out, const Coord& c )
        {
            out<<c.getTranslation();
            out<<" "<<c.getOrientation();
            return out;
        }
        inline friend std::istream& operator >> (std::istream& in, Coord& c )
        {
            in>>c.translation;
            in>>c.orientation;
            return in;
        }

        static Coord identity()
        {
            Coord c;
            return c;
        }

        /// Apply a transformation with respect to itself
        void multRight( const Coord& c )
        {
            translation += c.getTranslation();
            orientation = orientation * c.getOrientation();
        }

        /// compute the product with another frame on the right
        Coord mult( const Coord& c ) const
        {
            Coord r;
            r.translation = translation + c.translation; //orientation.rotate( c.translation );
            r.orientation = orientation * c.getOrientation();
            return r;
        }
        /// compute the projection of a vector from the parent frame to the child
        Vector3 vectorToChild( const Vector3& v ) const
        {
            return orientation.inverseRotate(v);
        }


        enum { static_size = 1 };
        Real* ptr() { return &translation; }
        const Real* ptr() const { return &translation; }

        static unsigned int size() { return 5; }
        Real& operator[](int i)
        {
            if (i < 1) return this->translation;
            else       return this->orientation[i-1];
        }
        const Real& operator[](int i) const
        {
            if (i < 1) return this->translation;
            else       return this->orientation[i-1];
        }
    };

    typedef Coord::Pos CPos;
    typedef Coord::Rot CRot;
    static const CPos& getCPos(const Coord& c) { return c.getTranslation(); }
    static void setCPos(Coord& c, const CPos& v) { c.getTranslation() = v; }
    static const CRot& getCRot(const Coord& c) { return c.getOrientation(); }
    static void setCRot(Coord& c, const CRot& v) { c.getOrientation() = v; }

    typedef Deriv::Pos DPos;
    typedef Deriv::Rot DRot;
    static const DPos& getDPos(const Deriv& d) { return d.getVTranslation(); }
    static void setDPos(Deriv& d, const DPos& v) { d.getVTranslation() = v; }
    static const DRot& getDRot(const Deriv& d) { return d.getVOrientation(); }
    static void setDRot(Deriv& d, const DRot& v) { d.getVOrientation() = v; }


    typedef SparseConstraint<Coord> SparseVecCoord;
    typedef SparseConstraint<Deriv> SparseVecDeriv;

    //! All the Constraints applied to a state Vector
#ifndef SOFA_SMP
    typedef	vector<SparseVecDeriv> VecConst;
#else /* SOFA_SMP */
    typedef	SharedVector<SparseVecDeriv> VecConst;
#endif /* SOFA_SMP */

#ifndef SOFA_SMP

    typedef vector<Coord> VecCoord;
    typedef vector<Deriv> VecDeriv;
    typedef vector<Real> VecReal;
#else /* SOFA_SMP */
    typedef SharedVector<Coord> VecCoord;
    typedef SharedVector<Deriv> VecDeriv;
    typedef SharedVector<Real> VecReal;
#endif /* SOFA_SMP */

    template<typename T>
    static void set(Coord& c, T x, T, T)
    {
        c.getTranslation() = (Real)x;
    }

    template<typename T>
    static void get(T& x, T&, T&, const Coord& c)
    {
        x = (T)c.getTranslation();
    }

    template<typename T>
    static void add(Coord& c, T x, T, T)
    {
        c.getTranslation() += (Real)x;
    }

    template<typename T>
    static void set(Deriv& d, T x, T, T)
    {
        d.getVTranslation() = (Real)x;
    }

    template<typename T>
    static void get(T& x, T& y, T& z, const Deriv& d)
    {
        x = (T)d.getVTranslation();
        y = (T)0;
        z = (T)0;
    }

    template<typename T>
    static void add(Deriv& d, T x, T, T)
    {
        d.getVTranslation() += (T)x;
    }
    static const char* Name()
    {
        return "LaparoscopicRigid3";
    }

    static Coord interpolate(const helper::vector< Coord > &ancestors, const helper::vector< Real > &coefs)
    {
        assert(ancestors.size() == coefs.size());

        Coord c;

        for (unsigned int i = 0; i < ancestors.size(); i++)
        {
            c += ancestors[i] * coefs[i];
        }

        return c;
    }

    static Deriv interpolate(const helper::vector< Deriv > &ancestors, const helper::vector< Real > &coefs)
    {
        assert(ancestors.size() == coefs.size());

        Deriv d;

        for (unsigned int i = 0; i < ancestors.size(); i++)
        {
            d += ancestors[i] * coefs[i];
        }

        return d;
    }
};

inline LaparoscopicRigid3Types::Deriv operator*(const LaparoscopicRigid3Types::Deriv& d, const Rigid3Mass& m)
{
    LaparoscopicRigid3Types::Deriv res;
    res.getVTranslation() = d.getVTranslation() * m.mass;
    res.getVOrientation() = m.inertiaMassMatrix * d.getVOrientation();
    return res;
}

inline LaparoscopicRigid3Types::Deriv operator/(const LaparoscopicRigid3Types::Deriv& d, const Rigid3Mass& m)
{
    LaparoscopicRigid3Types::Deriv res;
    res.getVTranslation() = d.getVTranslation() / m.mass;
    res.getVOrientation() = m.invInertiaMassMatrix * d.getVOrientation();
    return res;
}

typedef LaparoscopicRigid3Types LaparoscopicRigidTypes; ///< Alias

// Specialization of the defaulttype::DataTypeInfo type traits template

template<>
struct DataTypeInfo< sofa::defaulttype::LaparoscopicRigid3Types::Deriv > : public FixedArrayTypeInfo< sofa::defaulttype::LaparoscopicRigid3Types::Deriv >
{
    static const char* name() { return "LaparoscopicRigid3Types::Deriv"; }
};

template<>
struct DataTypeInfo< sofa::defaulttype::LaparoscopicRigid3Types::Coord > : public FixedArrayTypeInfo< sofa::defaulttype::LaparoscopicRigid3Types::Coord >
{
    static const char* name() { return "LaparoscopicRigid3Types::Coord"; }
};

} // namespace defaulttype


//================================================================================================================
// This is probably useless because the RigidObject actually contains its mass and computes its inertia forces itself:
//================================================================================================================

namespace core
{
namespace componentmodel
{
namespace behavior
{
/// Specialization of the inertia force for defaulttype::RigidTypes
template <>
inline defaulttype::LaparoscopicRigid3Types::Deriv inertiaForce<
defaulttype::LaparoscopicRigid3Types::Coord,
            defaulttype::LaparoscopicRigid3Types::Deriv,
            defaulttype::Vector3,
            defaulttype::Rigid3Mass,
            objectmodel::BaseContext::SpatialVector
            >
            (
                    const objectmodel::BaseContext::SpatialVector& vframe,
                    const defaulttype::Vector3& aframe,
                    const defaulttype::Rigid3Mass& mass,
                    const defaulttype::LaparoscopicRigid3Types::Coord& x,
                    const defaulttype::LaparoscopicRigid3Types::Deriv& v )
{
    defaulttype::Vector3 omega( vframe.lineVec[0], vframe.lineVec[1], vframe.lineVec[2] );
    defaulttype::Vector3 origin, finertia, zero(0,0,0);
    origin[0] = x.getTranslation();

    finertia = -( aframe + omega.cross( omega.cross(origin) + defaulttype::Vector3(v.getVTranslation()*2,0,0) ))*mass.mass;
    return defaulttype::LaparoscopicRigid3Types::Deriv( finertia[0], zero );
    /// \todo replace zero by Jomega.cross(omega)
}

} // namespace behavoir

} // namespace componentmodel

} // namespace core

} // namespace sofa

#endif
