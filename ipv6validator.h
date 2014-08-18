/**
 *
 * This file is part of SubnetMapper.
 *
 * Copyright (C) 2008 "Thomas Thiel" <kv@wolfchild.de>
 *
 * SubnetMapper is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * SubnetMapper is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

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
