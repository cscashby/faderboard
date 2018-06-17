/*============================================================================*/
/* Convert unsigned long value to d-digit decimal string in local buffer      */
/*============================================================================*/
char *u2s(unsigned long x,unsigned d)
{  static char b[16];
   char *p;
   unsigned digits = 0;
   unsigned long t = x;

   do ++digits; while (t /= 10);
   // if (digits > d) d = digits; // uncomment to allow more digits than spec'd
   *(p = b + d) = '\0';
   do *--p = x % 10 + '0'; while (x /= 10);
   while (p != b) *--p = ' ';
   return b;
}

