#pragma once

#include <chrono>
#include <memory>

#include "envoy/server/guarddog.h"
#include "envoy/server/listener_manager.h"
#include "envoy/thread_local/thread_local.h"

#include "common/common/thread.h"

#include "server/connection_handler_impl.h"

namespace Envoy {
namespace Server {

/**
 * A server threaded worker that wraps up a worker thread, event loop, etc.
 */
class Worker : Logger::Loggable<Logger::Id::main> {
public:
  Worker(ThreadLocal::Instance& tls, std::chrono::milliseconds file_flush_interval_msec);

  Event::Dispatcher& dispatcher() { return handler_->dispatcher(); }
  Network::ConnectionHandler* handler() { return handler_.get(); }

  // fixfix
  void initialize(GuardDog& guard_dog);
  void addListenerNonThreadSafe(ListenerSharedPtr listener);
  void addListenerThreadSafe(ListenerSharedPtr listener);
  void removeListener(uint64_t listener_unique_id);

  /**
   * Exit the worker. Will block until the worker thread joins. Called from the main thread.
   */
  void exit();

private:
  void onNoExitTimer();
  void threadRoutine(GuardDog& guard_dog);

  ThreadLocal::Instance& tls_;
  ConnectionHandlerImplPtr handler_;
  Event::TimerPtr no_exit_timer_;
  Thread::ThreadPtr thread_;
};

typedef std::unique_ptr<Worker> WorkerPtr;

} // Server
} // Envoy
