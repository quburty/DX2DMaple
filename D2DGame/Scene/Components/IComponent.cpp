#include "stdafx.h"
#include "IComponent.h"
#include "../Actor/Actor.h"

IComponent::IComponent(Context * const context, Actor * const actor)
    : context(context)
    , actor(actor)
{
}