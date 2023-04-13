#include "tp_image_utils/AbstractImageSequence.h"

namespace tp_image_utils
{
std::vector<std::function<AbstractImageSequence*(const std::string&)>> AbstractImageSequence::sm_factories;

//##################################################################################################
AbstractImageSequence::~AbstractImageSequence()=default;

//##################################################################################################
AbstractImageSequence* AbstractImageSequence::loadSequence(const std::string& data)
{
  for(const auto& factory : sm_factories)
    if(auto s=factory(data); s)
      return s;

  return nullptr;
}

//##################################################################################################
void AbstractImageSequence::addFactory(const std::function<AbstractImageSequence*(const std::string&)>& factory)
{
  sm_factories.push_back(factory);
}

}
