template <unsigned int N>
struct StaticFactorial
{
    static_assert(N < 13, "12 Is max for StaticFac");

    const unsigned int& getValue() const { return m_Value; }
    static const unsigned int m_Value = N * StaticFactorial<N - 1>::m_Value;
};

template <>
struct StaticFactorial<0>
{
    static const unsigned int m_Value = 1;
};
