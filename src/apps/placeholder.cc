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

extern "C" int putc_extra(char c, FILE *file, bool semihosting_enabled)
{
    (void)c;
    (void)file;
    (void)semihosting_enabled;
    return -1;
}

extern "C" void getc_extra(int *result, FILE *file, bool semihosting_enabled)
{
    (void)result;
    (void)file;
    (void)semihosting_enabled;
}

extern "C" int flush_extra(FILE *file, bool semihosting_enabled)
{
    (void)file;
    (void)semihosting_enabled;
    return -1;
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
