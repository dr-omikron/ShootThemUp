﻿#pragma once

class AnimUtils
{
public:
    template <typename T>
    static T* FindNotifyByClass(UAnimSequenceBase* Animation)
    {
        if(!Animation) return nullptr;
        const auto NotifyEvents = Animation->Notifies;
        for (auto NotifyEvent : NotifyEvents)
        {
            if(const auto AnimNotify = Cast<T>(NotifyEvent.Notify))
            {
                return AnimNotify;
            }
        }
        return nullptr;
    }
};