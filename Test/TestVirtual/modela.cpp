#include "modela.h"
#include "airplane.h"

ModelA::ModelA()
{
}

void ModelA::fly( const AirPlane &destination )
{
    AirPlane::fly( destination);
}
