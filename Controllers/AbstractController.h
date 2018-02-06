#ifndef ABSTRACTCONTROLLER_H
#define ABSTRACTCONTROLLER_H

#include "Services/ServiceContainer.h"

class AbstractController
{
private:

protected:
    ServiceContainer * serviceContainer;

public:
    AbstractController();
    void setServiceContainer(ServiceContainer *);
};

#endif // ABSTRACTCONTROLLER_H
