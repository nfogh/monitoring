#pragma once
#include "IPubSub.hpp"
#include <cassert>
#include <cstring>
#include <unordered_map>

#include <iostream>

namespace PubSubMonitoring {
struct PubSub : public IPubSub
{
  void Publish(const IMessage &message) override
  {
    mMessages[message.GetName()].resize(static_cast<size_t>(message.GetSize()));
    ::memcpy(mMessages[message.GetName()].data(), message.GetData(), static_cast<size_t>(message.GetSize()));
    for (auto &subscription : mSubscriptions) {
      if (subscription.message->GetName() == message.GetName()) {
        assert(subscription.message->GetSize() == message.GetSize());
        ::memcpy(
          subscription.message->GetData(), message.GetData(), static_cast<size_t>(subscription.message->GetSize()));
        subscription.handler();
      }
    }
  };

  void Subscribe(IMessage &message, std::function<void()> handler) override
  {
    auto messageIt = mMessages.find(message.GetName());
    if (messageIt != mMessages.end()) {
      ::memcpy(message.GetData(), messageIt->second.data(), static_cast<size_t>(message.GetSize()));
    }
    mSubscriptions.push_back({ &message, handler });
  };

  void Unsubscribe(const IMessage &message) override
  {
    auto it = std::find_if(mSubscriptions.begin(), mSubscriptions.end(), [&](const auto &subscription) {
      return subscription.message == &message;
    });
    if (it != mSubscriptions.end()) { mSubscriptions.erase(it); }
  };

private:
  struct Subscription
  {
    IMessage *message;
    std::function<void()> handler;
  };

  std::vector<Subscription> mSubscriptions;
  std::unordered_map<std::string, std::vector<unsigned char>> mMessages;
};
}// namespace PubSubMonitoring
