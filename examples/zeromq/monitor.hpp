#pragma once

#include <functional>
#include <tuple>

#include "IMessage.hpp"

namespace PubSubMonitoring {
struct IPubSub
{
  virtual ~IPubSub() = default;
  virtual void Publish(const IMessage &message) = 0;
  virtual void Subscribe(const IMessage &message, std::function<void()> handler) = 0;
  virtual void Unsubscribe(const IMessage &message) = 0;
};

template<typename MonitorT, typename... MessageT> struct PubSubMonitor
{
  PubSubMonitor(IPubSub &pubsub, MonitorT monitor, Handlers &&...handlers) : mPubSub(pubsub), mMonitor(std::move(monitor))
  {
    std::apply([&](auto &arg) { subscriptions.push_back(subscriber.Subscribe(arg, [&] { Check(); })); }, mMessages);
  }

private:
  void Check() { mMonitor(); }

  std::reference_wrapper<IPubSub> mPubSub;
  std::vector<SubscriptionHandle> mSubscriptions;
  MonitorT mMonitor;
  std::tuple<MessageT...> mMessages;
};

}// namespace PubSubMonitoring
