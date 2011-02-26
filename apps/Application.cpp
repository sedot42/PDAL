/******************************************************************************
* Copyright (c) 2011, Michael P. Gerlek (mpg@flaxen.com)
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

#include <iostream>

#include "Application.hpp"

#include "libpc/libpc_config.hpp"

using namespace libpc;

namespace po = boost::program_options;


Application::Application(int argc, char* argv[], const std::string& appName)
    : m_argc(argc)
    , m_argv(argv)
    , m_appName(appName)
    , m_isVerbose(false)
{
    return;
}

int Application::run()
{
    // add -h, -v, etc
    addBasicOptionSet();

    // add the options for the derived application
    addOptions();

    // parse the command line
    parseOptions();

    // handle the well-known options
    if (hasOption("version")) 
    {
        outputVersion(std::cout);
        return 0;
    }
    
    if (hasOption("help")) 
    {
        outputHelp(std::cout);
        return 0;
    }

    return 0;
}


bool Application::hasOption(const std::string& name)
{
    return m_variablesMap.count(name) > 0;
}


bool Application::isVerbose() const
{
    return m_isVerbose;
}


void Application::addOptionSet(po::options_description* options)
{
    m_options.push_back(options);
}


void Application::outputHelp(std::ostream& oss)
{
    outputVersion(oss);

    std::vector<po::options_description*>::const_iterator iter;
    for (iter = m_options.begin(); iter != m_options.end(); ++iter)
    {
        const po::options_description* options = *iter;
        oss << *options;
    }

    oss <<"\nFor more information, see the full documentation for las2las at:\n";
    
    oss << " http://liblas.org/utilities/las2las.html\n";
    oss << "----------------------------------------------------------\n";
}


void Application::outputVersion(std::ostream& oss)
{
    oss << "--------------------------------------------------------------------\n";
    oss << "    pc2pc (" << libpc::GetFullVersionString() << ")\n";
    oss << "--------------------------------------------------------------------\n";
}


void Application::addBasicOptionSet()
{
    po::options_description* file_options = new po::options_description("las2las options");

    file_options->add_options()
        ("help,h", "produce help message")
        ("verbose,v", po::value<bool>(&m_isVerbose)->zero_tokens(), "Verbose message output")
        ("version", "Show version info")
        ;

    addOptionSet(file_options);

    return;
}


void Application::parseOptions()
{
    po::options_description options;

    std::vector<po::options_description*>::iterator iter;
    for (iter = m_options.begin(); iter != m_options.end(); ++iter)
    {
        po::options_description* sub_options = *iter;
        options.add(*sub_options);
    }

    //po::positional_options_description p;
    //p.add("input", 1);
    //p.add("output", 1);

    po::store(po::command_line_parser(m_argc, m_argv).
        options(options)/*.positional(p)*/.run(), m_variablesMap);

    po::notify(m_variablesMap);

    return;
}

