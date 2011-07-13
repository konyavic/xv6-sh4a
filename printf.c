#include <stdarg.h>
#include "types.h"
#include "sh.h"
//#include <proc.h>
//#include <defs.h>
//#include "types.h"
#include "stat.h"
#include "user.h"

typedef int bool;
#define true    1
#define false   0

//#include "sync.h" /* spinlocks    */

#define NULL        0
#define print_hex64 print_hex
extern void getcallerpcs(void *v, uint pcs[]);

/* convert nibble to lowercase hex char */
#define hexchars(x) (((x) < 10) ? ('0' + (x)) : ('a' + ((x) - 10)))


static void
uputc(int fd, char c)
{
  write(fd, &c, 1);
}

int print_hex(int fd, int val,
              int  width,
          int precision,
          bool adjleft,
          bool nullpad)
{
    long i, n = 0;
    long nwidth = 0;


    while ((val >> (4 * nwidth)) && (int) nwidth <  2 * sizeof (int))
    nwidth++;

    if (nwidth == 0)
    nwidth = 1;

    // May need to increase number of printed digits
    if (precision > nwidth)
    nwidth = precision;

    // May need to increase number of printed characters
    if (width == 0 && width < nwidth)
    width = nwidth;

    // Print number with padding
    if (! adjleft)
    for (i = width - nwidth; i > 0; i--, n++)
        uputc (fd, nullpad ? '0' : ' ');
    for (i = 4 * (nwidth - 1); i >= 0; i -= 4, n++)
    uputc (fd, hexchars ((val >> i) & 0xF));
    if (adjleft)
    for (i = width - nwidth; i > 0; i--, n++)
        uputc (fd, ' ');

    return n;
}

/**
 *  Print a string
 *
 *  @param s    zero-terminated string to print
 *  @param width    minimum width of printed string
 *
 *  Prints the zero-terminated string using uputc().  The printed
 *  string will be right padded with space to so that it will be
 *  at least WIDTH characters wide.
 *
 *      @returns the number of charaters printed.
 */
int print_string(int fd, const char * s,
                     const int width,
                     const int precision)
{
    int n = 0;

    for (;;)
    {
    if (*s == 0)
        break;

    uputc(fd, *s++);
    n++;
    if (precision && n >= precision)
        break;
    }

    while (n < width) { uputc(fd, ' '); n++; }

    return n;
}

/**
 *  Print hexadecimal value with a separator
 *
 *  @param val  value to print
 *  @param bits number of lower-most bits before which to
 *                      place the separator
 *      @param sep      the separator to print
 *
 *  @returns the number of charaters printed.
 */
int print_hex_sep(int fd, const int val,
                      const int bits,
                      const char *sep)
{
    int n = 0;

    n = print_hex(fd, val >> bits, 0, 0, false, false);
    n += print_string(fd, sep, 0, 0);
    n += print_hex(fd, val & ((1 << bits) - 1), 0, 0, false, false);

    return n;
}


/**
 *  Print decimal value
 *
 *  @param val  value to print
 *  @param width    width of field
 *      @param pad      character used for padding value up to width
 *
 *  Prints a value as a decimal in the given WIDTH with leading
 *  whitespaces.
 *
 *  @returns the number of characters printed (may be more than WIDTH)
 */
int print_dec(int fd, const int val,
                const  int width,
                const  char pad)
{
    int divisor;
    int digits;


    /* estimate number of spaces and digits */
    for (divisor=1, digits=1; val/divisor >= 10; divisor*=10, digits++);


    /* print spaces */
    for ( ; digits < width; digits++ )
    uputc(fd, ' ');

    /* print digits */
    do {
    uputc(fd, ((val/divisor) % 10) + '0');
    } while (divisor /= 10);

    /* report number of digits printed */
    return digits;
}

// yumura lock
//DEFINE_SPINLOCK(printf_spin_lock);


/**
 *  Does the real printf work
 *
 *  @param format_p     pointer to format string
 *  @param args     list of arguments, variable length
 *
 *  Prints the given arguments as specified by the format string.
 *  Implements a subset of the well-known printf plus some L4-specifics.
 *
 *  @returns the number of characters printed
 */
int do_printf(int fd, const char* format_p, va_list args)
{
    const char* format = format_p;
    int n = 0;
    int i = 0;
    int width = 8;
    int precision = 0;
    bool adjleft = false, nullpad = false;

#define arg(x) va_arg(args, x)

    //yumura lock
    //printf_spin_lock.lock();

    /* sanity check */
    if (format == NULL)
    {   //yumura lock
    //printf_spin_lock.unlock();
    return 0;
    }

    while (*format)
    {
    switch (*(format))
    {
    case '%':
        width = precision = 0;
        adjleft = nullpad = false;
    reentry:
        switch (*(++format))
        {
        /* modifiers */
        case '.':
        for (format++; *format >= '0' && *format <= '9'; format++)
            precision = precision * 10 + (*format) - '0';
        if (*format == 'w')
        {
            // Set precision to printsize of a hex word
            precision = sizeof (int) * 2;
            format++;
        }
        format--;
        goto reentry;
        case '0':
        nullpad = (width == 0);
        case '1'...'9':
        width = width*10 + (*format)-'0';
        goto reentry;
        case 'w':
        // Set width to printsize of a hex word
        width = sizeof (int) * 2;
        goto reentry;
        case '-':
        adjleft = true;
        goto reentry;
        case 'l':
        goto reentry;
        break;
        case 'c':
        uputc(fd, arg(int));
        n++;
        break;
        case 'm':   /* microseconds */
        {
        n += print_hex64(fd, arg(long long int), width, precision,
                   adjleft, nullpad);
        break;
        }
        case 'd':
        {
        long val = arg(long);
        if (val < 0)
        {
            uputc(fd, '-');
            val = -val;
        }
        n += print_dec(fd, val, width, ' ');
        break;
        }
        case 'u':
        n += print_dec(fd, arg(long), width, ' ');
        break;
        case 'p':
        precision = sizeof (int) * 2;
        case 'x':
        n += print_hex(fd, arg(long), width, precision,
                   adjleft, nullpad);
        break;
        case 's':
        {
        char* s = arg(char*);
        if (s)
            n += print_string(fd, s, width, precision);
        else
            n += print_string(fd, "(null)", width, precision);
        }
        break;

//      case 't':
//      case 'T':
//      n += print_tid (arg (int), width, precision, adjleft);
//      break;

        case '%':
        uputc(fd, '%');
        n++;
        format++;
        continue;
        default:
        n += print_string(fd, "?", 0, 0);
        break;
        };
        i++;
        break;
    default:
        uputc(fd, *format);
        n++;
        break;
    }
    format++;
    }
    //yumura lock
    //printf_spin_lock.unlock();
    return n;
}

/**
 *  Flexible print function
 *
 *  @param format   string containing formatting and parameter type
 *          information
 *  @param ...  variable list of parameters
 *
 *  @returns the number of characters printed
 */
void printf(int fd, char* format, ...)
{
    va_list args;
    int i;
	//extern int xv6_scif_init;

	//if(!xv6_scif_init){
	//	extern void scif_init();

	//	scif_init();
	//}

    va_start(args, format);
    i = do_printf(fd, format, args);
    va_end(args);
    //return i;
};

//#include "types.h"
//#include "stat.h"
//#include "user.h"



//static void
//printint(int fd, int xx, int base, int sgn)
//{
//  static char digits[] = "0123456789ABCDEF";
//  char buf[16];
//  int i, neg;
//  uint x;

//  neg = 0;
//  if(sgn && xx < 0){
//    neg = 1;
//    x = -xx;
//  } else {
//    x = xx;
//  }

//  i = 0;
//  do{
//    buf[i++] = digits[x % base];
//  }while((x /= base) != 0);
//  if(neg)
//    buf[i++] = '-';

//  while(--i >= 0)
 //   putc(fd, buf[i]);
//}

// Print to the given fd. Only understands %d, %x, %p, %s.
//void
//printf(int fd, char *fmt, ...)
//{
//  char *s;
//  int c, i, state;
//  uint *ap;

//  state = 0;
//  ap = (uint*)(void*)&fmt + 1;
//  for(i = 0; fmt[i]; i++){
//    c = fmt[i] & 0xff;
//    if(state == 0){
//      if(c == '%'){
//        state = '%';
//      } else {
//        putc(fd, c);
//      }
//    } else if(state == '%'){
//      if(c == 'd'){
//        printint(fd, *ap, 10, 1);
//        ap++;
//      } else if(c == 'x' || c == 'p'){
//        printint(fd, *ap, 16, 0);
 //       ap++;
//      } else if(c == 's'){
//        s = (char*)*ap;
//        ap++;
//        if(s == 0)
//          s = "(null)";
//        while(*s != 0){
//          putc(fd, *s);
//          s++;
//        }
//      } else if(c == 'c'){
//        putc(fd, *ap);
//        ap++;
//      } else if(c == '%'){
//        putc(fd, c);
//      } else {
        // Unknown % sequence.  Print it to draw attention.
//        putc(fd, '%');
//        putc(fd, c);
//      }
//      state = 0;
//    }
//  }
//}
