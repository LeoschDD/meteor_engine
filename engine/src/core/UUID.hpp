#pragma once

#include "core/Headers.hpp"

namespace meteor
{
    class UUID
    {
    public: 
        UUID();
        UUID(uint64_t uuid);

        [[nodiscard]] uint64_t GetUUID() noexcept {return uuid_;}
        [[nodiscard]] const uint64_t GetUUID() const noexcept {return uuid_;}

    private:
        uint64_t uuid_;
    };
}