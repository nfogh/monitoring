#pragma once

#include <string>

namespace PubSubMonitoring {
struct IMessage
{
  virtual ~IMessage() = default;
  virtual const void *GetData() const = 0;
  virtual void *GetData() = 0;
  virtual int GetSize() const = 0;
  virtual const std::string& GetName() const = 0;
};
}// namespace PubSubMonitoring
