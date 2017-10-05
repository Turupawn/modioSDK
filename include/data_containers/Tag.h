#ifndef MODWORKS_TAG
#define MODWORKS_TAG

#include "dependencies/json/json.hpp"
#include "Utility.h"

using namespace std;
using json = nlohmann::json;

namespace modworks
{
  class Tag
  {
  public:
    Tag(json tag_json);
    int game;
    int mod;
    int date;
    string tag;
  };
}

#endif