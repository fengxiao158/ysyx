// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vtop__Syms.h"


VL_ATTR_COLD void Vtop___024root__trace_init_sub__TOP__0(Vtop___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root__trace_init_sub__TOP__0\n"); );
    // Init
    const int c = vlSymsp->__Vm_baseCode;
    // Body
    tracep->declBit(c+5,"clk", false,-1);
    tracep->declBit(c+6,"rst", false,-1);
    tracep->declBus(c+7,"led", false,-1, 15,0);
    tracep->pushNamePrefix("water_light ");
    tracep->declBit(c+5,"clk", false,-1);
    tracep->declBit(c+6,"rst", false,-1);
    tracep->declBus(c+7,"led", false,-1, 15,0);
    tracep->declBus(c+1,"count", false,-1, 31,0);
    tracep->declBus(c+2,"cur_sta", false,-1, 3,0);
    tracep->declBus(c+3,"nxt_sta", false,-1, 3,0);
    tracep->declBus(c+4,"led_count", false,-1, 3,0);
    tracep->declBus(c+8,"led0", false,-1, 3,0);
    tracep->declBus(c+9,"led1", false,-1, 3,0);
    tracep->declBus(c+10,"led2", false,-1, 3,0);
    tracep->declBus(c+11,"led3", false,-1, 3,0);
    tracep->declBus(c+12,"led4", false,-1, 3,0);
    tracep->declBus(c+13,"led5", false,-1, 3,0);
    tracep->declBus(c+14,"led6", false,-1, 3,0);
    tracep->declBus(c+15,"led7", false,-1, 3,0);
    tracep->declBus(c+16,"led8", false,-1, 3,0);
    tracep->declBus(c+17,"led9", false,-1, 3,0);
    tracep->declBus(c+18,"led10", false,-1, 3,0);
    tracep->declBus(c+19,"led11", false,-1, 3,0);
    tracep->declBus(c+20,"led12", false,-1, 3,0);
    tracep->declBus(c+21,"led13", false,-1, 3,0);
    tracep->declBus(c+22,"led14", false,-1, 3,0);
    tracep->declBus(c+23,"led15", false,-1, 3,0);
    tracep->popNamePrefix(1);
}

VL_ATTR_COLD void Vtop___024root__trace_init_top(Vtop___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root__trace_init_top\n"); );
    // Body
    Vtop___024root__trace_init_sub__TOP__0(vlSelf, tracep);
}

VL_ATTR_COLD void Vtop___024root__trace_full_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
void Vtop___024root__trace_chg_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
void Vtop___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/);

VL_ATTR_COLD void Vtop___024root__trace_register(Vtop___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root__trace_register\n"); );
    // Body
    tracep->addFullCb(&Vtop___024root__trace_full_top_0, vlSelf);
    tracep->addChgCb(&Vtop___024root__trace_chg_top_0, vlSelf);
    tracep->addCleanupCb(&Vtop___024root__trace_cleanup, vlSelf);
}

VL_ATTR_COLD void Vtop___024root__trace_full_sub_0(Vtop___024root* vlSelf, VerilatedVcd::Buffer* bufp);

VL_ATTR_COLD void Vtop___024root__trace_full_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root__trace_full_top_0\n"); );
    // Init
    Vtop___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vtop___024root*>(voidSelf);
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    Vtop___024root__trace_full_sub_0((&vlSymsp->TOP), bufp);
}

VL_ATTR_COLD void Vtop___024root__trace_full_sub_0(Vtop___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root__trace_full_sub_0\n"); );
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode);
    // Body
    bufp->fullIData(oldp+1,(vlSelf->water_light__DOT__count),32);
    bufp->fullCData(oldp+2,(vlSelf->water_light__DOT__cur_sta),4);
    bufp->fullCData(oldp+3,(vlSelf->water_light__DOT__nxt_sta),4);
    bufp->fullCData(oldp+4,(vlSelf->water_light__DOT__led_count),4);
    bufp->fullBit(oldp+5,(vlSelf->clk));
    bufp->fullBit(oldp+6,(vlSelf->rst));
    bufp->fullSData(oldp+7,(vlSelf->led),16);
    bufp->fullCData(oldp+8,(0U),4);
    bufp->fullCData(oldp+9,(1U),4);
    bufp->fullCData(oldp+10,(2U),4);
    bufp->fullCData(oldp+11,(3U),4);
    bufp->fullCData(oldp+12,(4U),4);
    bufp->fullCData(oldp+13,(5U),4);
    bufp->fullCData(oldp+14,(6U),4);
    bufp->fullCData(oldp+15,(7U),4);
    bufp->fullCData(oldp+16,(8U),4);
    bufp->fullCData(oldp+17,(9U),4);
    bufp->fullCData(oldp+18,(0xaU),4);
    bufp->fullCData(oldp+19,(0xbU),4);
    bufp->fullCData(oldp+20,(0xcU),4);
    bufp->fullCData(oldp+21,(0xdU),4);
    bufp->fullCData(oldp+22,(0xeU),4);
    bufp->fullCData(oldp+23,(0xfU),4);
}
