#pragma once
#include "stdafx.h"

class Object
{
public:
    Object() { id = GUID_Generator::Generator(); }
    virtual ~Object() = default;

    auto GetID() const { return id; }

private:
    uint id = 0;
};