/**
 * \file
 * \brief Some standard-library hook implementations.
 */

#ifdef __cplusplus
extern "C"
{
#endif

/* toolchain */
#include <semihost.h>
#include <stdio.h>

    /*
     * Must be provided by the application.
     */
    extern int putc_extra(char c, FILE *file, bool semihosting_enabled);
    extern void getc_extra(int *result, FILE *file, bool semihosting_enabled);
    extern int flush_extra(FILE *file, bool semihosting_enabled);

    /*
     * Must be set by a debugger.
     */
    volatile bool enable_semihosting = false;

    static int putc_wrapper(char c, FILE *file)
    {
        bool try_semihost = enable_semihosting;
        if (try_semihost)
        {
            char out[2] = {c, 0};
            sys_semihost_write0(out);
        }
        return putc_extra(c, file, try_semihost);
    }

    int stdin_fd = -1;

    static int getc_wrapper(FILE *file)
    {
        int result = -1;

        bool try_semihost = enable_semihosting && stdin_fd != -1;

        if (try_semihost)
        {
            char input = result;

            /*
             * The return value seems to be meaningless. There also does not
             * seem to be a way to make this non-blocking.
             */
            sys_semihost_read(stdin_fd, &input, sizeof(char));

            result = input;
        }
        getc_extra(&result, file, try_semihost);

        return result;
    }

    static int flush_wrapper(FILE *file)
    {
        return flush_extra(file, enable_semihosting);
    }

    static FILE __stdio = FDEV_SETUP_STREAM(putc_wrapper, getc_wrapper,
                                            flush_wrapper, _FDEV_SETUP_RW);

    FILE *const stdout = &__stdio;
    FILE *const stdin = &__stdio;
    FILE *const stderr = NULL;

#ifdef __cplusplus
}
#endif
