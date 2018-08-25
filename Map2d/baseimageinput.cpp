#include "baseimageinput.h"

BaseImageInput::BaseImageInput(int d)
{
    digitSource=d;
}

BaseImageInput::BaseImageInput(const std::string &s){
    strSource=s;
}
