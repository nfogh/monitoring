#pragma once

#include "PubSub/IPubSub.hpp"
#include "PubSub/IMessage.hpp"
#include <functional>
#include <tuple>

namespace PubSubMonitoring {
  namespace intern
  {
    template <typename TupleT, typename Fn>
    void for_each_tuple(Fn&& fn, TupleT&& tp) {
      std::apply
      (
          [&fn](auto&& ...args)
          {
              (fn(std::forward<decltype(args)>(args)), ...);
          }, std::forward<TupleT>(tp)
      );
    }
  }

  template<typename MessagesTpl, typename MonitorT>
  struct PubSubMonitor
  {
    PubSubMonitor(IPubSub &pubsub, MessagesTpl messages, MonitorT monitor) : mPubSub(pubsub), mMessages(std::move(messages)), mMonitor(std::move(monitor))
    {
      intern::for_each_tuple([&](auto &arg) { mPubSub.Subscribe(arg, [&] { Check(); }); }, mMessages);
    }

    ~PubSubMonitor()
    {
      intern::for_each_tuple([&](auto& arg) { mPubSub.Unsubscribe(arg); }, mMessages);
    }

  private:
    void Check()
    {
      std::apply([&](auto&&... args) { mMonitor(std::forward<decltype(args)>(args)...); }, mMessages);
    }

    IPubSub& mPubSub;
    MessagesTpl mMessages;
    MonitorT mMonitor;
  };

  template<typename MessagesT, typename MonitorT>
  struct MonitorRequirementsHandlers
  {
    MonitorRequirementsHandlers(MessagesT messages, MonitorT monitor) : mMessages(std::move(messages)), mMonitor(std::move(monitor)) {}

    [[nodiscard]] auto With(IPubSub& pubsub)
    {
      return PubSubMonitor(pubsub, mMessages, mMonitor);
    }

    private:
    MessagesT mMessages;
    MonitorT mMonitor;
  };

  template<typename MessagesT, typename ConditionsT>
  struct MonitorRequirements
  {
    MonitorRequirements(MessagesT messages, ConditionsT conditions) : mMessages(std::move(messages)), mConditions(std::move(conditions)) {}

    template<typename... HandlersT>
    [[nodiscard]] auto Handler(HandlersT&&... handlers)
    {
      return MonitorRequirementsHandlers(mMessages, Monitoring::Monitor(mConditions, std::forward<HandlersT>(handlers)...));
    }

    private:
    MessagesT mMessages;
    ConditionsT mConditions;
  };


  template<typename... MessageT>
  struct Monitor
  {
    Monitor(MessageT... messages) : mMessages(messages...) {}

    template<typename ConditionsT>
    [[nodiscard]] auto Require(ConditionsT conditions) { return MonitorRequirements(mMessages, conditions); }

    private:
    std::tuple<MessageT...> mMessages;
  };

}// namespace PubSubMonitoring
