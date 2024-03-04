#include "ILight.h"

ILight::~ILight()
{
	resource_->Release();
}