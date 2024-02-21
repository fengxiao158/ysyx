module=top

all:
	@echo "Write this Makefile by your self."

waves_sim:sim
	@./obj_dir/Vtop
	@gtkwave waves.vcd
	@echo "show waves"

sim:V$(module) sim_main
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	@echo "Write this Makefile by your self."
	@make -C obj_dir -f Vtop.mk Vtop

V$(module):vsrc/$(module).v
	@echo "generate Vtops"
	@verilator -Wall --trace -cc vsrc/$(module).v 

sim_main:vsrc/$(module).v csrc/sim_main.cpp
	@echo "generate Vtop.mk"
	@verilator -Wall --trace -cc vsrc/$(module).v --exe csrc/sim_main.cpp

.PHONY:clean
clean:
	@rm -rf obj_dir waves.vcd
	@echo "clean all"

include ../Makefile
