// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtop.h for the primary calling header

#include "verilated.h"

#include "Vtop___024root.h"

void Vtop___024root___eval_act(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_act\n"); );
}

extern const VlUnpacked<CData/*3:0*/, 16> Vtop__ConstPool__TABLE_hd79fb729_0;

VL_INLINE_OPT void Vtop___024root___nba_sequent__TOP__0(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__0\n"); );
    // Init
    CData/*3:0*/ __Vtableidx1;
    __Vtableidx1 = 0;
    // Body
    if (vlSelf->rst) {
        if ((0x2faf080U > vlSelf->water_light__DOT__count)) {
            vlSelf->water_light__DOT__count = ((IData)(1U) 
                                               + vlSelf->water_light__DOT__count);
        } else {
            vlSelf->water_light__DOT__led_count = (0xfU 
                                                   & ((IData)(1U) 
                                                      + (IData)(vlSelf->water_light__DOT__led_count)));
            vlSelf->water_light__DOT__count = 0U;
            vlSelf->led = ((8U & (IData)(vlSelf->water_light__DOT__cur_sta))
                            ? ((4U & (IData)(vlSelf->water_light__DOT__cur_sta))
                                ? ((2U & (IData)(vlSelf->water_light__DOT__cur_sta))
                                    ? ((1U & (IData)(vlSelf->water_light__DOT__cur_sta))
                                        ? 0x8000U : 0x4000U)
                                    : ((1U & (IData)(vlSelf->water_light__DOT__cur_sta))
                                        ? 0x2000U : 0x1000U))
                                : ((2U & (IData)(vlSelf->water_light__DOT__cur_sta))
                                    ? ((1U & (IData)(vlSelf->water_light__DOT__cur_sta))
                                        ? 0x800U : 0x400U)
                                    : ((1U & (IData)(vlSelf->water_light__DOT__cur_sta))
                                        ? 0x200U : 0x100U)))
                            : ((4U & (IData)(vlSelf->water_light__DOT__cur_sta))
                                ? ((2U & (IData)(vlSelf->water_light__DOT__cur_sta))
                                    ? ((1U & (IData)(vlSelf->water_light__DOT__cur_sta))
                                        ? 0x80U : 0x40U)
                                    : ((1U & (IData)(vlSelf->water_light__DOT__cur_sta))
                                        ? 0x20U : 0x10U))
                                : ((2U & (IData)(vlSelf->water_light__DOT__cur_sta))
                                    ? ((1U & (IData)(vlSelf->water_light__DOT__cur_sta))
                                        ? 8U : 4U) : 
                                   ((1U & (IData)(vlSelf->water_light__DOT__cur_sta))
                                     ? 2U : 1U))));
        }
        vlSelf->water_light__DOT__cur_sta = vlSelf->water_light__DOT__nxt_sta;
    } else {
        vlSelf->water_light__DOT__led_count = 0U;
        vlSelf->water_light__DOT__count = 0U;
        vlSelf->water_light__DOT__cur_sta = 0U;
    }
    __Vtableidx1 = vlSelf->water_light__DOT__led_count;
    vlSelf->water_light__DOT__nxt_sta = Vtop__ConstPool__TABLE_hd79fb729_0
        [__Vtableidx1];
}

void Vtop___024root___eval_nba(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_nba\n"); );
    // Body
    if (vlSelf->__VnbaTriggered.at(0U)) {
        Vtop___024root___nba_sequent__TOP__0(vlSelf);
        vlSelf->__Vm_traceActivity[1U] = 1U;
    }
}

void Vtop___024root___eval_triggers__act(Vtop___024root* vlSelf);
#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__act(Vtop___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__nba(Vtop___024root* vlSelf);
#endif  // VL_DEBUG

void Vtop___024root___eval(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval\n"); );
    // Init
    VlTriggerVec<1> __VpreTriggered;
    IData/*31:0*/ __VnbaIterCount;
    CData/*0:0*/ __VnbaContinue;
    // Body
    __VnbaIterCount = 0U;
    __VnbaContinue = 1U;
    while (__VnbaContinue) {
        __VnbaContinue = 0U;
        vlSelf->__VnbaTriggered.clear();
        vlSelf->__VactIterCount = 0U;
        vlSelf->__VactContinue = 1U;
        while (vlSelf->__VactContinue) {
            vlSelf->__VactContinue = 0U;
            Vtop___024root___eval_triggers__act(vlSelf);
            if (vlSelf->__VactTriggered.any()) {
                vlSelf->__VactContinue = 1U;
                if (VL_UNLIKELY((0x64U < vlSelf->__VactIterCount))) {
#ifdef VL_DEBUG
                    Vtop___024root___dump_triggers__act(vlSelf);
#endif
                    VL_FATAL_MT("vsrc/top.v", 1, "", "Active region did not converge.");
                }
                vlSelf->__VactIterCount = ((IData)(1U) 
                                           + vlSelf->__VactIterCount);
                __VpreTriggered.andNot(vlSelf->__VactTriggered, vlSelf->__VnbaTriggered);
                vlSelf->__VnbaTriggered.set(vlSelf->__VactTriggered);
                Vtop___024root___eval_act(vlSelf);
            }
        }
        if (vlSelf->__VnbaTriggered.any()) {
            __VnbaContinue = 1U;
            if (VL_UNLIKELY((0x64U < __VnbaIterCount))) {
#ifdef VL_DEBUG
                Vtop___024root___dump_triggers__nba(vlSelf);
#endif
                VL_FATAL_MT("vsrc/top.v", 1, "", "NBA region did not converge.");
            }
            __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
            Vtop___024root___eval_nba(vlSelf);
        }
    }
}

#ifdef VL_DEBUG
void Vtop___024root___eval_debug_assertions(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((vlSelf->clk & 0xfeU))) {
        Verilated::overWidthError("clk");}
    if (VL_UNLIKELY((vlSelf->rst & 0xfeU))) {
        Verilated::overWidthError("rst");}
}
#endif  // VL_DEBUG
