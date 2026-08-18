#define _WIN32_WINNT 0x0501

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iphlpapi.h>

#define SLEEP(x) Sleep(x)
#define CLEAR "cls"

void bar(float x)
{
    int n = (int)(x * 30 / 100);

    if (n < 0) n = 0;
    if (n > 30) n = 30;

    printf("[");
    for (int i = 0; i < 30; i++)
        printf(i < n ? "#" : ".");
    printf("] %5.1f%%\n", x);
}

unsigned long long fileTimeValue(FILETIME t)
{
    return ((unsigned long long)t.dwHighDateTime << 32) |
           t.dwLowDateTime;
}

float getCPU(unsigned long long *oldIdle,
             unsigned long long *oldTotal)
{
    FILETIME idle, kernel, user;

    if (!GetSystemTimes(&idle, &kernel, &user))
        return 0;

    unsigned long long i = fileTimeValue(idle);
    unsigned long long k = fileTimeValue(kernel);
    unsigned long long u = fileTimeValue(user);
    unsigned long long total = k + u;

    if (*oldTotal == 0)
    {
        *oldIdle = i;
        *oldTotal = total;
        return 0;
    }

    unsigned long long idleDiff = i - *oldIdle;
    unsigned long long totalDiff = total - *oldTotal;

    *oldIdle = i;
    *oldTotal = total;

    if (totalDiff == 0)
        return 0;

    float cpu = 100.0f *
        (1.0f - (float)idleDiff / (float)totalDiff);

    if (cpu < 0) cpu = 0;
    if (cpu > 100) cpu = 100;

    return cpu;
}

float getRAM(void)
{
    MEMORYSTATUSEX memory = {0};
    memory.dwLength = sizeof(memory);

    if (!GlobalMemoryStatusEx(&memory))
        return 0;

    return (float)memory.dwMemoryLoad;
}

unsigned long long getNetwork(void)
{
    DWORD size = 0;

    if (GetIfTable(NULL, &size, FALSE) != ERROR_INSUFFICIENT_BUFFER)
        return 0;

    MIB_IFTABLE *table = malloc(size);

    if (!table)
        return 0;

    if (GetIfTable(table, &size, FALSE) != NO_ERROR)
    {
        free(table);
        return 0;
    }

    unsigned long long total = 0;

    for (DWORD i = 0; i < table->dwNumEntries; i++)
    {
        total += table->table[i].dwInOctets;
        total += table->table[i].dwOutOctets;
    }

    free(table);
    return total;
}

void getProcessor(char *name, DWORD size)
{
    HKEY key;
    DWORD type = REG_SZ;

    if (RegOpenKeyExA(
        HKEY_LOCAL_MACHINE,
        "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
        0,
        KEY_READ,
        &key) == ERROR_SUCCESS)
    {
        RegQueryValueExA(
            key,
            "ProcessorNameString",
            NULL,
            &type,
            (LPBYTE)name,
            &size
        );

        RegCloseKey(key);
    }
    else
    {
        snprintf(name, size, "Procesador desconocido");
    }
}

int main(void)
{
    int opcion;

    printf("\n");
    printf("==============================\n");
    printf("   BETTER WITH ALGORITHMS\n");
    printf("==============================\n");
    printf("  1. Iniciar monitor\n");
    printf("  2. Salir\n");
    printf("==============================\n");
    printf("  Opcion: ");

    scanf("%d", &opcion);

    if (opcion != 1)
        return 0;

    char processor[256] = "Procesador desconocido";
    getProcessor(processor, sizeof(processor));

    SYSTEM_INFO info;
    GetSystemInfo(&info);

    unsigned long long oldIdle = 0;
    unsigned long long oldTotal = 0;
    unsigned long long oldNetwork = getNetwork();

    float cpu = 0;
    float ram = 0;
    float network = 0;

    MEMORYSTATUSEX memory = {0};
    memory.dwLength = sizeof(memory);
    GlobalMemoryStatusEx(&memory);

    float totalRAM =
        (float)memory.ullTotalPhys / 1024 / 1024 / 1024;

    float avgCPU = 0;
    float avgRAM = 0;
    float minCPU = 100;
    float maxCPU = 0;

    unsigned long samples = 0;

    SLEEP(1000);

    while (1)
    {
        system(CLEAR);

        float newCPU = getCPU(&oldIdle, &oldTotal);
        float newRAM = getRAM();

        unsigned long long newNetwork = getNetwork();

        if (newNetwork >= oldNetwork)
            network = (float)(newNetwork - oldNetwork)
                    / 1024.0f / 1024.0f;
        else
            network = 0;

        oldNetwork = newNetwork;

        cpu = cpu * 0.70f + newCPU * 0.30f;
        ram = ram * 0.80f + newRAM * 0.20f;

        samples++;

        avgCPU += cpu;
        avgRAM += ram;

        if (cpu < minCPU)
            minCPU = cpu;

        if (cpu > maxCPU)
            maxCPU = cpu;

        printf("================================================\n");
        printf("       BETTER WITH ALGORITHMS\n");
        printf("          SYSTEM MONITOR\n");
        printf("================================================\n\n");

        printf("PROCESADOR\n");
        printf("  %s\n", processor);
        printf("  Nucleos: %lu\n\n",
               (unsigned long)info.dwNumberOfProcessors);

        printf("CPU\n");
        bar(cpu);
        printf("  Uso actual: %.1f%%\n\n", cpu);

        printf("RAM\n");
        bar(ram);
        printf("  Uso actual : %.1f%%\n", ram);
        printf("  RAM total  : %.2f GB\n", totalRAM);
        printf("  RAM usada  : %.2f GB\n",
               totalRAM * ram / 100.0f);
        printf("  RAM libre  : %.2f GB\n\n",
               totalRAM - totalRAM * ram / 100.0f);

        printf("RED\n");
        printf("  Trafico: %.2f MB/s\n\n", network);

        printf("ESTADISTICAS CPU\n");
        printf("  Promedio: %.1f%%\n", avgCPU / samples);
        printf("  Minimo  : %.1f%%\n", minCPU);
        printf("  Maximo  : %.1f%%\n\n", maxCPU);

        printf("ESTADO\n");

        if (cpu >= 85 || ram >= 90)
            printf("  [!] CRITICO - Alto consumo\n");
        else if (cpu >= 65 || ram >= 75)
            printf("  [!] ADVERTENCIA - Carga moderada\n");
        else
            printf("  [+] OK - Sistema estable\n");

        printf("\n================================================\n");
        printf("Muestras: %lu | Actualizacion: 1 segundo\n",
               samples);
        printf("Ctrl+C para salir\n");

        SLEEP(1000);
    }

    return 0;
}
