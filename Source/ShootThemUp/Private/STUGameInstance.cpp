#include "STUGameInstance.h"

#include "STUSoundFuncLib.h"

void USTUGameInstance::ToggleVolume() const
{
    USTUSoundFuncLib::ToggleSoundClassVolume(MasterSoundClass);
}