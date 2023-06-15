#pragma once

namespace PubSubMonitoring {
struct IMessage
{
  virtual ~IMessage() = default;
  virtual void *GetData() const = 0;
  virtual void *GetData() = 0;
  virtual int GetSize() const = 0;
};
}// namespace PubSubMonitoring
