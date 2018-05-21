#ifndef SOUNDEFFECT_H
#define SOUNDEFFECT_H

/**
 * @brief         提供某些提示音效
 * @author        xiaobin <xiaobin@sunniwell.com>
 * @date          20180504
 */

#include <QObject>
#include <QSound>

class SoundEffect : public QObject
{
    Q_OBJECT
private:
    explicit SoundEffect(QObject *parent = nullptr);
    static SoundEffect *s_instance;

public:
    //声效类别的枚举值
    enum EffectMode {
        Error,
        Success,
        Snapshot
    };
    static SoundEffect *getInstance() {
        if (s_instance == NULL) {
            s_instance = new SoundEffect();
        }
        return s_instance;
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
