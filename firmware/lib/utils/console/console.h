
#ifndef CONSOLE_H_
#define CONSOLE_H_

    #ifndef CONSOLE_LINE_LENGTH
        #define CONSOLE_LINE_LENGTH 30
    #endif
    
    #ifndef CONSOLE_LINE_CHARACTER
        #define CONSOLE_LINE_CHARACTER '#'
    #endif
	
	#include <stdio.h>
	#include <stdint.h>
	
	void console_newline(void);
	void console_line(unsigned char length);
	void console_clear(void);

#endif /* CONSOLE_H_ */