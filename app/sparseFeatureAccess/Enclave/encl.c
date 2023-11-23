/*
 *  This file is part of the SGX-Step enclave execution control framework.
 *
 *  Copyright (C) 2017 Jo Van Bulck <jo.vanbulck@cs.kuleuven.be>,
 *                     Raoul Strackx <raoul.strackx@cs.kuleuven.be>
 *
 *  SGX-Step is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  SGX-Step is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with SGX-Step. If not, see <http://www.gnu.org/licenses/>.
 */

__attribute__((aligned(4096))) int array[4096] = {0xaa};
#define a array[0]
#include <inttypes.h>
#include <stdio.h>
#include <unistd.h>

void* get_a_addr( void )
{
    return &a;
}

int enclave_dummy_call(void)
{
    return a;
}

// uint64_t exp_func(int, int);
uint64_t sma(uint64_t, uint64_t);

int produceInterferenceArray(double interference[10], double index);

uint64_t multiplication(void){
    /*
    uint64_t multiplier = 0x1122334455667788;
    uint64_t var = 0XAE0000 * multiplier;
    //uint64_t multiplier = 0X18;
    //unsigned long long var = 0xdeadbeef * multiplier;
    //uint64_t var = 0XAE0000 * multiplier;
    uint64_t index = 0;
    while (var == 0XAE0000 * multiplier && index<10000000)
    {
        var = 0XAE0000;
        var *= multiplier;
        index++;
    }
    // printf("index:%llu", index);
    // info("result:%llu", var);
    return var;
    */
    uint64_t a;
    uint64_t b;
    a = 5;
    b = 8;
    // printf("We will calculate a^b");
    // printf("a=",a);
    // printf("b=",b);
    // printf("==== Calculation ====");
    uint64_t res = sma(a, b);//exp_func(a,b);
    // printf("Result:%" PRIu64 "\n",res);
    // printf(")===========");
    return res;
}

// cache structure
struct cachedEmbeddingT{
    double originIndex;
	double sparseFeatureArr[200];
	double accessFrequency;
}
// embedding table in cache
cachedEmbeddingT embeddingT[100];

double[] safeAccessEmbeddingTable( double index ){
    bool incache = false;
	for(int i=0;i<100;i++){
		if(embeddingT[i].originIndex==index){
			incache = true;
			embeddingT[i].accessFrequency++;

			// every time after the frequency update, need resort the cache
			// TODO

			return embeddingT[i].sparseFeatureArr;
		}
	}
	if(!incache){
		// if not in cache, need to load from external embedding table, and replace the last one in cache
		double interferenceArray[10];
		int realIndex = produceInterferenceArray(interferenceArray, index);
        double newLoadedArray[10][200] = loadFromExternal( interferenceArray );
		double sparseFeatureToCache[200] = newLoadedArray[realIndex];
		embeddingT[99].originIndex = index;
		embeddingT[99].sparseFeatureArr = sparseFeatureToCache;
		embeddingT[99].accessFrequency = 1;
		return sparseFeatureToCache;
	}
	return embeddingT[0].sparseFeatureArr;
}

int produceInterferenceArray(double interference[10], double index){
	int realIndex = rand()%10;
	for(int i=0;i<realIndex;i++){
		interference[i] = rand()%1000;
	}
	interference[realIndex] = index;
	for(int i=realIndex+1;i<10;i++){
		interference[i] = rand()%1000;
	}
	return realIndex;
}

uint64_t sma( uint64_t x,  uint64_t H){
	uint64_t h;
	uint64_t r;
	int bin[32];
	int i;
	r = x;
	i = 0;
	/* Converts H in Binary */
	while( H > 0 ){
		if (H % 2 == 0){
			bin[i] = 0;
		}else{
			bin[i] = 1;
		}
		H = H/2;
		i++;
	}
	i--; //t-1
	while(i>0){
		r = r * r;
		if( bin[--i] == 1 ){
			r = r * x;
		}
	}
	return r;
}

/*
long sma( long x,  long H,  long n){
	long h;
	unsigned long long r;
	int bin[32];
	int i;
	r = x;
	i = 0;
	// Converts H in Binary 
	while( H > 0 ){
		if (H % 2 == 0){
			bin[i] = 0;
		}else{
			bin[i] = 1;
		}
		H = H/2;
		i++;
	}
	i--; //t-1
	while(i>0){
		r = (r * r) % n;
		if( bin[--i] == 1 ){
			r = (r * x) % n;
		}
	}
	return r;
}
*/


/*
uint64_t exp_func(int x, int y){
    int exp = bin(y);
    printf("Binary value of b is:",exp);
    printf("Bit\tResult");
    int value = x;
 
    for(int i=3;i<len(exp);i++)
    {
        value = value * value;
        printf(i-1,":\t",value,"(square)");
        if(exp[i:i+1]=='1')
        {
            value = value*x;
        }
        printf(i-1,":\t",value,"(multiply)");
    }
    return value;
}
*/

