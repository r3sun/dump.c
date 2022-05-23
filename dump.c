#include <stdio.h>
#include <string.h>


#if _INTEGRAL_MAX_BITS > 32
	#define PROCESSOR_SIZE 64
	#define MINUS_1 0x8000000000000000
	#define MAX_INT 0xffffffffffffffff
#else
	#define PROCESSOR_SIZE 32
	#define MINUS_1 0x80000000
	#define MAX_INT 0xffffffff
#endif

char *bin, bytbuf[12], buf[PROCESSOR_SIZE + 1], prpbuf[81];
const char nuremals[] = "0123456789ABCDEF";

/* revers s */
void reverse(char *s)
{
	int c;
	char *p, *p1;

	p = s;

	for (; *s; *s++) ;
	--s;

	for (p, s; p < s; *p++, *s--) {
		c = *p;
		*p = *s;
		*s = c;
	}
}


/* checks whether the input string is valid or not */
int isvalid (int c, int base)
{
	char *cond;

	char bins[] = "01";
	char octs[] = "01234567";

	char *p;

	switch(base) {
		case 2: p = bins; break;
		case 8: p = octs; break;
		case 10: return 1; /* integers can be ASCII character so if integers always return 1 */
		case 16: p = nuremals; break;
		default: break;
	}
	if ((cond = strchr (p, c)) == NULL) {
		fprintf (stderr, "%c not a standard %d base symbole. It has been ignored\n", c, base);
		return 0;
	}
	return 1;
}

/* prepare the number in s in a fancy way and put it in t */
void prepnum (char *s, char *t, int obase)
{
	int bitgrp, bbitgrp, i, l;
	char *p, sprtr;

	switch(obase) {
		case 2: bitgrp = 4; bbitgrp = 8; sprtr = ' '; break;
		case 8: bitgrp = 3; sprtr = ' '; break;
		case 10: bitgrp = 3; sprtr = ','; break;
		case 16: bitgrp = 4; sprtr = ' '; break;
		default: break;
	}
	if (obase == 2) {
		reverse (s);
		while ((l = strlen(s)) && ( l %= bbitgrp)) {
			strcat(s, "0");
		}
		reverse (s);
	}
	p = t;

	reverse (s);
	while (*s != '\0') {
		for (i = 0; i < bitgrp; i++) {
			if (*s) {
				*p++ = *s++;
			} else {
				*p++ = '0';
			}
		}
		*p++ = sprtr;
	}
	*--p = '\0';
	reverse (t);
}


/* char string s to numeral of base */
long long chartoint (char *s, int base)
{
	long long d = 0;
	for(; *s; *s++) {
		*s = toupper(*s);
		if (isvalid(*s, base)) {
			d = base * d + (atobase (*s, base));
		}
	}
	return d;
}

/* convert character into integers */
int atobase (int c, int base)
{
	int i;
	i = 0;
	if (base != 10) {
		while (nuremals[i] != c) i++;
		return i;
	} else return c - '0';	/* if integer return the int value as they can be ASCII character */
}

/* intobase: int n is converted into the the numeral system defined by base and put into buffer */
void intobase (long long n, int base, char *buffer)
{
	char *p;
	p = buffer;

	if (n == 0)
		*buffer++ = '0';

	for (n; n != 0; n /= base) {
		*buffer++ = nuremals[(int)(n%base)];
	}
	*buffer = '\0';
	reverse (p);
}

void convert(long long n, int base, char *buffer)
{
	intobase (n, base, prpbuf);
	prepnum (prpbuf, buffer, base);
}


int main (int argc, char *argv[])
{
	int c, i, l, nflag, fflag, vflag, cond, ibase, obase;
	long long idata;
	FILE *fp;
	char *cp, *istr, *p;
	char num_nm[][3] = {				/* prefixe of different numerals */
						"", "", "bin", "",
						"", "", "", "",
						"oct", "", "int", "",
						"", "", "", "",
						"hex", NULL
						};

	int bases[] = {2, 8, 10, 16};
	idata = 0;
	i = nflag = fflag = vflag = ibase = obase = 0;
	l = 8;

	/* loop whrough argv */
	while (*++argv) {
		cp = *argv;
		/* determine operation behaviour  */
		while (*cp) {
			switch(*cp) {
				case '-':
					break;
				case 'b':
					ibase = 2;
					switch(*++cp) {
						case 'i':
							obase = 10;
							break;
						case 'h':
							obase = 16;
							break;
						case 'o':
							obase = 8;
							break;
						default:
							cp--;
							break;
					}
					break;
				case 'i':
					ibase = 10;
					switch(*++cp) {
						case 'b':
							obase = 2;
							break;
						case 'h':
							obase = 16;
							break;
						case 'o':
							obase = 8;
							break;
						default:
							cp--;
							break;
					}
					break;
				case 'h':
					ibase = 16;
					switch(*++cp) {
						case 'b':
							obase = 2;
							break;
						case 'i':
							obase = 10;
							break;
						case 'o':
							obase = 8;
							break;
						default:
							cp--;
							break;
					}
					break;
				case 'o':
					ibase = 8;
					switch(*++cp) {
						case 'b':
							obase = 2;
							break;
						case 'i':
							obase = 10;
							break;
						case 'h':
							obase = 16;
							break;
						default:
							cp--;
							break;
					}
					break;
				case 'f':				/* the next arg is the file that contains the input */
					fflag++;
					istr = *++argv;
					break;
				case 'l':				/* the length of number on each line */
					l = chartoint (*++argv, 10);
					break;
				case 'n':				/* the next arg is the input number */
					nflag++;
					istr = *++argv;
					break;
				case 'v':				/* verbose */
					vflag++;
					break;
				default:
					printf("\ndump: \'%c\' is not a key\n", *cp);
					break;
			}
			cp++;
		}
	}

	if (nflag) {						/* the next argument is the input number */
		idata = chartoint (istr, ibase);
		if (obase && ibase) {
			convert (idata, obase, buf);
			if(vflag)
				printf ("%s: ", num_nm[obase]);
			printf ("%s\n", buf);
		} else if (ibase) {
			for (i = 0; i <= 4; i++) {
				obase = bases[i];
				if (obase != ibase) {
					convert (idata, obase, buf);
					if(vflag)
						printf ("%s: ", num_nm[obase]);
					printf ("%s\n", buf);
				}
			}
		} else {
			printf ("dump: an input base is required\n");
			return -1;
		}
	} else if (fflag) {					/* the next argument is a file containing input number */
		fp = fopen (istr, "r");
		if (fp != NULL) {
			i = 0;
			if (obase == 0)
				obase = 2;
			if (ibase == 10) {
				while ((c = getc (fp)) != EOF) {
					if ((i % l) == 0 && vflag) {
						convert(i, 16, buf);
						printf("\n%s\t", buf);
					} else if ((i % l) == 0)
						putchar ('\n');

					convert (c, obase, buf);
					printf ("%s ", buf);
					i++;
				}
				if (vflag)
					printf ("\n\nnumber of bytes read: %10d\t", i);
			} else {
				printf("dump: input base %d is not valid\n", ibase);
				return -1;
			}
		} else {
			printf ("dump: unable to open file %s", istr);
			return -1;
		}
	}
	return 0;
}