
#include <iostream>
#include "Commands.h"	
#include "Utilities.h"
#include <tabulate/table.hpp>  // https://github.com/p-ranav/tabulate
#include "Process.h"

void PrintGPULayout();
void PrintProcessLayout();

class DummyProcess {       // The class
public:             // Access specifier
    int pid;        // Attribute (int variable)
    std::string processName;  // Attribute (string variable)
    DummyProcess(int pid, std::string processName)
    {
        this->pid = pid;
        this->processName = processName;
    }
};

void ProcessSmi()
{
    PrintGPULayout();
    PrintProcessLayout();
}


void PrintProcessLayout()
{
    // Hardcoded Processes:
    DummyProcess p1(1002, "C:\\Windows\\System32\\explorer.exe");
    DummyProcess p2(2012, "C:\\Windows\\System32\\cmd.exe");
    DummyProcess p3(4999, "C:\\Windows\\System32\\notepad.exe");
    DummyProcess p4(3923, "C:\\Windows\\System32\\taskmgr.exe");
    DummyProcess p5(5107, "C:\\Windows\\System32\\virus69.exe");
    DummyProcess printableProcesses[5] = { p1,p2,p3,p4,p5 };


    tabulate::Table processSummary;

	// Add Header Rows
    processSummary.add_row({ "Processes:","\u00A0","\u00A0","\u00A0" });
    processSummary.add_row({ "PID","Type","Process name", "GPU Memory Usage" });

    // Format Header Rows
    processSummary[0].format().border_left(" ").border_right(" ");
    processSummary[0][0].format().border_left("|");
    processSummary[0][3].format().border_right("|");

    processSummary[1].format().border_left(" ").border_right(" ").border_top("").corner_top_left("");
    processSummary[1][0].format().border_left("|");
    processSummary[1][3].format().border_right("|");

    
    // Contain in For Loop for adding each process
    int i = 0;
    for (DummyProcess process : printableProcesses)
    {
        processSummary.add_row({ std::to_string(process.pid), "C+G", process.processName, "N/A"});
        processSummary[2+i].format().border_left(" ").border_right(" ");
        if (i != 0) {
            processSummary[2 + i].format().border_top("").corner_top_left("");
        }
        else
        {
            processSummary[2 + i].format().border_top("=").corner_top_left("+");
        }
        processSummary[2+i][0].format().border_left("|");
        processSummary[2+i][3].format().border_right("|");
        
        i++;
    }

    // Make width uniform
    processSummary.column(0).format().width(12).font_align(tabulate::FontAlign::right);
    processSummary.column(1).format().width(7).font_align(tabulate::FontAlign::right);
    processSummary.column(2).format().width(60);
    processSummary.column(3).format().width(20).font_align(tabulate::FontAlign::right);


    std::cout << processSummary << std::endl;
}



void PrintGPULayout()
{
    tabulate::Table gpuSummary;

    // Add & Format First Row
    gpuSummary.add_row({ "CSOPESY-SMI 101.11", "Driver Ver. 101.11", "CUDA Ver. 25.5" });

    gpuSummary[0].format().border_left(" ").border_right(" ");
    gpuSummary[0][0].format().border_left("|");
    gpuSummary[0][1].format().font_align(tabulate::FontAlign::right);
    gpuSummary[0][2].format().border_right("|").font_align(tabulate::FontAlign::right);
    

    // Add & Format Second Row (Nested Table)
    tabulate::Table gpuFan;
    gpuFan.add_row({ "GPU\nFan", "Name\nTemp", "\nPerf", "\u00A0           \u00A0", "TCC/WDDM\nPwr:Usage/Cap" }).format().hide_border();
    gpuFan.column(4).format().font_align(tabulate::FontAlign::right);

    tabulate::Table gpuBus;
    gpuBus.add_row({ "Bus-Id","\u00A0", "Disp.A" }).format().hide_border();
    gpuBus.add_row({ "\u00A0","\u00A0", "Memory-Usage" });
    gpuBus.column(2).format().font_align(tabulate::FontAlign::right);

    tabulate::Table gpuManu;
    gpuManu.add_row({ "Volatile\nGPU-Util","\u00A0", "Uncorr. Ecc\nCompute M." }).format().hide_border();
    gpuManu.add_row({ "\u00A0","\u00A0","MIG M." });
    gpuManu.column(2).format().font_align(tabulate::FontAlign::right);

    gpuSummary.add_row({ gpuFan, gpuBus, gpuManu }); //Add Second Row

    // Add & Format Third Row (Nested Table)
    tabulate::Table gpuStats1;
    gpuStats1.add_row({ "\u00A0 0", "Nvidia", "Geforce", "GTX 1080", "WDDM" }).format().hide_border();
    gpuStats1.add_row({ "28%","37C","P8","\u00A0         \u00A0", "11W / 180W" });
    gpuStats1.column(0).format().font_align(tabulate::FontAlign::right);
    gpuStats1.column(4).format().font_align(tabulate::FontAlign::right);

    tabulate::Table gpuStats2;
    gpuStats2.add_row({ "0000000:26:00.0","On" }).format().hide_border();
    gpuStats2.add_row({ "701MiB /", "8192MiB" });
    gpuStats2.column(1).format().font_align(tabulate::FontAlign::right);

    tabulate::Table gpuStats3;
    gpuStats3.add_row({ "\u00A0          \u00A0","\u00A0", "N/A" }).format().hide_border();
    gpuStats3.add_row({ "\u00A0 0%","\u00A0", "Default" });
    gpuStats3.add_row({ "\u00A0          \u00A0","\u00A0", "N/A" });
    gpuStats3.column(1).format().font_align(tabulate::FontAlign::right);
    gpuStats3.column(2).format().font_align(tabulate::FontAlign::right);

    gpuSummary.add_row({ gpuStats1, gpuStats2, gpuStats3 }).format().border_top("="); //Add Third Row

    // Make widths uniform
    gpuSummary.column(0).format().width(48);
    gpuSummary.column(1).format().width(26);
    gpuSummary.column(2).format().width(26);

    std::cout << gpuSummary << std::endl << std::endl;
}

