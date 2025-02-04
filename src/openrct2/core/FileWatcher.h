/*****************************************************************************
 * Copyright (c) 2014-2023 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#pragma once

#include "String.hpp"

#include <functional>
#include <string>
#include <thread>
#include <vector>

#ifdef _WIN32
#    include "FileSystem.hpp"

typedef void* HANDLE;
#endif

/**
 * Creates a new thread that watches a directory tree for file modifications.
 */
class FileWatcher
{
private:
    std::thread _watchThread;
#if defined(_WIN32)
    fs::path _path;
    HANDLE _directoryHandle{};
#elif defined(__linux__)
    struct FileDescriptor
    {
        int Fd = -1;

        ~FileDescriptor();
        void Initialise();
        void Close();
    };

    struct WatchDescriptor
    {
        int const Fd;
        int const Wd;
        std::string const Path;

        WatchDescriptor(int fd, const std::string& path);
        ~WatchDescriptor();
    };

    FileDescriptor _fileDesc;
    std::vector<WatchDescriptor> _watchDescs;
#endif

public:
    std::function<void(u8string_view path)> OnFileChanged;

    FileWatcher(u8string_view directoryPath);
    ~FileWatcher();

private:
#if defined(_WIN32) || defined(__linux__)
    bool _finished{};
#endif

    void WatchDirectory();
};
