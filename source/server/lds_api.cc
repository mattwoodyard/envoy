#include "server/lds_api.h"

#include "common/json/config_schemas.h"

namespace Envoy {
namespace Server {

LdsApi::LdsApi(const Json::Object& config) : Json::Validator(config, Json::Schema::LDS_SCHEMA) {
}

} // Server
} // Envoy
