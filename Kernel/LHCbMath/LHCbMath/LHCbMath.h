/** @file
 *
 *  Collection of math related functions for general use in LHCb
 *
 *  @author Juan PALACIOS
 *  @date   2005-11-21
 */
// ============================================================================
#ifndef LHCBMATH_LHCBMATH_H
#define LHCBMATH_LHCBMATH_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <algorithm>
#include <array>
#include <cmath>
#include <functional>
#include <vector>
// ============================================================================
// Boost
// ============================================================================
#include "boost/call_traits.hpp"
// ========================================================================
/// Parameters for numerical calculations (M.Needham)
static const double hiTolerance = 1e-40;
static const double lowTolerance = 1e-20;
static const double looseTolerance = 1e-5;
static const double sqrt_12 = 3.4641016151377546;     // sqrt(12.)
static const double inv_sqrt_12 = 0.2886751345948129; // 1./sqrt(12.)
// ========================================================================
/** @var mULPS_float
 *  "tolerance" parameter for "Lomont"-compare of floating point numbers.
 *  It corresponds to relative ("Knuth/GLS") tolerance of about ~6*10^-6
 *  for values in excess of 10^-37.
 *
 *  @see lomont_compare_float
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2010-01-02
 */
const unsigned short mULPS_float = 100;
// ========================================================================
/** @var mULPS_float_low
 *  "Low-tolerance" parameter for "Lomont"-compare of floating point numbers.
 *  It corresponds to relative ("Knuth/GLS") tolerance of about ~6*10^-5
 *  for values in excess of 10^-37.
 *
 *  @see lomont_compare_float
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2010-01-02
 */
const unsigned short mULPS_float_low = 1000;
// =========================================================================
/** @var mULPS_double
 *  "tolerance" parameter for "Lomont"-compare of floating point numbers.
 *  It corresponds to relative ("Knuth/GLS") tolerance of about ~6*10^-13
 *  for values in excess of 10^-304.
 *  @see lomont_compare_double
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2010-01-02
 */
const unsigned int mULPS_double = 1000;
// ========================================================================
/** @struct abs_less
 *  comparison by absolute value
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2007-08-17
 */
template <class TYPE> struct abs_less : std::binary_function<TYPE, TYPE, TYPE> {
  inline TYPE
  operator()(typename boost::call_traits<const TYPE>::param_type v1,
             typename boost::call_traits<const TYPE>::param_type v2) const {
    return m_eval(std::fabs(v1), std::fabs(v2));
  }
  /// evaluator:
  std::less<TYPE> m_eval;
};
// ========================================================================
/** @struct abs_greater
 *  comparison by absolute value
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2007-08-17
 */
template <class TYPE>
struct abs_greater : std::binary_function<TYPE, TYPE, TYPE> {
  inline TYPE
  operator()(typename boost::call_traits<const TYPE>::param_type v1,
             typename boost::call_traits<const TYPE>::param_type v2) const {
    return m_eval(std::fabs(v1), std::fabs(v2));
  }
  /// evaluator:
  std::greater<TYPE> m_eval;
};
// ========================================================================
/** return "min_by_abs"
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2007-08-17
 */
template <class TYPE> inline TYPE absMin(TYPE v1, TYPE v2) {
  return std::min(std::fabs(v1), std::fabs(v2));
}
// ========================================================================
/** return  "max_by_abs"
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2007-08-17
 */
template <class TYPE> inline TYPE absMax(TYPE v1, TYPE v2) {
  return std::max(std::fabs(v1), std::fabs(v2));
}
// ========================================================================
/** compare two double numbers with relative precision 'epsilon'
 *
 *  Essentially it is a wrapper to gsl_fcmp function from GSL library
 *  See D.E.Knuth, "Seminumerical Algorithms", section 4.2.2
 *
 *  @param value1  (INPUT) the first value
 *  @param value2  (INPUT) the second value
 *  @param epsilon (INPUT) the (relative) precision
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2007-11-27
 */
bool knuth_equal_to_double(const double value1, const double value2,
                           const double epsilon = 1.0e-6);
// ========================================================================
// ========================================================================
/** @struct Equal_To
 *  helper structure for comparison of floating values
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2007-11-27
 */
template <class TYPE>
struct Equal_To : public std::binary_function<TYPE, TYPE, bool> {
  // ======================================================================
  typedef typename boost::call_traits<const TYPE>::param_type T;
  // ======================================================================
  /// comparison
  inline bool operator()(T v1, T v2) const {
    std::equal_to<TYPE> cmp;
    return cmp(v1, v2);
  }
  // ======================================================================
};
// ========================================================================
// ========================================================================
template <class TYPE> struct Zero;
template <class TYPE> struct NonZero;
// ========================================================================
/** @struct Zero
 *  helper structure for comparison of floating values
 *  @author Vanya BELYAEV Ivan.Belyaev@iep.ru
 *  @date 2007-11-27
 */
template <class TYPE> struct Zero : public std::unary_function<TYPE, bool> {
  // ======================================================================
  typedef typename boost::call_traits<const TYPE>::param_type T;
  /// comparison
  inline bool operator()(T v) const { return m_cmp(v, 0); }
  // ======================================================================
private:
  // ======================================================================
  // the comparizon criteria
  Equal_To<TYPE> m_cmp;
  // ======================================================================
};
// ========================================================================
template <> struct Zero<double> : public std::unary_function<double, bool> {
  // ======================================================================
  /// comparison
  inline bool operator()(const double v) const { return !v || m_cmp(v, 0); }
  // ======================================================================
private:
  // ======================================================================
  // the comparizon criteria
  Equal_To<double> m_cmp;
  // ======================================================================
};
// ========================================================================
template <> struct Zero<float> : public std::unary_function<float, bool> {
  // ======================================================================
  /// comparison
  inline bool operator()(const float v) const { return !v || m_cmp(v, 0); }
  // ======================================================================
private:
  // ======================================================================
  // the comparizon criteria
  Equal_To<float> m_cmp;
  // ======================================================================
};
// ========================================================================
/// partial specialization for const-types
template <class TYPE> struct Zero<const TYPE> : public Zero<TYPE> {};
// ========================================================================
/// partial specialization for references
template <class TYPE> struct Zero<TYPE &> : public Zero<TYPE> {};
// ========================================================================
/** @struct NotZero
 *  helper structure for comparison of floating values
 *  @author Vanya BELYAEV Ivan.Belyaev@iep.ru
 *  @date 2007-11-27
 */
template <class TYPE> struct NotZero : public std::unary_function<TYPE, bool> {
  // ======================================================================
  typedef typename boost::call_traits<const TYPE>::param_type T;
  /// comparison
  inline bool operator()(T v) const { return !m_zero(v); }
  // ======================================================================
private:
  // ======================================================================
  // the comparison criteria
  Zero<TYPE> m_zero;
  // ======================================================================
};
/// partial specialization for const-types
template <class TYPE> struct NotZero<const TYPE> : public NotZero<TYPE> {};
// ========================================================================
/// partial specialization for references
template <class TYPE> struct NotZero<TYPE &> : public NotZero<TYPE> {};
// ========================================================================
/** specialisation for vectors
 *  @see Zero
 *  @see Equal_To
 *  @see Lomont<float>
 */
template <class TYPE> struct Zero<std::vector<TYPE>> {
public:
  // ======================================================================
  inline bool operator()(const std::vector<TYPE> &v) const {
    /// empty vector or all elements are zeros
    return v.empty() || (v.end() == std::find_if(v.begin(), v.end(), m_nz));
  }
  // ======================================================================
private:
  // ======================================================================
  // comparison criteria for elements
  NotZero<TYPE> m_nz;
  // ======================================================================
};
// ========================================================================
/// Is value sufficiently  small ?
template <class TYPE> struct Small : public std::unary_function<TYPE, bool> {
  // ======================================================================
  typedef TYPE Inner;
  // ======================================================================
  // constructor with threshold
  Small(const TYPE &a) : m_a(std::abs(a)) {}
  // the opnly one important method
  inline bool operator()(const TYPE &a) const { return std::abs(a) <= m_a; }
  // ======================================================================
private:
  // ======================================================================
  /// default constructor is disabled
  Small(); // default constructor is disabled
  // ======================================================================
private:
  // ======================================================================
  TYPE m_a;
  // ======================================================================
};

#endif // LHCBMATH_LHCBMATH_H
