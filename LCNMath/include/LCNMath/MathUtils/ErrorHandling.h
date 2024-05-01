#pragma once

// TODO : find a work around

#ifdef WIN32
#ifndef ASSERT
#define ASSERT(X) if(!(X)) __debugbreak()
#endif
#else
#define ASSERT(X)
#endif