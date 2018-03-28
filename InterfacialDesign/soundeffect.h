#ifndef SOUNDEFFECT_H
#define SOUNDEFFECT_H

#include <QObject>
#include <QSound>

class SoundEffect : public QObject
{
    Q_OBJECT
private:
    explicit SoundEffect(QObject *parent = nullptr);
    static SoundEffect *_instance;

public:
    //声效类别的枚举值
    enum EffectMode {
        Error,
        Success,
        Snapshot
    };
    static SoundEffect *getInstance() {
        if (_instance == NULL) {
            _instance = new SoundEffect();
        }
        return _instance;
    }

    void triggerSoundEffect(EffectMode mode);
    const QSound *getSoundEffect(EffectMode mode);

signals:

public slots:

private:
    QSound *m_errorEffct;
    QSound *m_successEffect;
    QSound *m_snapshotEffect;
};

#endif // SOUNDEFFECT_H
