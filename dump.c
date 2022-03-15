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


/* checks whether the inputed string is valid or not */
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


void prepnum (char *s, char *t, int obase)
{
	int bitgrp, i, l;
	char *p, sprtr;

	switch(obase) {
		case 2: bitgrp = 4; sprtr = ' '; break;
		case 8: bitgrp = 3; sprtr = ' '; break;
		case 10: bitgrp = 3; sprtr = ','; break;
		case 16: bitgrp = 4; sprtr = ' '; break;
		default: break;
	}
	if (obase == 2) {
		reverse (s);
		while ((l = strlen(s)) && ( l %= bitgrp)) {
			strcat(s, "0");
		}
		reverse (s);
	}
	p = t;

	reverse (s);
	while (*s != '\0') {
		for (i = 0; i < bitgrp && *s != '\0'; i++) {
			*p++ = *s++;
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

	for (n; n != 0; n /= base) {
		*buffer++ = nuremals[(int)(n%base)];
	}
	*buffer = '\0';
	reverse (p);
}
int main (int argc, char *argv[])
{
	int c, aflag, fflag, cond, ibase, obase;
	FILE *fp;
	char *cp, *istr, *p, *numnm = (char *) malloc (sizeof(char) * 4);
	long long idata = 0;
	aflag = fflag = 0;

	for (cp = *++argv; *cp; *cp++) {
		switch(*cp) {
			case 'b':
				ibase = 2;
				switch(*++cp) {
					case 'i':
						obase = 10;
						numnm = "int";
						break;
					case 'h':
						obase = 16;
						numnm = "hex";
						break;
					case 'o':
						obase = 8;
						numnm = "oct";
						break;
					default:
						obase = 0;
						break;
				}
				break;
			case 'i':
				ibase = 10;
				switch(*++cp) {
					case 'b':
						obase = 2;
						numnm = "bin";
						break;
					case 'h':
						obase = 16;
						numnm = "hex";
						break;
					case 'o':
						obase = 8;
						numnm = "oct";
						break;
					default:
						obase = 0;
						break;
				}
				break;
			case 'h':
				ibase = 16;
				switch(*++cp) {
					case 'b':
						obase = 2;
						numnm = "bin";
						break;
					case 'i':
						obase = 10;
						numnm = "int";
						break;
					case 'o':
						obase = 8;
						numnm = "oct";
						break;
					default:
						obase = 0;
						break;
				}
				break;
			case 'o':
				ibase = 8;
				switch(*++cp) {
					case 'b':
						obase = 2;
						numnm = "bin";
						break;
					case 'i':
						obase = 10;
						numnm = "int";
						break;
					case 'h':
						obase = 16;
						numnm = "hex";
						break;
					default:
						obase = 0;
						break;
				}
				break;
			case 'f':
				fflag++;
				istr = *++argv;
				break;
			case 'a':
				aflag++;
				istr = *++argv;
				break;
		}
	}

	if (aflag) {
		idata = chartoint (istr, ibase);
		if (ibase == 10) {
			if (obase) {
				intobase (idata, obase, buf);
				prepnum (buf, prpbuf, obase);
				printf ("%s: %s\n", numnm, prpbuf);
			} else {
				obase = 2;
				intobase (idata, obase, buf);
				prepnum (buf, prpbuf, obase);
				printf ("bin: %s\n", prpbuf);
				obase = 8;
				intobase (idata, obase, buf);
				prepnum (buf, prpbuf, obase);
				printf ("oct: %s\n", prpbuf);
				obase = 16;
				intobase (idata, obase, buf);
				prepnum (buf, prpbuf, obase);
				printf ("hex: %s\n", prpbuf);
			}
		} else if (ibase == 2) {
			if (obase) {
				intobase (idata, obase, buf);
				prepnum (buf, prpbuf, obase);
				printf ("%s: %s\n", numnm, prpbuf);
			} else {
				obase = 8;
				intobase (idata, obase, buf);
				prepnum (buf, prpbuf, obase);
				printf ("oct: %s\n", prpbuf);
				obase = 10;
				intobase (idata, obase, buf);
				prepnum (buf, prpbuf, obase);
				printf ("int: %s\n", prpbuf);
				obase = 16;
				intobase (idata, obase, buf);
				prepnum (buf, prpbuf, obase);
				printf ("hex: %s\n", prpbuf);
			}
		}else if (ibase == 8) {
			if (obase) {
				intobase (idata, obase, buf);
				prepnum (buf, prpbuf, obase);
				printf ("%s: %s\n", numnm, prpbuf);
			} else {
				obase = 2;
				intobase (idata, obase, buf);
				prepnum (buf, prpbuf, obase);
				printf ("bin: %s\n", prpbuf);
				obase = 10;
				intobase (idata, obase, buf);
				prepnum (buf, prpbuf, obase);
				printf ("int: %s\n", prpbuf);
				obase = 16;
				intobase (idata, obase, buf);
				prepnum (buf, prpbuf, obase);
				printf ("hex: %s\n", prpbuf);
			}
		} else if (ibase == 16) {
			if (obase) {
				intobase (idata, obase, buf);
				prepnum (buf, prpbuf, obase);
				printf ("%s: %s\n", numnm, prpbuf);
			} else {
				obase = 2;
				intobase (idata, obase, buf);
				prepnum (buf, prpbuf, obase);
				printf ("bin: %s\n", prpbuf);
				obase = 8;
				intobase (idata, obase, buf);
				prepnum (buf, prpbuf, obase);
				printf ("oct: %s\n", prpbuf);
				obase = 10;
				intobase (idata, obase, buf);
				prepnum (buf, prpbuf, obase);
				printf ("int: %s\n", prpbuf);
			}
		} 
	} else if (fflag) {
		fp = fopen (istr, "r");
		if (fp != NULL) {
			if (ibase == 10) {
				if (obase == 2) {
					while ((c = getc (fp)) != EOF) {
						intobase (c, obase, buf);
						prepnum (buf, prpbuf, obase);
						printf ("%s ", prpbuf);
					}
				} else if (obase == 8) {
					while ((c = getc (fp)) != EOF) {
						intobase (c, obase, buf);
						prepnum (buf, prpbuf, obase);
						printf ("%s ", prpbuf);
					}
				} else if (obase == 16) {
					while ((c = getc (fp)) != EOF) {
						intobase (c, obase, buf);
						prepnum (buf, prpbuf, obase);
						printf ("%s ", prpbuf);
					}
				}
			}
		} else {
			printf ("unable to open file %s", istr);
		}
	}
}