#pragma once

#ifdef FORWARD_API_EXPORTS
    #define FORWARD_API __declspec(dllexport)
#else
    #define FORWARD_API __declspec(dllimport)
#endif