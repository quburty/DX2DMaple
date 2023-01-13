#pragma once

class GUID_Generator final
{
public:
    static auto Generator() -> const uint;
    static auto GeneratorToString() -> const std::string;
};