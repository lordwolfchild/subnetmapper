#ifndef IPV6VALIDATOR_H
#define IPV6VALIDATOR_H

#include <QValidator>

class IPv6Validator : public QValidator
{
    Q_OBJECT
public:
    explicit IPv6Validator(QObject *parent = 0);
    void fixup ( QString & input ) const;
    State validate(QString & input, int & pos) const;

signals:

public slots:

};

#endif // IPV6VALIDATOR_H
