#pragma once
#include "IPubSub.hpp"
#include <cstring>
#include <cassert>

#include <iostream>

namespace PubSubMonitoring {
struct PubSub : public IPubSub
{
  void Publish(const IMessage &message) override
  {
    for (auto &subscription : mSubscriptions)
    {
      if (subscription.message->GetName() == message.GetName())
      {
        assert(subscription.message->GetSize() == message.GetSize());
        ::memcpy(subscription.message->GetData(), message.GetData(), static_cast<size_t>(subscription.message->GetSize()));
        subscription.handler();
      }
    }
  };

  void Subscribe(IMessage &message, std::function<void()> handler) override
  {
    mSubscriptions.push_back({&message, handler});
  };

  void Unsubscribe(const IMessage &message) override
  {
    auto it = std::find_if(mSubscriptions.begin(), mSubscriptions.end(), [&](const auto &subscription) {
      return subscription.message == &message;
    });
    if (it != mSubscriptions.end())
    {
      mSubscriptions.erase(it);
    }
  };

private:
    struct Subscription
    {
        IMessage* message;
        std::function<void()> handler;
    };

    std::vector<Subscription> mSubscriptions;
};
}
