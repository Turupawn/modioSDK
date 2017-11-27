#include "instance_data_containers/Image.h"

namespace modio
{
  void Image::initialize(ModioImage modio_image)
  {
    if(modio_image.filename)
      this->filename = modio_image.filename;
    if(modio_image.full)
      this->full = modio_image.full;
    if(modio_image.thumb_1280x720)
      this->thumb_1280x720 = modio_image.thumb_1280x720;
    if(modio_image.thumb_640x360)
      this->thumb_640x360 = modio_image.thumb_640x360;
    if(modio_image.thumb_320x180)
      this->thumb_320x180 = modio_image.thumb_320x180;
  }
}