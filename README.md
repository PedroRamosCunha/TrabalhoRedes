<h1>Trabalho de Redes 1</h1>
<h4>Gabriela Barion Vidal - 4277535 <br> Rodrigo Bragato Piva - 10684573 <br> Pedro Ramos Cunha - 10892248</h4>
<h3>Sobre o projeto</h3>
<p>
O Grupo Zenith de sondas é uma extracurricular famosa da Escola de Engenharia de <br>
de São Carlos (EESC). Como parte de suas responsabilidades, temos a prospecção de<br>
dados meteorológicos para estudos de clima e outras informações. Isso é feito a<br>
partir do envio de sondas meteorológicas, que nada mais são que sistemas embarcados<br>
que enviam sinais em um ritmo constante para a base.</p>
<p>
Para tanto, é necessário que haja um gerenciamento, pois o grupo lança inúmeras <br>
sondas, simultâneamente, para que se tenha dados de várias partes da cidade e, <br>
assim, modelos mais precisos.</p>

<h3>Ambiente de Desenvolvimento do Código (IDE)</h3>
<p>
O códgigo foi desenvolvido no ambiente de programação <b>Visual Studio Code</b><br>
A partir do desenvolvimento síncrono do código através da extenção <em>Live Shar</em><br>

</p>
<h3>Ambiente de compilação e execução do programa</h3>

<p>
O programa 
</p>
<h3>Dados do Sistema Operacional</h3>
    ramos@Pedro:~$ lsb_release -a
    No LSB modules are available.
    Distributor ID:	Ubuntu
    Description:	Ubuntu 20.04.1 LTS
    Release:	20.04
    Codename:	focal


<h3>Dados da máquina</h3>

    ramos@Pedro:~$ lscpu
    Architecture:                    x86_64
    CPU op-mode(s):                  32-bit, 64-bit
    Byte Order:                      Little Endian
    Address sizes:                   48 bits physical, 48 bits virtual
    CPU(s):                          4
    On-line CPU(s) list:             0-3
    Thread(s) per core:              2
    Core(s) per socket:              2
    Socket(s):                       1
    NUMA node(s):                    1
    Vendor ID:                       AuthenticAMD
    CPU family:                      21
    Model:                           101
    Model name:                      AMD A10-9600P RADEON R5, 10 COMPUTE CORES 4C+6G
    Stepping:                        1
    Frequency boost:                 enabled
    CPU MHz:                         1284.038
    CPU max MHz:                     2400.0000
    CPU min MHz:                     1200.0000
    BogoMIPS:                        4791.03
    Virtualisation:                  AMD-V
    L1d cache:                       64 KiB
    L1i cache:                       192 KiB
    L2 cache:                        2 MiB
    NUMA node0 CPU(s):               0-3


    ramos@Pedro:~$sudo lshw -class CPU    
    *-cpu                     
       description: CPU
       product: AMD A10-9600P RADEON R5, 10 COMPUTE CORES 4C+6G
       vendor: Advanced Micro Devices [AMD]
       physical id: 4
       bus info: cpu@0
       version: AMD A10-9600P RADEON R5, 10 COMPUTE CORES 4C+6G
       serial: Unknown
       slot: Socket FP4
       size: 1325MHz
       capacity: 2400MHz
       width: 64 bits
       clock: 100MHz
       capabilities: lm fpu fpu_exception wp vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush mmx fxsr sse sse2 ht syscall nx mmxext fxsr_opt pdpe1gb rdtscp x86-64 constant_tsc rep_good acc_power nopl nonstop_tsc cpuid extd_apicid aperfmperf pni pclmulqdq monitor ssse3 fma cx16 sse4_1 sse4_2 movbe popcnt aes xsave avx f16c lahf_lm cmp_legacy svm extapic cr8_legacy abm sse4a misalignsse 3dnowprefetch osvw ibs xop skinit wdt lwp fma4 tce nodeid_msr tbm topoext perfctr_core perfctr_nb bpext ptsc mwaitx cpb hw_pstate ssbd vmmcall fsgsbase bmi1 avx2 smep bmi2 xsaveopt arat npt lbrv svm_lock nrip_save tsc_scale vmcb_clean flushbyasid decodeassists pausefilter pfthreshold avic v_vmsave_vmload vgif overflow_recov cpufreq
       configuration: cores=4 enabledcores=4 threads=4

    

    

