#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include "messages.h"

/*

This is a list of messages generated by PropLoader.  There are two command line options that affect the message output; -c and -v (code and verbose, respectively).
By default (ie: when no -c or -v option is active) a limited set of messages appears and contain just the text of the message.

With -c, a numeric code is prepended to each message in the format ###-<text_message>.
With -v, verbose (debugging) messages appear along with normal messages.  These are messages meant only for PropLoader developers to diagnose problems.
Both -c and -v can be used individually or together.

There are three categories of messages and message codes:

  * Status - These express state/progress/event information and are given codes 001 through 099.

  * Error - These express fatal problems and are given codes 100 and beyond.

  * Verbose - These are for seasoned developers. They may express state information or specific deep error information that is usually only helpful to a small
    set of users, thus, they are never shown without an active -v option on the command line. These are all automatically given the code 000; tools that parse
    them should ignore all 000-coded messages.

Code numbers ARE NEVER REUSED for a condition that means something different than what was first intended by a message.  When a new State or Error message
is created, it simply takes on the next available code number even if it's logically related to another far away message.

*/

// message codes 1-99 -- must be in the same order as the INFO_xxx enum values in messages.h
static const char *infoText[] = {
"Opening file '%s'",
"Downloading file to port %s",
"Verifying RAM",
"Programming EEPROM",
"Download successful!",
"[ Entering terminal mode. Type ESC or Control-C to exit. ]",
"Writing '%s' to the SD card",
"%ld bytes remaining             ",
"%ld bytes sent                  ",
"Setting module name to '%s'"
};

// message codes 100 and up -- must be in the same order as the ERROR_xxx enum values in messsages.h
static const char *errorText[] = {
"Option -n can only be used to name wifi modules",
"Invalid address: %s",
"Download failed: %d",
"Can't open file '%s'",
"Propeller not found on port %s",
"Failed to enter terminal mode",
"Unrecognized wi-fi module firmware\n\
    Version is %s but expected %s.\n\
    Recommended action: update firmware and/or PropLoader to latest version(s).",
"Failed to write SD card file '%s'",
"Invalid module name",
"Failed to set module name",
"File is truncated or not a Propeller application image",
"File is corrupt or not a Propeller application",
"Can't read Propeller application file '%s'",
"Wifi module discovery failed",
"No wifi modules found",
"Serial port discovery failed",
"No serial ports found",
"Unable to connect to port %s",
"Unable to connect to module at %s",
"Failed to set baud rate",
"Internal error",
"Insufficient memory"
};

static void vmessage(const char *fmt, va_list ap, int eol);
static void vnmessage(int code, const char *fmt, va_list ap, int eol);

static const char *messageText(int code)
{
    const char *fmt;
    if (code >= MIN_INFO && code < MAX_INFO)
        fmt = infoText[code - MIN_INFO];
    else if (code >= MIN_ERROR && code < MAX_ERROR)
        fmt = errorText[code - MIN_ERROR];
    else
        fmt = "Internal error";
    return fmt;
}

int verbose = 0;
int showMessageCodes = false;

int error(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vmessage(fmt, ap, '\n');
    va_end(ap);
    return -1;
}

int nerror(int code, ...)
{
    va_list ap;
    va_start(ap, code);
    vnmessage(code, messageText(code), ap, '\n');
    va_end(ap);
    return -1;
}

void message(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vmessage(fmt, ap, '\n');
    va_end(ap);
}

void nmessage(int code, ...)
{
    va_list ap;
    va_start(ap, code);
    vnmessage(code, messageText(code), ap, '\n');
    va_end(ap);
}

void nprogress(int code, ...)
{
    va_list ap;
    va_start(ap, code);
    vnmessage(code, messageText(code), ap, '\n');
    va_end(ap);
}

static void vmessage(const char *fmt, va_list ap, int eol)
{
    const char *p = fmt;
    int code = 0;

    /* check for and parse the numeric message code */
    if (*p && isdigit(*p)) {
        while (*p && isdigit(*p))
            code = code * 10 + *p++ - '0';
        if (*p == '-')
            fmt = ++p;
    }

    /* display messages in verbose mode or when the code is > 0 */
    if (verbose || code > 0) {
        if (showMessageCodes)
            printf("%03d-", code);
        if (code > 99)
            printf("ERROR: ");
        vprintf(fmt, ap);
        putchar(eol);
        if (eol == '\r')
            fflush(stdout);
    }
}

static void vnmessage(int code, const char *fmt, va_list ap, int eol)
{
    /* display messages in verbose mode or when the code is > 0 */
    if (verbose || code > 0) {
        if (showMessageCodes)
            printf("%03d-", code);
        if (code > 99)
            printf("ERROR: ");
        vprintf(fmt, ap);
        putchar(eol);
        if (eol == '\r')
            fflush(stdout);
    }
}