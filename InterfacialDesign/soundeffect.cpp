#include "soundeffect.h"

SoundEffect *SoundEffect::_instance = NULL;
SoundEffect::SoundEffect(QObject *parent) : QObject(parent)
{
    m_errorEffct = new QSound(":/error.wav");
    m_successEffect = new QSound(":/success.wav");
    m_snapshotEffect = new QSound(":/snapshot.wav");
}

void SoundEffect::triggerSoundEffect(SoundEffect::EffectMode mode)
{
    switch (mode) {
    case Error:
        m_errorEffct->play();
        break;
    case Success:
        m_successEffect->play();
        break;
    case Snapshot:
        m_snapshotEffect->play();
        break;
    default:
        break;
    }
}

const QSound *SoundEffect::getSoundEffect(SoundEffect::EffectMode mode)
{
    switch (mode) {
    case Error:
        return m_errorEffct;
    case Success:
        return m_successEffect;
    case Snapshot:
        return m_snapshotEffect;
    default:
        break;
    }
    return NULL;
}
