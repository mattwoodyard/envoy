

#include <string>

#include "envoy/http/codec.h"
#include "envoy/http/header_map.h"
#include "envoy/network/filter.h"
#include "envoy/router/router.h"
#include "envoy/upstream/cluster_manager.h"
#include "common/http/header_map_impl.h"
#include "common/common/enum_to_int.h"
#include "common/http/codes.h"

#include "common/filter/tcp_proxy.h"
#include "common/http/http1_connect_handler.h"

namespace Envoy {
namespace Http {
Http1ConnectHandlerImpl::Http1ConnectHandlerImpl(HeaderMap& request_headers,
                                                 const Router::RouteEntry& route_entry,
                                                 Upstream::ClusterManager& cluster_manager,
                                                 Network::ReadFilterCallbacks* read_callbacks)
    : Filter::TcpProxy(nullptr, cluster_manager), request_headers_(request_headers),
      route_entry_(route_entry),
      common_impl_(request_headers, route_entry, *this, cluster_manager, read_callbacks) {

  read_callbacks_ = read_callbacks;
  read_callbacks_->connection().addConnectionCallbacks(downstream_callbacks_);
}

void Http1ConnectHandlerImpl::onInitFailure() { common_impl_.onInitFailure(); }

void Http1ConnectHandlerImpl::onUpstreamHostReady() { common_impl_.onUpstreamHostReady(); }
void Http1ConnectHandlerImpl::onConnectTimeoutError() { common_impl_.onConnectTimeoutError(); }
void Http1ConnectHandlerImpl::onConnectionFailure() { common_impl_.onConnectionFailure(); }
void Http1ConnectHandlerImpl::onConnectionSuccess() {
  // TODO(woodyard) - 200 OK and off you go
  HeaderMapImpl headers{{Headers::get().Status, std::to_string(enumToInt(Code::OK))}};
  sendHeadersOnlyResponse(headers);
}

void Http1ConnectHandlerImpl::sendHeadersOnlyResponse(HeaderMap& ) {}
}
}
