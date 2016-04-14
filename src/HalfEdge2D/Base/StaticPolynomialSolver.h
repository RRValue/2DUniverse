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
        TypeRef(T) c0 = VoidToType(T, coef[0]);
        TypeRef(T) c1 = VoidToType(T, coef[1]);
        TypeRef(T) c2 = VoidToType(T, coef[2]);

        static Eigen::Matrix<T, 3, 3> coef_mat3 = []
        {
            Eigen::Matrix<T, 3, 3> m;
            m <<
                T( 1), T(-2), T(1),
                T(-2), T( 2), T(0),
                T( 1), T( 0), T(0);
            
            return m;
        }();

        Result result;

        // transform coeficent
        Eigen::Matrix<T, 3, 1> t_coef = coef_mat3 * Eigen::Matrix<T, 3, 1>(c0, c1, c2);

        // solve quadratic param
        T a = t_coef[1] / t_coef[0];
        T b = t_coef[2] / t_coef[0];

        T det = (a * a) - (T(4) * b);
        T t = -a / T(2);

        if(det < 0.0f)
            return result;
        else if(det < T(1e-5f))
        {
            result.m_Solutions = 1;
            result[0] = t;

            return result;
        }

        det = std::sqrt(det) / T(2);

        result.m_Solutions = 2;
        result[0] = t - det;
        result[1] = t + det;

        return result;
    }

    template<>
    Result solveImpl<4>(const CoefArray<4>& coef) const
    {
        TypeRef(T) c0 = VoidToType(T, coef[0]);
        TypeRef(T) c1 = VoidToType(T, coef[1]);
        TypeRef(T) c2 = VoidToType(T, coef[2]);
        TypeRef(T) c3 = VoidToType(T, coef[3]);

        static Eigen::Matrix<T, 4, 4> coef_mat = []
        {
            Eigen::Matrix<T, 4, 4> m;
            m <<
                T(-1), T( 3), T(-3), T( 1),
                T( 3), T(-6), T( 3), T( 0),
                T(-3), T( 3), T( 0), T( 0),
                T( 1), T( 0), T( 0), T( 0);

            return m;
        }();

        Result result;

        // transform coeficent
        Eigen::Matrix<T, 4, 1> t_coef = coef_mat * Eigen::Matrix<T, 4, 1>(c0, c1, c2, c3);

        // solce cubic
        const T& a = t_coef[0];
        const T& b = t_coef[1];
        const T& c = t_coef[2];
        const T& d = t_coef[3];

        T a2 = a * a;
        T a3 = a2 * a;
        T b2 = b * b;
        T b3 = b2 * b;

        T f = ((T(3) * c / a) - (b2 / a2)) / T(3);
        T g = ((T(2) * b3 / a3) - (T(9) * b * c / a2) + (T(27) * d / a)) / T(27);

        T f3 = f * f * f;
        T g2 = g * g;

        T h = (g2 / T(4)) + (f3 / T(27));

        // check for one solution (f == g == h)
        if(std::abs(f) < T(1e-10) && std::abs(g) < T(1e-10) && std::abs(h) < T(1e-10))
        {
            result.m_Solutions = 1;

            result[0] = -std::cbrt(d / a);

            return result;
        }
        else if(h > T(0))
        {
            T h_sqrt = std::sqrt(h);
            T g_half = g / T(2);

            T s = std::cbrt(-g_half + h_sqrt);
            T u = std::cbrt(-g_half - h_sqrt);

            result.m_Solutions = 1;

            result[0] = (s + u) - (b / (T(3) * a));

            return result;
        }

        T i = std::sqrt((g2 / T(4)) - h);
        T j = std::cbrt(i);
        T k = std::acos(-g / (T(2) * i));

        T k_third = k / T(3);

        T m = std::cos(k_third);
        T n = std::sqrt(T(3)) * std::sin(k_third);
        T p = -b / (T(3) * a);

        if(std::abs(n) < T(1e-10))
        {
            result.m_Solutions = 2;

            result[0] = (-j * m) + p;
            result[1] = 2 * j * m + p;
        }

        result.m_Solutions = 3;

        result[0] = (-j * (m + n)) + p;
        result[1] = (-j * (m - n)) + p;
        result[2] = 2 * j * m + p;

        return result;
    }

private:

};

#endif //_BASE_STATICPOLYNOMIALSOLVER_H_
