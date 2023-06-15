#pragma once
#include "IMessage.hpp"
#include <functional>

namespace PubSubMonitoring {
struct IPubSub
{
  virtual ~IPubSub() = default;
  virtual void Publish(const IMessage &message) = 0;
  virtual void Subscribe(IMessage &message, std::function<void()> handler) = 0;
  virtual void Unsubscribe(const IMessage &message) = 0;
};
}
