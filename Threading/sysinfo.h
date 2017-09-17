#ifndef SYSINFO_H
#define SYSINFO_H
#if defined(__linux__) || defined(linux) || defined(__linux)
//linux
#define SYSINFO_OS_LINUX 
#endif

#if defined(_WIN32) || defined(_WIN64)
// windows
#define SYSINFO_OS_WINDOWS
#endif

#if defined(__APPLE__) || defined(__MACH__)
#define SYSINFO_OS_APPLE 
#endif

#if defined(unix) || defined(__unix) || defined(__unix__)
#define SYSINFO_OS_UNIX
#endif

#endif //SYSINFO_H
