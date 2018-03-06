#include "soundeffect.h"

SoundEffect *SoundEffect::_instance = NULL;
SoundEffect::SoundEffect(QObject *parent) : QObject(parent)
{
    errorEffct = new QSound(":/error.wav");
    successEffect = new QSound(":/success.wav");
    snapshotEffect = new QSound(":/snapshot.wav");
}

void SoundEffect::triggerSoundEffect(SoundEffect::EffectMode mode)
{
    switch (mode) {
    case Error:
        errorEffct->play();
        break;
    case Success:
        successEffect->play();
        break;
    case Snapshot:
        snapshotEffect->play();
        break;
    default:
        break;
    }
}

const QSound *SoundEffect::getSoundEffect(SoundEffect::EffectMode mode)
{
    switch (mode) {
    case Error:
        return errorEffct;
    case Success:
        return successEffect;
    case Snapshot:
        return snapshotEffect;
    default:
        break;
    }
    return NULL;
}
