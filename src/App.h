/**
 * \file
 * \brief A simple application interface.
 */
#pragma once

/* third-party */
#include "coral/cli/CommandLineApp.h"

extern "C"
{
/* toolchain */
#include <semihost.h>
}

using CommandLine = Coral::CommandLineApp::CommandLine;
using CommandLineApp = Coral::CommandLineApp;

static constexpr const char *prompt = "$ ";

/*
 * Must be set by a debugger.
 */
extern volatile bool enable_semihosting;

extern int stdin_fd;

class App
{
  public:
    App(CommandLineApp::CommandRegistration register_commands,
        bool initialize_semihosting = true)
        : logger(), buf(), app(register_commands, buf, &logger)
    {
        if (initialize_semihosting && enable_semihosting)
        {
            stdin_fd = sys_semihost_open(":tt", 0);
        }

        register_common();

        logger.log("Application starting.\n");
        logger.log(prompt);
    }

    inline void poll_stdin(char *input)
    {
        if (stdin_fd > 0 and enable_semihosting and gets(input) != NULL)
        {
            /* Publish command data. */
            buf.push_n_blocking(input, strlen(input));
            buf.push_blocking('\n');

            logger.log(prompt);
        }
    }

    Coral::PrintfLogger logger;
    CommandLineApp::Processor::Buffer buf;
    CommandLineApp app;

  protected:
    inline void register_common()
    {
        app.add_handler(
            "cli", [this](CommandLine &cli) { do_cli(cli); },
            "toggle the semihosting CLI on or off");
    }

    void do_cli(CommandLine &cli)
    {
        (void)cli;

        enable_semihosting = not enable_semihosting;
        printf("Toggling semihosting CLI %s.\n",
               enable_semihosting ? "on" : "off");
    }
};
