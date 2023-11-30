#include "Provider.h"

Provider& Provider::get()
{
    static Provider self;
    return self;
}
