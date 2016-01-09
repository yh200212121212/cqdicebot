#include <time.h>
#include <stdint.h>
#include <math.h>

typedef int32_t MS_INT;

class MersenneTwister
{
public:
	void rseed(MS_INT seed) {
		if (isInitialized) {
			return;
		}
		msInit(seed);
	}
	int rand(void) {
		if (isInitialized == false) {
			return 0;
		}
		return msRand();
	}
	MersenneTwister(int seed) :isInitialized(0) {
		rseed(seed);
	}
	~MersenneTwister() {

	}
	void msInit(int seed) {
		MS_INT i;
		MS_INT p;
		idx = 0;
		MT[0] = seed;
		for (i = 1; i < 624; ++i) { /* loop over each other element */
			p = 1812433253 * (MT[i - 1] ^ (MT[i - 1] >> 30)) + i;
			MT[i] = p & 0xffffffff; /* get last 32 bits of p */
		}
		isInitialized = 1;
	}
	MS_INT msRand() {
		if (!isInitialized)
			msInit((MS_INT)time(NULL));

		if (idx == 0)
			msGenerate();

		MS_INT y = MT[idx];
		y = y ^ (y >> 11);
		y = y ^ ((y << 7) & 2636928640);
		y = y ^ ((y << 15) & 4022730752);
		y = y ^ (y >> 18);

		idx = (idx + 1) % 624; /* increment idx mod 624*/
		return y;
	}
	void msGenerate() {
		MS_INT i, y;
		for (i = 0; i < 624; ++i) {
			y = (MT[i] & 0x80000000) +
				(MT[(i + 1) % 624] & 0x7fffffff);
			MT[i] = MT[(i + 397) % 624] ^ (y >> 1);
			if (y % 2) { /* y is odd */
				MT[i] = MT[i] ^ (2567483615);
			}
		}
	}
private:
	MS_INT MT[624];
	MS_INT idx;
	MS_INT isInitialized;
};
