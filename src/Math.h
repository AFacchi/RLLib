/*
 * Math.h
 *
 *  Created on: Sep 3, 2012
 *      Author: sam
 */

#ifndef MATH_H_
#define MATH_H_

#include <cmath>
#include <limits>

namespace RLLib
{

// Some number checking

class Boundedness
{
  public:
    template<class T>
    inline static bool checkValue(const T& value)
    {
      return !isnan(value) && !isinf(value);
    }
};

// Helper class for range management for testing environments
template<class T>
class Range
{
  private:
    T minv, maxv;

  public:
    Range(const T& minv = std::numeric_limits<T>::min(), const T& maxv =
        std::numeric_limits<T>::max()) :
        minv(minv), maxv(maxv)
    {
    }

    T bound(const T& value) const
    {
      return std::max(minv, std::min(maxv, value));
    }

    bool in(const T& value) const
    {
      return value >= minv && value <= maxv;
    }

    T length() const
    {
      return maxv - minv;
    }

    T min() const
    {
      return minv;
    }

    T max() const
    {
      return maxv;
    }

    T center() const
    {
      return min() + (length() / 2.0);
    }
};

class Signum
{
  public:
    template<typename T>
    inline static int valueOf(const T& val)
    {
      return (T(0) < val) - (val < T(0));
    }
};

// Important distributions
class Random
{
  public:

    // [0..1]
    inline static float randomFloat()
    {
      return float(rand()) * (1.f / static_cast<float>(RAND_MAX));
    }

    // [0..1]
    inline static double randomDouble()
    {
      return double(rand()) / RAND_MAX;
    }

    // A gaussian random deviate
    inline static double nextNormalGaussian()
    {
      double r, v1, v2;
      do
      {
        v1 = 2.0 * randomDouble() - 1.0;
        v2 = 2.0 * randomDouble() - 1.0;
        r = v1 * v1 + v2 * v2;
      } while (r >= 1.0 || r == 0);
      const double fac(sqrt(-2.0 * log(r) / r));
      return v1 * fac;
    }

    inline static double gaussianProbability(const float& x, const float& m,
        const float& s)
    {
      return exp(-0.5 * pow((x - m) / s, 2)) / (s * sqrt(2.0 * M_PI));
    }

    // http://en.literateprograms.org/Box-Muller_transform_(C)
    inline static double nextGaussian(const double& mean, const double& stddev)
    {
      static double n2 = 0.0;
      static int n2_cached = 0;
      if (!n2_cached)
      {
        double x, y, r;
        do
        {
          x = drand48() - 1;
          y = drand48() - 1;

          r = x * x + y * y;
        } while (r == 0.0 || r > 1.0);
        {
          double d = sqrt(-2.0 * log(r) / r);
          double n1 = x * d;
          n2 = y * d;
          double result = n1 * stddev + mean;
          n2_cached = 1;
          return result;
        }
      }
      else
      {
        n2_cached = 0;
        return n2 * stddev + mean;
      }
    }

    inline float sampleNormalDistribution(float b)
    {
      float result(0.0f);
      for (int i = 0; i < 12; i++)
        result += 2.0f * ((randomFloat() - 0.5f) * b);
      return result / 2.0f;
    }

    inline float sampleTriangularDistribution(float b)
    {
      float randResult = 2.0f * ((randomFloat() - 0.5f) * b)
          + 2.0f * ((randomFloat() - 0.5f) * b);
      return (sqrt(6.0f) / 2.0f) * randResult;
    }
};

} // namespace RLLib

#endif /* MATH_H_ */
