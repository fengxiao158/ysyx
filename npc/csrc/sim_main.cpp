#include <verilated.h>
#include "../obj_dir/Vtop.h"
#include <verilated_vcd_c.h>
#include <cstdlib>
#include <ctime>

int main(int argc,char *argv[])
{
    Vtop *top=new Vtop;
    srand(time(0));
    Verilated::traceEverOn(true);
    VerilatedVcdC *tfp=new VerilatedVcdC;
    top->trace(tfp,10);
    tfp->open("waves.vcd");

    for (int i=0;i<10;i++)
    {
        top->a=rand()%2;
        top->b=rand()%2;
        top->eval();
        tfp->dump(i);
    }
    tfp->flush();
    tfp->close();
    delete tfp;
    delete top;
    
    return 0;
}
