#ifndef SUBNET_V6_H
#define SUBNET_V6_H

#include "subnet.h"

/*

 TODO: Implement this. at the time, this class is just a copy of the IPv4 Subnet Class.
 I do not even think, that this is a very good idea to inherit Subnet like this. Maybe
 a restructurization would be in order? Maybe when issuing a model which uses polymorpic
 methods for storing and/or accessing its data...

 Technically the implementation of IPv6 calculations is very similar to IPv4. The
 arithmetics are the same, just the address space has quadrupled to a 128bit string.

 We do not have a datatype for that. As modern processors have a 64bit address space,
 there is simply no room for 128bit long numbers in the cpu registers. So we must emulate
 the correct behaviour. As we do not need features like bit carrying or stuff like that,
 and bitwise operations are atomic to each single bit we can just split the number into
 two 64bit quint64 variables and do the calculations twice for each part of the ip
 address string. This should work pretty well, reducing the big problem to writing
 parser routines for parsing a readbale IPv6 address to our variables even in reduced
 form.

 I would say there should be three steps involved:

 1.) check validity of the address string (regexp)
 2.) undo the reducement to a normalized for of an IPv6 address.
 3.) parse this normalized form, which should be trivial.

 Divided. Time to conquer.

*/

class Subnet_v6 : public Subnet
{
public:
    explicit Subnet_v6(QObject *parent = 0);
};

#endif // SUBNET_V6_H
