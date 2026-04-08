#include "core/UUID.hpp"

static std::random_device random_device;
static std::mt19937_64 engine(random_device());
static std::uniform_int_distribution<uint64_t> distribution;

meteor::UUID::UUID()
    : uuid_(distribution(engine))
{}

meteor::UUID::UUID(uint64_t uuid)
    : uuid_(uuid)
{}
