/* internal */
#include "App.h"

bool state = true;

void do_toggle(CommandLine &cli)
{
    (void)cli;
    state = not state;
    printf("Toggling state %s.\n", state ? "true" : "false");
}

void register_commands(CommandLineApp &app)
{
    app.add_handler("toggle", do_toggle, "toggle a boolean true or false");
}

char input[BUFSIZ];

extern "C" void _exit(int code)
{
    (void)code;

    while (1)
    {
        ;
    }
}

int main(void)
{
    App app(register_commands);

    while (true)
    {
        app.poll_stdin(input);
    }

    return 0;
}
