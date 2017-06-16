#pragma once

#include "common/json/json_validator.h"

namespace Envoy {
namespace Server {

class LdsApi : Json::Validator {
public:
  LdsApi(const Json::Object& config);
};

} // Server
} // Envoy
