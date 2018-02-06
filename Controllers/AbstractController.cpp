#include "AbstractController.h"

AbstractController::AbstractController()
{

}

void AbstractController::setServiceContainer(ServiceContainer * serviceContainer)
{
    this->serviceContainer = serviceContainer;
}
