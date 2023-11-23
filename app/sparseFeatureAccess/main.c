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

#include <sgx_urts.h>
#include "Enclave/encl_u.h"
#include <sys/mman.h>
#include <signal.h>
#include "libsgxstep/enclave.h"
#include "libsgxstep/debug.h"
#include "libsgxstep/pt.h"
#include <math.h>



void *a_pt;
int fault_fired = 0, aep_fired = 0;

void aep_cb_func(void)
{
    gprsgx_region_t gprsgx;
    uint64_t erip = edbgrd_erip() - (uint64_t) get_enclave_base();
    info("Hello world from AEP callback with erip=%#llx! Resuming enclave..", erip); 

    edbgrd(get_enclave_ssa_gprsgx_adrs(), &gprsgx, sizeof(gprsgx_region_t));
    dump_gprsgx_region(&gprsgx);

    aep_fired++;
}

void fault_handler(int signal)
{
	info("Caught fault %d! Restoring access rights..", signal);
    ASSERT(!mprotect(a_pt, 4096, PROT_READ | PROT_WRITE));
    print_pte_adrs(a_pt);
    fault_fired++;
}


#include <stdbool.h>
#include <stdint.h>

#define INTEL_MSR_VOLTAGE_VERSION "1.0.0"
#define INTEL_MSR_VOLTAGE_CONFIGURATION_FILE_PATH "/etc/intel-msr-voltages.conf"

bool is_intel_cpu();

bool msr_tools_exists();

int userNum = 1000;
int factor = 200;
void initialiseEmBeddingTable(double t[][factor]);

int main( int argc, char **argv )
{
    sgx_launch_token_t token = {0};
	uint64_t retval = 0, pre_retval=0, updated = 0;
    char old = 0x00, new = 0xbb;
    sgx_enclave_id_t eid = 0;

    // access sparse feature embedding table - outside sgx
	double embeddingT[userNum][factor];
	initialiseEmBeddingTable(embeddingT); // construct embedding table
	// for (int i=0;i<1000;i++){
	// 	for(int j=0;j<200;j++){
	// 		info("value:%d",&embeddingT[i][j]);
	// 	}
	// }

    
   	info("Creating enclave...");
   	sgx_status_t ret = SGX_ERROR_UNEXPECTED;
   	ret = sgx_create_enclave( "./Enclave/encl.so", /*debug=*/1,
   	                                    &token, &updated, &eid, NULL );
   	if (ret != SGX_SUCCESS) {
   		printf("Error code is 0x%X. Please refer to the \"Intel SGX SDK Developer Reference\" for more details.\n", ret);
   	}
    printf("enclave creating status:0x%X.\n", ret);

    // register_aep_cb(aep_cb_func);
    // print_enclave_info();

    info("reading/writing debug enclave memory..");
    SGX_ASSERT(get_a_addr(eid, &a_pt));
    edbgrd(a_pt, &old, 1);
    edbgwr(a_pt, &new, 1);
    edbgrd(a_pt, &new, 1);
    info("a at %p: old=0x%x; new=0x%x", a_pt, old & 0xff, new & 0xff);

    // mprotect to provoke page faults during enclaved execution 
    info("revoking a access rights..");
    print_pte_adrs(a_pt);
    ASSERT(!mprotect(a_pt, 4096, PROT_NONE));
    print_pte_adrs(a_pt);
    ASSERT(signal(SIGSEGV, fault_handler) != SIG_ERR);

    info("calling enclave..");
    SGX_ASSERT( enclave_dummy_call(eid, &retval) );

    info("calling multiplication..");

    multiplication(eid, &retval);

	info("result in decimal:%"PRIu64"\n", retval);
   	SGX_ASSERT( sgx_destroy_enclave( eid ) );
    info("all is well; exiting..");
	return 0;
}

void initialiseEmBeddingTable(double t[][factor]){
	
    // for(size_t i=0;i<userNum;i++){
    //     t[i] = new double[factor];
    // }

    for(int i=0;i<userNum;i++){
        for(int j=0;j<factor;j++){
            t[i][j] = 0.1 * (rand() / (RAND_MAX + 1.0)) / sqrt(factor);
        }
    }
}

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <float.h>
#include <cpuid.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <getopt.h>

bool is_intel_cpu() {
	char* cpu_signature = malloc(13 * sizeof(char*));
	*(cpu_signature+12) = '\0';		//set null character as end of CPU signature
	bool is_intel;
	unsigned int eax = 0;

	unsigned int cpuid_success = __get_cpuid((unsigned int)0x0000,
											&eax,
											(unsigned int*)cpu_signature,
											(unsigned int*)(cpu_signature+8),
											(unsigned int*)(cpu_signature+4));

	if (!cpuid_success) {
		fprintf(stderr, "intel-msr-voltages: error: cpuid instruction failed!");
		is_intel = false;
	}
	else {
		is_intel = (strcmp(cpu_signature, "GenuineIntel") == 0);
	}

	//free allocated variables
	free(cpu_signature);
	return is_intel;
}

bool msr_tools_exists() {
	char* current_line = malloc(1024 * sizeof(char *));
	bool wrmsr_found;
	bool rdmsr_found;
	int wrmsr_which_popen_exit_code;
	int rdmsr_which_popen_exit_code;

	FILE* which_wrmsr_output = popen("which wrmsr", "r");
	while (fgets(current_line, 1024, which_wrmsr_output) != NULL) {}
	wrmsr_which_popen_exit_code = WEXITSTATUS(pclose(which_wrmsr_output));
	wrmsr_found = (wrmsr_which_popen_exit_code == 0);

	FILE* which_rdmsr_output = popen("which rdmsr", "r");
	while (fgets(current_line, 1024, which_rdmsr_output) != NULL) {}
	rdmsr_which_popen_exit_code = WEXITSTATUS(pclose(which_rdmsr_output));
	rdmsr_found = (rdmsr_which_popen_exit_code == 0);

	//free allocated variables
	free(current_line);

	return (wrmsr_found && rdmsr_found);
}