#ifndef AIRPLANE_H
#define AIRPLANE_H

class AirPlane
{
public:
    AirPlane();
public:
    virtual void fly( const AirPlane& destination )=0;
};

#endif // AIRPLANE_H
