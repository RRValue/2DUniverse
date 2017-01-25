#pragma once

#include "HalfEdge2D/Base/BernsteinMatrix.h"

#include <Eigen/Core>

#include <array>

template<size_t G>
struct BezierParameterGradeTraits
{
    BezierParameterGradeTraits()
    {
        static_assert(G > 0, "Grade must be greaster than 0");
    }
};

template<typename T>
struct BezierParameterTypeTraits
{
    BezierParameterTypeTraits()
    {
        static_assert(false, "BezierParameter not defined for this Type");
    }
};

template<> struct BezierParameterTypeTraits<float>{};
template<> struct BezierParameterTypeTraits<double>{};

template<typename T, size_t G, size_t D>
class BezierParameter
{
private:
    typedef Eigen::Matrix<T, D, 1> ParameterType;
    typedef Eigen::Matrix<T, D, G + 1> ParameterEntryType;
    typedef std::array<ParameterEntryType, G + 1> ParameterEntriesArray;
    
    typedef Eigen::Matrix<T, G + 1, 1> ProcessType;

protected:
    BezierParameter()
    {
        m_Parameter.setZero();

        for(size_t i = 0; i <= G; i++)
            m_DerivedParameter[i].setZero();
    }

    ~BezierParameter()
    {

    }

    void set(const size_t& idx, const ParameterType& parameter)
    {
        if(idx > G)
            return;

        if(parameter == m_Parameter.col(idx))
            return;

        m_Parameter.col(idx) = parameter;

        updateParameter();
    }

    ParameterType get(const size_t& idx) const
    {
        return m_Parameter.col(idx);
    }

    ParameterType get(const size_t& d, const T& alpha) const
    {
        ProcessType a_vec;
        a_vec.setZero();

        T ca = T(1);

        for(size_t i = 0; i <= G - d; i++)
        {
            a_vec[G - i - d] = ca;

            ca *= alpha;
        }

        T factor = T(1);

        for(size_t i = 1; i <= d; i++)
            factor *= (G - i + 1);

        return factor * m_DerivedParameter[d] * a_vec;
    }

    void updateParameter()
    {
        for(size_t i = 0; i <= G; i++)
        {
            if(i == 0)
                m_TmpParameterArray[0] = m_Parameter;
            else
                for(size_t j = 0; j <= G - i; j++)
                    m_TmpParameterArray[i].col(j) = m_TmpParameterArray[i - 1].col(j + 1) - m_TmpParameterArray[i - 1].col(j);
        }

        for(size_t i = 0; i <= G; i++)
            m_DerivedParameter[i] = (m_BernsteinMatrices.getBernsteinMatrix(i) * m_TmpParameterArray[i].transpose()).transpose();
    }

protected:
    ParameterEntryType m_Parameter;
    ParameterEntriesArray m_DerivedParameter;

private:
    ParameterEntriesArray m_TmpParameterArray;

private:
    static const BezierParameterTypeTraits<T> m_TypeTrait;
    static const BezierParameterGradeTraits<G> m_GradeTrait;

private:
    static const BernsteinMatrix<T, G> m_BernsteinMatrices;
};
