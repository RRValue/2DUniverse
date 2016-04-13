#ifndef _BASE_STATICPOLYNOMIALSOLVER_H_
#define _BASE_STATICPOLYNOMIALSOLVER_H_

#include <array>
#include <initializer_list>
#include <cassert>

// define solver traits
template<typename T>
struct StaticPolynomialSolverTypeTrait
{
    StaticPolynomialSolverTypeTrait() { static_assert(false, "Type of StaticPolynomialSolver not supported") }
};

template<>
struct StaticPolynomialSolverTypeTrait<float>
{
    StaticPolynomialSolverTypeTrait() {}
};

template<>
struct StaticPolynomialSolverTypeTrait<double>
{
    StaticPolynomialSolverTypeTrait() {}
};

#define TypeRef(T) T&
#define VoidToType(T, V) *(T*)V
#define TypeToVoid(V) (void*)&V
#define TypePointerToVoid(V) (void*)V

// define solver
template <typename T, unsigned int N>
class StaticPolynomialSolver : protected StaticPolynomialSolverTypeTrait<T>
{
    static_assert(N >= 1 && N <= 4, "Grade must between 1 and 4");

private:
    template<typename T, unsigned int N>
    struct PolynomialSolveResult : public std::array<T, N>
    {
        PolynomialSolveResult()
        {
            m_Solutions = 0;
        }

        unsigned int m_Solutions;
    };

public:
    typedef PolynomialSolveResult<T, N> Result;

public:
    template<unsigned int M>
    class CoefArray : public std::array<void*, M>
    {
        typedef std::initializer_list<T> TypeInitList;
        typedef std::initializer_list<T* const> TypePointerInitList;

    public:
        CoefArray() = delete;
        CoefArray(const TypeInitList& list)
        {
            assert(list.size() == M && "Coef list size not correct");

            size_t i = 0;
            for(const auto& e : list)
                (*this)[i++] = TypeToVoid(e);
        }

        CoefArray(const TypePointerInitList& list)
        {
            assert(list.size() == M && "Coef list size not correct");

            size_t i = 0;
            for(const auto& e : list)
                (*this)[i++] = TypePointerToVoid(e);
        }

        CoefArray(const std::array<T, M>& array)
        {
            size_t i = 0;
            for(const auto& e : array)
                (*this)[i++] = TypeToVoid(e);
        }
    };

public:
    Result solve(const CoefArray<N + 1>& coef) const
    {
        return solveImpl(coef);
    }

private:
    template<unsigned int M>
    Result solveImpl(const CoefArray<M>& coef) const
    {
        static_assert(false, "Solve impl. not defined");
    }

    template<>
    Result solveImpl<2>(const CoefArray<2>& coef) const
    {
        // solve: c0 * (1 - a) + c1 == 0
        // a = c0 / (c0 - c1)

        // (c0 - c1) must be greater than epslion (1e-5f)
        // otherwise -> no root

        Result res;

        TypeRef(T) c0 = VoidToType(T, coef[0]);
        TypeRef(T) c1 = VoidToType(T, coef[1]);

        T a = c0 - c1;

        if(std::abs(a) < 1e-5f)
            return res;

        res[0] = c0 / a;
        res.m_Solutions = 1;

        return res;
    }

    template<>
    Result solveImpl<3>(const CoefArray<3>& coef) const
    {
        Result result;

        return result;
    }

    template<>
    Result solveImpl<4>(const CoefArray<4>& coef) const
    {
        Result result;

        return result;
    }
};

#endif //_BASE_STATICPOLYNOMIALSOLVER_H_
