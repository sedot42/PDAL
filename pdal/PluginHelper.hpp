/******************************************************************************
* Copyright (c) 2012, Howard Butler (hobu.inc@gmail.com)
*
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following
* conditions are met:
*
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in
*       the documentation and/or other materials provided
*       with the distribution.
*     * Neither the name of Hobu, Inc. or Flaxen Geo Consulting nor the
*       names of its contributors may be used to endorse or promote
*       products derived from this software without specific prior
*       written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
* OF SUCH DAMAGE.
****************************************************************************/

#pragma once

#include <string>

#include <pdal/pdal_export.hpp>

namespace pdal
{

struct PluginInfo
{
    std::string name;
    std::string description;
    std::string link;
    PluginInfo(const std::string& n, const std::string& d, const std::string& l)
      : name(n), description(d), link(l)
    {}
};

}

extern "C"
{
// This is a placeholder so as not to break existing plugins even though the
// internal interface has changed.
typedef void PF_ExitFunc;
typedef void (*PF_InitFunc)();
}

#include <pdal/PluginManager.hpp>

#define CREATE_SHARED_PLUGIN(version_major, version_minor, T, type, info) \
    extern "C" PDAL_DLL void PF_initPlugin() \
    { \
        bool stage = std::is_convertible<T*, Stage *>::value; \
        if (stage) \
            pdal::PluginManager<pdal::Stage>::registerPlugin<T>(info); \
        else \
            pdal::PluginManager<pdal::Kernel>::registerPlugin<T>(info); \
    }

#define CREATE_STATIC_PLUGIN(version_major, version_minor, T, type, info) \
    extern "C" PDAL_DLL void T ## _InitPlugin() \
    { \
        bool stage = std::is_convertible<T*, Stage *>::value; \
        if (stage) \
            pdal::PluginManager<pdal::Stage>::registerPlugin<T>(info); \
        else \
            pdal::PluginManager<pdal::Kernel>::registerPlugin<T>(info); \
    }
