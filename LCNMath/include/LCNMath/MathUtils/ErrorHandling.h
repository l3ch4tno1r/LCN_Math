#pragma once

// TODO : find a work around

#ifndef ASSERT
#define ASSERT(X) if(!(X)) __debugbreak()
#endif