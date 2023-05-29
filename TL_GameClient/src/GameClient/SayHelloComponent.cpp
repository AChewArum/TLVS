#include "GameClient/SayHelloComponent.h"

void SayHelloComponent::Tick()
{
    ComponentBase::Tick();

    DebugHelper::PrintDebugString(TEXT("hello \n"));
}
