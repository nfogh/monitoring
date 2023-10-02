#pragma once

#include "PubSub/IMessage.hpp"
#include "PubSub/IPubSub.hpp"
#include <functional>
#include <monitoring/monitor.hpp>
#include <tuple>

namespace PubSubMonitoring {
namespace Intern {
  template<typename T, typename FuncT, int... Is> void for_each(FuncT func, T &&arg, std::integer_sequence<int, Is...>)
  {
    auto unused{ (func(std::get<Is>(arg)), 0)... };
    (void)unused;
  }

  template<typename FuncT, typename... Ts> void for_each_in_tuple(FuncT func, std::tuple<Ts...> &args)
  {
    for_each(func, args, std::make_integer_sequence<int, sizeof...(Ts)>());
  }

  template<typename FuncT, typename... Ts> void for_each_in_tuple(FuncT func, const std::tuple<Ts...> &args)
  {
    for_each(func, args, std::make_integer_sequence<int, sizeof...(Ts)>());
  }
}// namespace Intern

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

template<typename MessagesT, typename MonitorT> struct PubSubMonitorT
{
  PubSubMonitorT(IPubSub &pubsub, MessagesT messages, MonitorT monitor)
    : mPubSub(pubsub), mMessages(std::move(messages)), mMonitor(std::move(monitor))
  {
    mMonitor.GetOpt().SetCheckFunc([&] { Check(); });
    Intern::for_each_in_tuple([&](auto &arg) { mPubSub.Subscribe(arg, [&] { Check(); }); }, mMessages);
    Check();
  }

  ~PubSubMonitorT()
  {
    Intern::for_each_in_tuple([&](auto &arg) { mPubSub.Unsubscribe(arg); }, mMessages);
  }

private:
  void Check()
  {
    Monitoring::Intern::apply([&](auto &&...args) { mMonitor(std::forward<decltype(args)>(args)...); }, mMessages);
  }

  IPubSub &mPubSub;
  MessagesT mMessages;
  MonitorT mMonitor;
};

template<typename MessagesT, typename MonitorT>
auto PubSubMonitor(IPubSub &pubsub, MessagesT messages, MonitorT monitor)
{
  return PubSubMonitorT<MessagesT, MonitorT>(pubsub, std::move(messages), std::move(monitor));
}

template<typename MessagesT, typename RequirementsT> struct MonitorWithRequirementsT
{
  MonitorWithRequirementsT(MessagesT messages, IPubSub &pubsub, RequirementsT requirements)
    : mMessages(std::move(messages)), mPubSub(pubsub), mRequirements(std::move(requirements))
  {}

  template<typename... HandlersT> /*[[nodiscard]]*/ auto Handler(HandlersT &&...handlers)
  {
    auto mergedHandlers = [handlers = std::forward_as_tuple(handlers...)](bool result) {
      Intern::for_each_in_tuple([&](auto &handler) { handler(result); }, handlers);
    };
    auto subscribeAndCheckFunc = SubscribeAndCheckFunc(mPubSub);
    return PubSubMonitor(mPubSub, mMessages, Monitoring::Monitor(mRequirements, mergedHandlers, subscribeAndCheckFunc));
  }

private:
  MessagesT mMessages;
  IPubSub &mPubSub;
  RequirementsT mRequirements;
};

template<typename MessagesT, typename RequirementsT>
auto MonitorWithRequirements(MessagesT messages, IPubSub &pubsub, RequirementsT requirements)
{
  return MonitorWithRequirementsT<MessagesT, RequirementsT>(std::move(messages), pubsub, std::move(requirements));
}

template<typename MessagesT> struct MonitorWithT
{
  MonitorWithT(MessagesT messages, IPubSub &pubsub) : mMessages(std::move(messages)), mPubSub(pubsub) {}

  template<typename RequirementsT> /*[[nodiscard]]*/ auto Require(RequirementsT &&requirements)
  {
    return MonitorWithRequirements(mMessages, mPubSub, std::forward<RequirementsT>(requirements));
  }

private:
  MessagesT mMessages;
  IPubSub &mPubSub;
};

template<typename MessagesT> auto MonitorWith(MessagesT messages, IPubSub &pubsub)
{
  return MonitorWithT<MessagesT>(std::move(messages), pubsub);
}

template<typename... MessageT> struct MonitorT
{
  MonitorT(MessageT... messages) : mMessages(std::move(messages)...) {}

  /*[[nodiscard]]*/ auto With(IPubSub &pubsub) { return MonitorWith(mMessages, pubsub); }

private:
  std::tuple<MessageT...> mMessages;
};

template<typename... MessageT> auto Monitor(MessageT &&...messages)
{
  return MonitorT<MessageT...>(std::forward<MessageT>(messages)...);
}

}// namespace PubSubMonitoring
