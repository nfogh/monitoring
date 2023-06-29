#pragma once

#include "condition.hpp"
#include "intern.hpp"

namespace Monitoring {

template<typename GetterT, typename T, typename CompT> struct Comparison
{
  Comparison(GetterT getter, T otherVal, CompT comp)
    : mGetter(std::move(getter)), mOtherVal(std::move(otherVal)), mComp(std::move(comp))
  {}

  template<typename ValT> bool check(const ValT &val) const { return mComp(mOtherVal, mGetter(val)); }

  LIBMONITORING_DEFINE_LOGIC_OPERATORS;

private:
  GetterT mGetter;
  T mOtherVal;
  CompT mComp;
};

template<typename T, typename Getter> auto Max(Getter getter, T max)
{
  return Comparison(std::move(getter), std::move(max), std::greater_equal<T>());
}

template<typename T> auto Max(T max) { return Max(Intern::Identity(), std::move(max)); }

template<typename T, typename Getter> auto Min(Getter getter, T min)
{
  return Comparison(std::move(getter), std::move(min), std::less_equal<T>());
}

template<typename T> auto Min(T min) { return Min(Intern::Identity(), std::move(min)); }

template<typename T, typename Getter = Intern::Identity> struct MinHyst
{
  MinHyst(T min, T hyst, Getter getter = Intern::Identity())
    : mGetter(std::move(getter)), mMin(std::move(min)), mHyst(std::move(hyst))
  {}

  template<typename ValT> bool check(const ValT &val)
  {
    if (mInHyst) {
      mInHyst = mGetter(val) < mMin + mHyst;
    } else {
      mInHyst = mGetter(val) < mMin;
    }
    return !mInHyst;
  }

  LIBMONITORING_DEFINE_LOGIC_OPERATORS;

private:
  Getter mGetter;
  T mMin;
  T mHyst;
  bool mInHyst = false;
};

template<typename T, typename Getter = Intern::Identity> struct MaxHyst
{
  MaxHyst(T max, T hyst, Getter getter = Intern::Identity())
    : mGetter(std::move(getter)), mMax(std::move(max)), mHyst(std::move(hyst))
  {}

  template<typename ValT> bool check(const ValT &val)
  {
    if (mInHyst) {
      mInHyst = mGetter(val) > mMax - mHyst;
    } else {
      mInHyst = mGetter(val) > mMax;
    }
    return !mInHyst;
  }

  LIBMONITORING_DEFINE_LOGIC_OPERATORS;

private:
  Getter mGetter;
  T mMax;
  T mHyst;
  bool mInHyst = false;
};

}// namespace Monitoring
