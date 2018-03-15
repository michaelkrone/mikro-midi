#pragma once

#include <ic/IoIc595.hpp>
#include <ic/IoIc4051.hpp>
#include <muxer/IoDemuxer595.hpp>
#include <muxer/IoMuxer4051.hpp>

#include "Command.h"

/*
  Init IC's
*/

// 4051's
// io::Ic4051 ic4051_0(IC_4051_0_S0, IC_4051_0_S1, IC_4051_0_S2, IC_4051_0_MUX, 5, 100);
// io::Ic4051 ic4051_1(IC_4051_0_S0, IC_4051_0_S1, IC_4051_0_S2, IC_4051_1_MUX, 2, 100);

// init 4051 muxers
// io::Muxer4051 muxer1(ic4051_0);
// io::Muxer4051 muxer2(ic4051_1);

// 595's and demuxers
io::Ic595 ic595_VuMeter(IC_595_0_SHCP, IC_595_0_STCP, IC_595_0_DS);
// io::Ic595 ic595_0_1(IC_595_0_SHCP, IC_595_0_STCP, IC_595_0_DS);

// demuxer0 -> Channel0
io::Demuxer595 demuxer_0_1(ic595_0_1);

/*
  Command LED's
*/

Commands<8> leds1 = {demuxer_0_1, {
  {mackie::Commands::Channel0_Button_Mute, PinLed_Channel0_Button_Mute},
  {mackie::Commands::Channel0_Button_Solo, PinLed_Channel0_Button_Solo},
  {mackie::Commands::Channel0_Button_Rec, PinLed_Channel0_Button_Rec},
  {mackie::Commands::Channel0_Button_Select, PinLed_Channel0_Button_Select},
  {mackie::Commands::Channel1_Button_Mute, PinLed_Channel1_Button_Mute},
  {mackie::Commands::Channel1_Button_Solo, PinLed_Channel1_Button_Solo},
  {mackie::Commands::Channel1_Button_Rec, PinLed_Channel1_Button_Rec},
  {mackie::Commands::Channel1_Button_Select, PinLed_Channel1_Button_Select}
}};

// template <uint8_t N>
// void update(const Commands<N> &cmds) {
//     for(uint8_t i = 0; i < cmds.length; i++) {
//       cmds.cmd[i];
//     }
// }
//
// update(leds1);

//
// /*
//   Command Buttons
// */
// static const uint8_t CommandButtonsLen = 4;
// //
// // CommandButton commandButtons[CommandButtonsLen] = {
// //   {mackie::Commands::Transport_Play, io::EventEmitter(io::BouncedInput(&muxer, 0))},
// //   {mackie::Commands::Transport_Stop, io::EventEmitter(io::BouncedInput(&muxer, 1))},
// //   {mackie::Commands::Channel0_Button_Solo, io::EventEmitter(io::BouncedInput(&muxer, 2))},
// //   {mackie::Commands::Channel0_Button_Mute, io::EventEmitter(io::BouncedInput(&muxer, 3))}
// // };
//
// io::EventEmitter b1 = io::EventEmitter(io::BouncedInput(&muxer, 0));
// io::EventEmitter b2 = io::EventEmitter(io::BouncedInput(&muxer, 1));
// io::EventEmitter b3 = io::EventEmitter(io::BouncedInput(&muxer, 2));
// io::EventEmitter b4 = io::EventEmitter(io::BouncedInput(30));
// // io::Input b4 = io::Input(30);
// void handler(int v, io::SignalEventType t) {
//   Serial.println(t == io::EventUp ? mackie::On : mackie::Off);
//   // mackie::protocol->sendCommand((uint8_t)arg, t == io::EventUp ? mackie::On : mackie::Off);
// }
//
// void setupHardware() {
//   // for (uint8_t i = 0; i < CommandButtonsLen; i++) {
//     // uint8_t cmd = commandButtons[i].cmd;
//     Serial.println("add listeners");
//     b1.addListener(io::EventDown, handler);
//
//     b2.addListener(io::EventDown, [](int v, io::SignalEventType t) {
//       Serial.println("2 down");
//     });
//
//     b3.addListener(io::EventDown, [](int v, io::SignalEventType t) {
//       Serial.println("3 down");
//     });
//
//     b4.addListener(io::EventCatchAll, [](int v, io::SignalEventType t) {
//       Serial.println(t);
//     });
//     // commandButtons[i].button.addListener(io::EventCatchAll, handler, cmd);
//   // }
// }
//
// void updateHardware() {
//   // if (changed[mackie::States::Command] == mackie::State::Changed) {
//   //   // uint16_t* cmds = mackie::protocol->getState(mackie::States::Command);
//   //   for (uint8_t i = 0; i < CommandOutputLen; i++) {
//   //     commandOutputState[i] = HIGH;//cmds[commandOutput[i].cmd];
//   //   }
//   // }
//   //
//   demuxer.writeAll(commandOutputState);
//
//   // for (uint8_t i = 0; i < CommandButtonsLen; i++) {
//     // commandButtons[i].button.update();
//     // }
//     b1.update();
//     b2.update();
//     b3.update();
//     b4.update();
//
// }
