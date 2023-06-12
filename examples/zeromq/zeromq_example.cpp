#include <future>
#include <iostream>
#include <string>
#include <thread>

#include "zmq.hpp"
#include "zmq_addon.hpp"

struct MessageIF
{
    virtual ~MessageIF() = default;
    virtual const std::string &name() const = 0;
    virtual const void *data() const = 0;
    virtual void *data() = 0;
    virtual int size() const = 0;
};

struct Message1 : public MessageIF
{
    const std::string& name() const override { return mName; }
    const void *data() const override { return &mData; }
    void *data() override { return &mData; }
    int size() const override { return sizeof(mData); }

    private:
    inline static const std::string mName{"Message1"};
    struct Data
    {
        int a = 0;
        int b = 0;
        int c = 0;
    } mData;
};

struct Message2 : public MessageIF
{
    const std::string& name() const override { return mName; }
    const void *data() const override { return &mData; }
    void *data() override { return &mData; }
    int size() const override { return sizeof(mData); }

    inline static const std::string mName{"Message2"};
    struct Data
    {
        float a = 0.0f;
        float b = 0.0f;
        float c = 0.0f;
    } mData;
};

std::mutex mtx;
void print(const std::string &str)
{
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << str << std::endl;
}

void PublisherThread(zmq::context_t *ctx) {
    //  Prepare publisher
    zmq::socket_t publisher(*ctx, zmq::socket_type::pub);
    publisher.bind("inproc://#1");

    // Give the subscribers a chance to connect, so they don't lose any messages
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    Message1 message1;
    Message2 message2;

    while (true) {
        //  Write three messages, each with an envelope and content
        publisher.send(zmq::message_t(message1.name()), zmq::send_flags::sndmore);
        publisher.send(zmq::const_buffer(message1.data(), static_cast<size_t>(message1.size())));
        publisher.send(zmq::message_t(message2.name()), zmq::send_flags::sndmore);
        publisher.send(zmq::const_buffer(message2.data(), static_cast<size_t>(message2.size())));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

/*template<typename Func>
auto FieldA(Func func)
{
  return [func = std::move(func)](const auto& arg){ return func(arg.mData.a); };
};

template<typename Func>
auto FieldB(Func func)
{
  return [func = std::move(func)](const auto& arg){ return func(arg.mData.b); };
};

template<typename Func>
auto FieldC(Func func)
{
  return [func = std::move(func)](const auto& arg){ return func(arg.mData.c); };
};


template<typename T>
void Subscribe(std::function<void(const T&)> callback)
{
    auto myMonitor = Monitor(Message1(), FieldA(LargerThan(0)), ImmediateAlarmActivation());
    callback(msg);
}*/

void SubscriberThread1(zmq::context_t *ctx) {
    //  Prepare subscriber
    zmq::socket_t subscriber(*ctx, zmq::socket_type::sub);
    subscriber.connect("inproc://#1");

    subscriber.set(zmq::sockopt::subscribe, Message1().name());

    while (1) {
        // Receive all parts of the message
        std::vector<zmq::message_t> recv_msgs;
        zmq::recv_result_t result =
          zmq::recv_multipart(subscriber, std::back_inserter(recv_msgs));
        assert(result && "recv failed");
        assert(*result == 2);

        //const auto messageName = recv_msgs[0].to_string_view();



        std::stringstream ss;

        ss << "Thread2: [" << recv_msgs[0].to_string() << "] "
                  << recv_msgs[1].size() << std::endl;

        print(ss.str());
    }
}

void SubscriberThread2(zmq::context_t *ctx) {
    //  Prepare our context and subscriber
    zmq::socket_t subscriber(*ctx, zmq::socket_type::sub);
    subscriber.connect("inproc://#1");

    subscriber.set(zmq::sockopt::subscribe, Message2().name());

    while (1) {
        // Receive all parts of the message
        std::vector<zmq::message_t> recv_msgs;
        zmq::recv_result_t result =
          zmq::recv_multipart(subscriber, std::back_inserter(recv_msgs));
        assert(result && "recv failed");
        assert(*result == 2);

        std::stringstream ss;

        ss << "Thread3: [" << recv_msgs[0].to_string() << "] "
                  << recv_msgs[1].size() << std::endl;

        print(ss.str());
    }
}

int main()
{
    /*
     * No I/O threads are involved in passing messages using the inproc transport.
     * Therefore, if you are using a ØMQ context for in-process messaging only you
     * can initialise the context with zero I/O threads.
     *
     * Source: http://api.zeromq.org/4-3:zmq-inproc
     */
    zmq::context_t ctx(0);

    auto thread1 = std::async(std::launch::async, PublisherThread, &ctx);

    // Give the publisher a chance to bind, since inproc requires it
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    auto thread2 = std::async(std::launch::async, SubscriberThread1, &ctx);
    auto thread3 = std::async(std::launch::async, SubscriberThread2, &ctx);
    thread1.wait();
    thread2.wait();
    thread3.wait();

    /*
     * Output:
     *   An infinite loop of a mix of:
     *     Thread2: [A] Message in A envelope
     *     Thread2: [B] Message in B envelope
     *     Thread3: [A] Message in A envelope
     *     Thread3: [B] Message in B envelope
     *     Thread3: [C] Message in C envelope
     */
}
