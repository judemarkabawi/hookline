#include "misc.hpp"

#include <glm/glm.hpp>
#include <vector>

#if defined(_WIN32)
#include <windows.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#elif defined(__linux__)
#include <sys/stat.h>
#include <unistd.h>
#endif  // WINDOWS

namespace {
/**
    Get the path to the current executable in various OS-specific ways.
 */
static std::string get_exe_path() {
#if defined(_WIN32)
    // See:
    // https://stackoverflow.com/questions/1023306/finding-current-executables-path-without-proc-self-exe
    TCHAR buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::string ret = buffer;
    ret = ret.substr(0, ret.rfind('\\'));
    return ret;
#elif defined(__linux__)
    // From:
    // https://stackoverflow.com/questions/933850/how-do-i-find-the-location-of-the-executable-in-c
    std::vector<char> buffer(1000);
    while (1) {
        ssize_t got = readlink("/proc/self/exe", &buffer[0], buffer.size());
        if (got <= 0) {
            return "";
        } else if (got < (ssize_t)buffer.size()) {
            std::string ret = std::string(buffer.begin(), buffer.begin() + got);
            return ret.substr(0, ret.rfind('/'));
        }
        buffer.resize(buffer.size() + 4000);
    }
#elif defined(__APPLE__)
    // From:
    // https://stackoverflow.com/questions/799679/programmatically-retrieving-the-absolute-path-of-an-os-x-command-line-app/1024933
    uint32_t bufsize = 0;
    std::vector<char> buffer;
    _NSGetExecutablePath(&buffer[0], &bufsize);
    buffer.resize(bufsize, '\0');
    bufsize = buffer.size();
    if (_NSGetExecutablePath(&buffer[0], &bufsize) != 0) {
        throw std::runtime_error(
            "Call to _NSGetExecutablePath failed for mysterious reasons.");
    }
    std::string ret = std::string(&buffer[0]);
    return ret.substr(0, ret.rfind('/'));
#else
#error "No idea what the OS is."
#endif
}
}  // namespace

namespace hookline {
std::vector<glm::vec2> get_basic_shape_debug() {
    return {/*BL*/ {-1.0, -1.0}, /*BR*/ {1.0, -1.0},
            /*TL*/ {-1.0, 1.0}, /*TR*/ {1.0, 1.0}};
}

std::string data_path(std::string const &suffix) {
    static std::string path = get_exe_path();  // cache result of get_exe_path()
    return path + "/" + suffix;
}
}  // namespace hookline