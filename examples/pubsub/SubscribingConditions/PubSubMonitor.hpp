#pragma once

#include "PubSub/IMessage.hpp"
#include "PubSub/IPubSub.hpp"
#include <functional>
#include <tuple>

namespace PubSubMonitoring {
namespace intern {
  template<typename TupleT, typename Fn> void for_each_tuple(Fn &&fn, TupleT &&tp)
  {
    std::apply([&fn](auto &&...args) { (fn(std::forward<decltype(args)>(args)), ...); }, std::forward<TupleT>(tp));
  }

  template<typename> struct is_subscribing : std::false_type
  {
  };

  template<typename T, typename T::is_subscribing_t = true> struct is_subscribing : std::true_type
  {
  };

  template<typename CallableT,
    typename CallbackT,
    typename TupleT,
    std::size_t...,
    std::enable_if_t<!is_subscribing<CallableT>::value, bool> = true>
  auto myapply(CallableT callable, CallbackT callback, TupleT tpl, std::index_sequence<I...>)
  {
    return callable(callback, std::get<I>(tpl)...);
  }

  template<typename CallableT,
    typename CallbackT,
    typename TupleT,
    std::size_t... I,
    std::enable_if_t<is_subscribing<CallableT>::value, bool> = true>
  auto myapply(CallableT callable, CallbackT /*callback*/, TupleT tpl, std::index_sequence<I...>)
  {
    return callable(std::get<I>(tpl)...);
  }

  template<typename CallableT, typename CallbackT, typename TupleT>
  auto myapply(CallableT callable, CallbackT callback, TupleT tpl)
  {
    static constexpr auto size = std::tuple_size<TupleT>::value;
    return apply(callable, callback, tpl, std::make_index_sequence<size>{});
  }
}// namespace intern

template<typename MessagesTpl, typename MonitorT> struct PubSubMonitor
{
  PubSubMonitor(IPubSub &pubsub, MessagesTpl messages, MonitorT monitor)
    : mPubSub(pubsub), mMessages(std::move(messages)), mMonitor(std::move(monitor))
  {
    intern::for_each_tuple([&](auto &arg) { mPubSub.Subscribe(arg, [&] { Check(); }); }, mMessages);
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
  MessagesTpl mMessages;
  MonitorT mMonitor;
};

template<typename MessagesT, typename MonitorT> struct MonitorRequirementsHandlers
{
  MonitorRequirementsHandlers(MessagesT messages, MonitorT monitor)
    : mMessages(std::move(messages)), mMonitor(std::move(monitor))
  {}

  [[nodiscard]] auto With(IPubSub &pubsub) { return PubSubMonitor(pubsub, mMessages, mMonitor); }

private:
  MessagesT mMessages;
  MonitorT mMonitor;
};

template<typename MessagesT, typename ConditionsT> struct MonitorRequirements
{
  MonitorRequirements(MessagesT messages, ConditionsT conditions)
    : mMessages(std::move(messages)), mConditions(std::move(conditions))
  {}

  template<typename... HandlersT> [[nodiscard]] auto Handler(HandlersT &&...handlers)
  {
    return MonitorRequirementsHandlers(
      mMessages, Monitoring::Monitor(mConditions, std::forward<HandlersT>(handlers)...));
  }

private:
  MessagesT mMessages;
  ConditionsT mConditions;
};


template<typename... MessageT> struct Monitor
{
  Monitor(MessageT... messages) : mMessages(messages...) {}

  template<typename ConditionsT> [[nodiscard]] auto Require(ConditionsT conditions)
  {
    return MonitorRequirements(mMessages, conditions);
  }

private:
  std::tuple<MessageT...> mMessages;
};

}// namespace PubSubMonitoring
