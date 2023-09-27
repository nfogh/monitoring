#pragma once

#include "PubSub/IMessage.hpp"
#include "PubSub/IPubSub.hpp"
#include <functional>
#include <monitoring/monitor.hpp>
#include <tuple>

namespace PubSubMonitoring {
namespace intern {
  template<typename TupleT, typename Fn> void for_each_tuple(Fn &&fn, TupleT &&tp)
  {
    std::apply([&fn](auto &&...args) { (fn(std::forward<decltype(args)>(args)), ...); }, std::forward<TupleT>(tp));
  }
}// namespace intern

struct SubscribeAndCheckFunc
{
  SubscribeAndCheckFunc(IPubSub &pubsub) : mPubSub(pubsub) {}

  void SetCheckFunc(std::function<void(void)> checkFunc) { mCheckFunc = std::move(checkFunc); }

  template<typename Message> void operator()(Message &&message)
  {
    mPubSub.Subscribe(message, [&] { mCheckFunc(); });
  }

private:
  IPubSub &mPubSub;
  std::function<void(void)> mCheckFunc;
};

template<typename MessagesT, typename MonitorT> struct PubSubMonitor
{
  PubSubMonitor(IPubSub &pubsub, MessagesT messages, MonitorT monitor)
    : mPubSub(pubsub), mMessages(std::move(messages)), mMonitor(std::move(monitor))
  {
    mMonitor.GetOpt().SetCheckFunc([&] { Check(); });
    intern::for_each_tuple([&](auto &arg) { mPubSub.Subscribe(arg, [&] { Check(); }); }, mMessages);
    Check();
  }

  ~PubSubMonitor()
  {
    intern::for_each_tuple([&](auto &arg) { mPubSub.Unsubscribe(arg); }, mMessages);
  }

private:
  void Check()
  {
    std::apply([&](auto &&...args) { mMonitor(std::forward<decltype(args)>(args)...); }, mMessages);
  }

  IPubSub &mPubSub;
  MessagesT mMessages;
  MonitorT mMonitor;
};

template<typename MessagesT, typename RequirementsT> struct MonitorWithRequirements
{
  MonitorWithRequirements(MessagesT messages, IPubSub &pubsub, RequirementsT requirements)
    : mMessages(std::move(messages)), mPubSub(pubsub), mRequirements(std::move(requirements))
  {}

  template<typename... HandlersT> [[nodiscard]] auto Handler(HandlersT &&...handlers)
  {
    auto mergedHandlers = [handlers = std::forward_as_tuple(handlers...)](bool result) {
      std::apply([result](auto &&...handlers) { (..., handlers(result)); }, handlers);
    };
    auto subscribeAndCheckFunc = SubscribeAndCheckFunc(mPubSub);
    return PubSubMonitor(mPubSub, mMessages, Monitoring::Monitor(mRequirements, mergedHandlers, subscribeAndCheckFunc));
  }

private:
  MessagesT mMessages;
  IPubSub &mPubSub;
  RequirementsT mRequirements;
};

template<typename MessagesT> struct MonitorWith
{
  MonitorWith(MessagesT messages, IPubSub &pubsub) : mMessages(std::move(messages)), mPubSub(pubsub) {}

  template<typename RequirementsT> [[nodiscard]] auto Require(RequirementsT &&requirements)
  {
    return MonitorWithRequirements(mMessages, mPubSub, std::forward<RequirementsT>(requirements));
  }

private:
  MessagesT mMessages;
  IPubSub &mPubSub;
};


template<typename... MessageT> struct Monitor
{
  Monitor(MessageT... messages) : mMessages(messages...) {}

  [[nodiscard]] auto With(IPubSub &pubsub) { return MonitorWith(mMessages, pubsub); }

private:
  std::tuple<MessageT...> mMessages;
};

}// namespace PubSubMonitoring
