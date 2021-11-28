#include "rand.h"

int initialize = 0;
int r[1000];
int t = 0;

uint64 rand() {
    int i;

    if (initialize == 0) {
        r[0] = SEED;
        for (i = 1; i < 31; i++) {
            r[i] = (16807LL * r[i - 1]) % 2147483647;
            if (r[i] < 0) {
                r[i] += 2147483647;
            }
        }
        for (i = 31; i < 34; i++) {
            r[i] = r[i - 31];
            r[i] = r[i - 31] + r[i - 3];
        }

		initialize = 1;
    }
    
	//t = t % 656;
    t = t%32;
    r[t + 344] = r[t + 344 - 31] + r[t + 344 - 3];
    
	t++;
    //for(int i = 0; i < 1000; i++)printk("%ld,", r[t] % 10 );
    //return (uint64)r[t - 1 + 344] % 10 + 1;
    return (uint64)r[t]%10 + 1;
}
