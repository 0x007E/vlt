
#include "console.h"

void console_newline(void)
{
	putchar('\n');
	putchar('\r');
}

void console_line(unsigned char length)
{
	console_newline();

	for (unsigned char i=0; i < length; i++)
	{
		putchar(CONSOLE_LINE_CHARACTER);
	}
	
	console_newline();
}

void console_clear(void)
{
	puts("\e[1;1H\e[2J");
}
