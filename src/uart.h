int simple_uart0_putchar(char c, FILE *stream);
int simple_uart0_getchar(FILE *stream);
int simple_uart1_putchar(char c, FILE *stream);

void simple_uart0_init(void);
void simple_uart1_init(void);

/* http://www.ermicro.com/blog/?p=325 */

FILE simple_uart0_io = FDEV_SETUP_STREAM(simple_uart0_putchar, simple_uart0_getchar, _FDEV_SETUP_WRITE);
FILE simple_uart1_out = FDEV_SETUP_STREAM(simple_uart1_putchar, NULL, _FDEV_SETUP_READ);
