#ifndef MODELA_H
#define MODELA_H

class AirPlane;
class ModelA : public AirPlane
{
public:
    ModelA();
public:
    virtual void fly( const AirPlane& destination );

};

#endif // MODELA_H
