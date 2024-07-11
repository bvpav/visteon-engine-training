FROM ghcr.io/texasinstruments/ubuntu-distro:latest

ENV TI_SDK_PREFIX /home/tisdk

WORKDIR $TI_SDK_PREFIX

RUN wget https://dr-download.ti.com/software-development/software-development-kit-sdk/MD-Snl3iJzGTW/09.02.00.05/ti-processor-sdk-linux-adas-j721s2-evm-09_02_00_05-Linux-x86-Install.bin && \
    chmod +x ti-processor-sdk-linux-adas-j721s2-evm-09_02_00_05-Linux-x86-Install.bin && \
    ./ti-processor-sdk-linux-adas-j721s2-evm-09_02_00_05-Linux-x86-Install.bin --mode unattended --prefix . ----debuglevel 4 && \
    rm -rf ti-processor-sdk-linux-adas-j721s2-evm-09_02_00_05-Linux-x86-Install.bin