#include "modlib.h"

static u32bits redspace[4*_WORDSIZE_];

#define MP_MULTIPLY(a,b,lowProd,highProd) \
{ \
	register u32bits othermixProd, mixProd, lowWordA, lowWordB, highWordA, highWordB; \
	lowWordA = (a)& 0xffff; \
	lowWordB = (b)& 0xffff; \
	highWordA = (a) >> 0x10; \
	highWordB = (b) >> 0x10; \
	lowProd = lowWordA * lowWordB; \
	highProd = highWordA * highWordB; \
	mixProd = lowWordB * highWordA; \
	othermixProd = lowWordA * highWordB; \
	mixProd += othermixProd; \
if (mixProd < othermixProd) highProd += 0x10000; \
	othermixProd = mixProd >> 16; \
	highProd += othermixProd; \
	mixProd <<= 0x10; \
	lowProd += mixProd; \
if (lowProd < mixProd) highProd++; \
}


void modredu(u32bits *result, u32bits *A, u32bits *modulus, int length, int n )
	{
		register u32bits lo, hi, carry, w;
		int i, j, l, extra, prec, flag;
		u32bits *r_sqr;
		register u32bits *r_ptr;
		u32bits *temp;
		u32bits b, div_b, z, b_t;
		int hit;
		u32bits thresh;
		u32bits T;
		int no;


		no = n;
		while (modulus[no - 1] == 0)
			no--;
		b = 0xffffffff; //4294967296.0;
		z = modulus[no - 1];
		b_t = z;
		div_b = 0xffffffff / z; //4294967296.0/z;


		r_sqr = A;
		temp = redspace;

		while (A[length] == 0)
			length--;

		flag = 0;
		extra = 0;
		j = length + 1 - no;
		r_ptr = r_sqr + j;

		while (j>0)
		{
			hit = 0;
			prec = 2;
			if (extra == 1)
			{
				if (r_ptr[no + extra - 1] != 0)
					flag = 1;
				else
					flag = 0;
			}
			if (flag == 0)
			{
				extra = 0;
				if (compar(modulus, r_ptr, no) >= 0)  // short compar( register u32bits *r1, register u32bits *r2, int precision);

				{
					extra = 1;
					j--;
					r_ptr--;
				}
			}
			while (hit == 0)
			{
				if (prec>2)
					T = T / 2;
				else
				{
					z = r_ptr[no - 1];
					thresh = z / b_t;
					if (extra)
					{
						z = r_ptr[no];
						z *= div_b;
						thresh += z;
						T = thresh;
						if (!T)
						{
							T = 0xffffffff;
						}
					}
					else
					{
						T = thresh;
					}



				} /* end of if then else */

				temp[no] = 0;
				carry = 0;
				for (i = 0; i<no; i++)
				{
					w = modulus[i];
					MP_MULTIPLY(T, w, lo, hi);
					lo += carry;
					if (lo <  carry)
					{
						hi++;
					}
					carry = hi;
					temp[i] = lo;
				} /* end for for i */


				temp[i] = carry;



				if (compar(r_ptr, temp, no + extra) >= 0)
				{
					hit = 1;
					intSub(r_ptr, temp, no + extra);  //int intSub(  register u32bits *r1, register u32bits *r2,  int precision);

					i = 0;
					l = no - 1 + extra;
					while (r_ptr[l] == 0)
					{
						i++;
						l--;
					}

					if (i>0)
					{
						i -= extra;
						j -= i;
						r_ptr = r_ptr - i;
					}
				} /*end of i r_sqr */
				else
					prec++;


			}  /* end of while hit */


		}  /* end of while j >=0 */

		while (compar(r_sqr, modulus, no + 1) >= 0)
		{
			hit = 0;
			prec = 2;
			while (hit == 0)
			{
				if (prec>2)
					T = T / 2;
				else
				{
					z = r_sqr[no - 1];
					thresh = z / b_t;
					z = r_sqr[no];
					z *= div_b;
					thresh += z;
					T = thresh;
					if (!T)
					{
						T = 0xffffffff;
					}

					else
					{
						T = thresh;
					}



				}   /* end of if-then else prec >3 */


				temp[no] = 0;
				carry = 0;
				for (i = 0; i<no; i++)   /* assumption that T<=b */
				{
					w = modulus[i];
					MP_MULTIPLY(T, w, lo, hi);
					lo += carry;
					if (lo <  carry)
						hi++;
					carry = hi;
					temp[i] = lo;

				} /* end for for i */


				temp[i] = carry;

				if (compar(r_sqr, temp, no + 1) >= 0)
				{
					hit = 1;
					intSub(r_sqr, temp, no + 1);   //int intSub(  register u32bits *r1, register u32bits *r2,  int precision);
				} /*  end of if */
				else
					prec++;

			}  /* end of while hit */

		}  /* end of while compar */

		for (i = 0; i <= n; i++)
			result[i] = r_sqr[i];
	}



	short compar(register u32bits *r1, register u32bits *r2, int precision)
	{

		r1 += precision - 1;
		r2 += precision - 1;

		do {
			if (*r1 < *r2)
				return -1;
			if (*(r1--) > *(r2--))
				return 1;
		} while (--precision);
		return 0;  /*  *r1 == *r2  */

	}


	/* result = r1-r2, this assumes r1 >= r2*/

	int intSub(register u32bits *r1, register u32bits *r2, int precision)
	{

		register u32bits x, R1, R2;
		int i = 0, borrow = 0;



		while (precision--) {
			R1 = r1[i];
			R2 = r2[i];
			if (borrow) {
				x = R1 - R2 - 1;
				borrow = (R1 <= R2);
			}
			else {
				x = R1 - R2;
				borrow = (R1 < R2);
			}
			r1[i] = x;
			i++;
		}
		return(borrow);    /* return the final carry/borrow flag bit */
	}