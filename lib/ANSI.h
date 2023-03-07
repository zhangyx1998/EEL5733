/**
 * EEL5733 Assignments
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief ANSI Escape Codes
 */
#ifndef H
#define H

#define ANSI_ESCAPE(CODE) "\e[" CODE "m"

#define RESET			ANSI_ESCAPE("0")

#define S_BOLD(S)		ANSI_ESCAPE("1") S ANSI_ESCAPE("22")
#define S_DIM(S)		ANSI_ESCAPE("2") S ANSI_ESCAPE("22")
#define S_ITALIC(S)		ANSI_ESCAPE("3") S ANSI_ESCAPE("23")
#define S_UNDERLINE(S)	ANSI_ESCAPE("4") S ANSI_ESCAPE("24")
#define S_BLINK(S)		ANSI_ESCAPE("5") S ANSI_ESCAPE("25")
#define S_INVERT(S)		ANSI_ESCAPE("7") S ANSI_ESCAPE("27")
#define S_INVISIBLE(S)	ANSI_ESCAPE("8") S ANSI_ESCAPE("28")
#define S_STRIKE(S)		ANSI_ESCAPE("9") S ANSI_ESCAPE("29")

#define C_GREY			ANSI_ESCAPE("30")
#define C_RED			ANSI_ESCAPE("31")
#define C_GREEN			ANSI_ESCAPE("32")
#define C_YELLOW		ANSI_ESCAPE("33")
#define C_BLUE			ANSI_ESCAPE("34")
#define C_PURPLE		ANSI_ESCAPE("35")
#define C_CYAN			ANSI_ESCAPE("36")
#define C_WHITE			ANSI_ESCAPE("37")

#define BG_BLACK		ANSI_ESCAPE("40")
#define BG_RED			ANSI_ESCAPE("41")
#define BG_GREEN		ANSI_ESCAPE("42")
#define BG_YELLOW		ANSI_ESCAPE("43")
#define BG_BLUE			ANSI_ESCAPE("44")
#define BG_PURPLE		ANSI_ESCAPE("45")
#define BG_CYAN			ANSI_ESCAPE("46")
#define BG_WHITE		ANSI_ESCAPE("47")

#define C_B_GREY		ANSI_ESCAPE("90")
#define C_B_RED			ANSI_ESCAPE("91")
#define C_B_GREEN		ANSI_ESCAPE("92")
#define C_B_YELLOW		ANSI_ESCAPE("93")
#define C_B_BLUE		ANSI_ESCAPE("94")
#define C_B_PURPLE		ANSI_ESCAPE("95")
#define C_B_CYAN		ANSI_ESCAPE("96")
#define C_B_WHITE		ANSI_ESCAPE("97")

#define BG_B_GRAY		ANSI_ESCAPE("100")
#define BG_B_RED		ANSI_ESCAPE("101")
#define BG_B_GREEN		ANSI_ESCAPE("102")
#define BG_B_YELLOW		ANSI_ESCAPE("103")
#define BG_B_BLUE		ANSI_ESCAPE("104")
#define BG_B_PURPLE		ANSI_ESCAPE("105")
#define BG_B_CYAN		ANSI_ESCAPE("106")
#define BG_B_WHITE		ANSI_ESCAPE("107")

#endif
