#pragma once

#include <string>

#include "envoy/http/codec.h"
#include "envoy/http/header_map.h"
#include "envoy/network/filter.h"
#include "envoy/router/router.h"
#include "envoy/upstream/cluster_manager.h"

#include "common/filter/tcp_proxy.h"
#include "common/http/websocket/ws_handler_impl.h"

namespace Envoy {
namespace Http {

// Future
// No point in tunnelling on end_stream
// TunnelHandler * shouldTunnel(HeaderMapPtr&& headers)

  class Http1ConnectHandlerImpl : public Filter::TcpProxy, public WsHandlerCallbacks {
public:
  Http1ConnectHandlerImpl(HeaderMap& request_headers, const Router::RouteEntry& reoute_entry,
                          Upstream::ClusterManager& cluster_manager,
                          Network::ReadFilterCallbacks* read_callbacks);

  //protected:
  // Filter::TcpProxy
  const std::string& getUpstreamCluster() override { return route_entry_.clusterName(); }

  void onInitFailure() override;
  void onUpstreamHostReady() override;
  void onConnectTimeoutError() override;
  void onConnectionFailure() override;
  void onConnectionSuccess() override;
  
  void sendHeadersOnlyResponse(HeaderMap& headers);

private:
  HeaderMap& request_headers_;
  const Router::RouteEntry& route_entry_;
  // WsHandlerCallbacks ws_callbacks_;
  // TODO(mattwoodyard) - refactor out common parts to a tunnelHandler
  //  my assertion is everything thing but onConnectionSuccess is common
  WebSocket::WsHandlerImpl common_impl_;
  // NullHttpConnectionCallbacks http_conn_callbacks_;
};

typedef std::unique_ptr<Http1ConnectHandlerImpl>
    Http1ConnectHandlerImplPtr;
}
}
