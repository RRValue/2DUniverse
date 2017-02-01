#ifndef _BASE_POLYNOMIALSOLVER_H_
#define _BASE_POLYNOMIALSOLVER_H_

#include <array>
#include <initializer_list>
#include <cassert>

// define solver traits
template<typename T>
struct PolynomialSolverTypeTrait
{
    PolynomialSolverTypeTrait() { static_assert(false, "Type of PolynomialSolver not supported") }
};

template<>
struct PolynomialSolverTypeTrait<float>
{
    PolynomialSolverTypeTrait() {}
};

template<>
struct PolynomialSolverTypeTrait<double>
{
    PolynomialSolverTypeTrait() {}
};

#define ConstTypeRef(T) const T&

// define solver
template <typename T, unsigned int G>
class PolynomialSolver : protected PolynomialSolverTypeTrait<T>
{
    static_assert(G >= 1, "Grade must be greater than 1");

private:
    template<typename T, unsigned int G>
    struct PolynomialSolveResult : public std::array<T, G>
    {
        PolynomialSolveResult()
        {
            m_Solutions = 0;
        }

        unsigned int m_Solutions;
    };

public:
    typedef PolynomialSolveResult<T, G> Result;

private:
    typedef Eigen::Matrix<T, G + 1, 1> CoefVec;

public:
    Result solve(const CoefVec& coef) const
    {
        return solveImpl<G>(coef);
    }

private:
    template<unsigned int M>
    Result solveImpl(const CoefVec& coef) const
    {
        static_assert(false, "Solve impl. not defined");
    }

    template<>
    Result solveImpl<1>(const CoefVec& coef) const
    {
        // solve: a*x + b == 0
        // a = -b / a

        // a must be greater than epslion (1e-5f)
        // otherwise -> no root

        Result res;

        ConstTypeRef(T) a = coef(0);
        ConstTypeRef(T) b = coef(1);

        if(std::abs(a) < 1e-5f)
            return res;

        res[0] = -b / a;
        res.m_Solutions = 1;

        return res;
    }

    template<>
    Result solveImpl<2>(const CoefVec& coef) const
    {
        Result result;

        if(std::abs(coef[0]) < T(1e-10))
        {
            CoefVec new_vec;
            new_vec.setZero();

            for(size_t i = 1; i < G; i++)
                new_vec(i - 1) = coef(i);
            
            return solveImpl<1>(new_vec);
        }

        T a = coef[1] / coef[0];
        T b = coef[2] / coef[0];

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
    Result solveImpl<3>(const CoefVec& coef) const
    {
        // algorithmic source: http://www.1728.org/cubic2.htm

        Result result;
        
        // solce cubic
        const T& a = coef[0];
        const T& b = coef[1];
        const T& c = coef[2];
        const T& d = coef[3];

        if(std::abs(coef[0]) < T(1e-10))
        {
            CoefVec new_vec;
            new_vec.setZero();

            for(size_t i = 0; i < G; i++)
                new_vec(i) = coef(i + 1);

            return solveImpl<2>(new_vec);
        }

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
};

#endif //_BASE_POLYNOMILASOLVER_H_