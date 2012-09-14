#include "ipv6validator.h"
#include "subnet_v6.h"
#include <QRegExp>

IPv6Validator::IPv6Validator(QObject *parent) :
    QValidator(parent)
{
}

void IPv6Validator::fixup(QString &input) const
{
    Subnet_v6 helperSubnet(input);
    input=helperSubnet.toString();

}

QValidator::State IPv6Validator::validate(QString &input, int & /*pos*/) const
{
    if (input.contains(QRegExp("[^0-9a-f:]{1,}",Qt::CaseInsensitive))) {
        return QValidator::Invalid;
    }

    if (input.count(':')>7) {
        return QValidator::Invalid;
    }

    if (input.contains(QRegExp("[:][:][:]{1,}",Qt::CaseInsensitive))) {
        return QValidator::Invalid;
    }


    if (input.contains(QRegExp("^([0-9a-f]{0,4}[:]){2,7}[0-9a-f]{0,4}$",Qt::CaseSensitive))) {
        if (input.contains("::")) {
            return QValidator::Acceptable;
        } else {
            if (input.count(':')==7) return QValidator::Acceptable;
        }
    }

    if (input.contains(QRegExp("^([0-9a-f]{0,4}[:]){0,8}$",Qt::CaseSensitive))) {
        return QValidator::Intermediate;
    }

    return QValidator::Intermediate;
}
