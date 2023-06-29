#pragma once
#include "PubSub/IMessage.hpp"
namespace PubSubMonitoring {
struct IntMessage : public IMessage
{
  explicit IntMessage(std::string name = "IntMessage") : mName(std::move(name)) {}
  explicit IntMessage(int i, std::string name = "IntMessage") : mData{ i }, mName(std::move(name)) {}
  const void *GetData() const override { return &mData; }
  void *GetData() override { return &mData; }
  int GetSize() const override { return sizeof(mData); }

  int GetI() const { return mData.i; }
  void SetI(int i) { mData.i = i; }

  const std::string &GetName() const override { return mName; }

private:
  struct Data
  {
    int i = 0;
  } mData;

  std::string mName = "IntMessage";
};
}// namespace PubSubMonitoring