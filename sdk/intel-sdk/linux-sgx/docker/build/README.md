# Build and run with Docker

Files in this directory demonstrate how to build and install the SGX SDK and PSW, and run SGX applications in Docker containers.

## Quick Start

###  Prerequisites
1. Install [Docker and Compose](https://docs.docker.com/) and configure them properly following their respective installation guide.
2. Install [SGX Flexible Launch Control driver](https://github.com/intel/SGXDataCenterAttestationPrimitives/tree/master/driver/linux). **Note**: See below to run with the Legacy Launch Control driver.
3. In the root directory of this repo, prepare source code and download prebuilt binaries:
```
$ make preparation
```

### Run with Docker Compose
This will start AESM and an SGX sample on one terminal using docker-compose.
```
$ ./build_compose_run.sh
```

### Run with Docker directly

Alternatively, you can run AESM and SGX sample containers in two separate terminals.

In one terminal,
```
$ ./build_and_run_aesm_docker.sh
```
In another terminal,
```
$ ./build_and_run_sample_docker.sh
```

## Dockerfile

The Dockerfile specifies 3 image build targets:
1. builder: Builds PSW and SDK bin installers from source. This requires downloading the prebuilt AEs and optimized libs from 01.org.
2. aesm: Takes the PSW installer from builder to install and run the AESM daemon.
3. sample: Installs the SDK installer from builder, then builds and runs the SampleEnclave app

- [build_and_run_aesm_docker.sh](./build_and_run_aesm_docker.sh): Shows how to build and run the AESM image in Docker. This will start the AESM service listening to a named socket, located in /var/run/aesmd in the container and mounted in Docker volume aesmd-socket.

- [build_and_run_sample_docker.sh](./build_and_run_sample_docker.sh): Shows how to build and run the SampleEnclave app inside a Docker container with a locally built SGX sample image.

## Legacy Launch Control driver and kernel for SGX

All SGX applications need access to the SGX device nodes exposed by the kernel space driver. Depending on the driver or kernel you are using, the SGX device nodes may have different names and locations. Therefore, you need to ensure those nodes are mapped and mounted inside the containers properly.


[SGX kernel patches](https://github.com/jsakkine-intel/linux-sgx/commits/master) are still in process of upstreaming.
The [Flexible Launch Control driver](https://github.com/intel/SGXDataCenterAttestationPrimitives/tree/master/driver) is developed to imitate the kernel patches as closely as possible.

The sample scripts and Compose files are compatible with the Flexible Launch Control driver or a custom built kernel with SGX support. If you need to use the Legacy Launch Control driver then you need to make following modifications:
1. Replace "/dev/sgx/enclave" device with "/dev/isgx" and **remove** "/dev/sgx/provision" device for AESM in docker-compose.yml and build_and_run_aesm_docker.sh
2. Replace "/dev/sgx/enclave" with "/dev/isgx" for the sample container in docker-compose.yml and build_and_run_sample_docker.sh

**Note**: When you switch between drivers, make sure you uninstall the previous driver and reboot the system before installing the other one.

**Note**: Earlier versions of the Flexible Launch Control driver and kernel patches may expose the SGX device as a single node at "/dev/sgx".
