#pragma once

template<typename HARDWARE_T, typename INSTRUCTION_T>
void Inst_Loop(HARDWARE_T & hw, const INSTRUCTION_T & inst) {
  auto & call_state = hw.GetCurThread().GetExecState().GetTopCallState();
  auto & mem_state = call_state.GetMemory();
  size_t cur_ip = call_state.GetIP();
  const size_t cur_mp = call_state.GetMP();
  emp_assert(cur_ip > 0);
  // CurIP is the next instruction (not the one currently executing)
  // Because IP gets incremented before execution, cur_ip should never be 0.
  const size_t bob = cur_ip - 1;
  const size_t eob = hw.FindEndOfBlock(cur_mp, cur_ip);
  const bool skip = false;
  if (skip) {
    // Skip to EOB
    call_state.SetIP(eob);
    // Advance past the block close if not at end of module.
    if (hw.IsValidProgramPosition(cur_mp, eob)) ++call_state.IP();
  } else {
    // Open flow
    emp_assert(cur_mp < hw.GetProgram().GetSize());
    hw.GetFlowHandler().OpenFlow(
      hw,
      {
        sgp::lsgp_utils::FlowType::WHILE_LOOP,
        cur_mp,
        cur_ip,
        bob,
        eob
      },
      hw.GetCurThread().GetExecState()
    );
  }
}
