#ifndef PENROSE_API_HPP
#define PENROSE_API_HPP

#if defined(_WIN32) && !defined(__GNUC__)
#define PENROSE_API __declspec(dllexport)
#elif defined(__GNUC__)
#define PENROSE_API __attribute__ ((visibility ("default")))
#else
#define PENROSE_API
#endif

#endif // PENROSE_API_HPP
