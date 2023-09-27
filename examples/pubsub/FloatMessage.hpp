#pragma once
#include "PubSub/IMessage.hpp"
namespace PubSubMonitoring {
struct FloatMessage : public IMessage
{
  explicit FloatMessage(std::string name = "FloatMessage") : mName(std::move(name)) {}
  FloatMessage(float f = 0.0f, std::string name = "FloatMessage") : mData{ f }, mName(std::move(name)) {}
  const void *GetData() const override { return &mData; }
  void *GetData() override { return &mData; }
  int GetSize() const override { return sizeof(mData); }
  const std::string &GetName() const override { return mName; }

  float GetF() const { return mData.f; }
  void SetF(float f) { mData.f = f; }

  bool operator>=(const FloatMessage &other) const { return mData.f >= other.mData.f; }
  bool operator<=(const FloatMessage &other) const { return mData.f <= other.mData.f; }
  bool operator>(const FloatMessage &other) const { return mData.f > other.mData.f; }
  bool operator<(const FloatMessage &other) const { return mData.f < other.mData.f; }
  bool operator==(const FloatMessage &other) const { return mData.f == other.mData.f; }
  bool operator!=(const FloatMessage &other) const { return mData.f != other.mData.f; }

private:
  struct Data
  {
    float f = 0.0f;
  } mData;

  const std::string mName = "FloatMessage";
};
}// namespace PubSubMonitoring