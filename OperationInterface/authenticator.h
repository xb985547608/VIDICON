#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

/**
 * @brief         验证器抽象接口，用于授权
 * @author        xiaobin <xiaobin@sunniwell.com>
 * @date          20180504
 */

#include <QString>

class Authenticator {

public:
    Authenticator() {}
    virtual bool isAuthorization() = 0;
    virtual QString name() = 0;
private:
};

#endif // AUTHORIZATION_H
