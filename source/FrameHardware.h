#pragma once

#include <algorithm>

#include "base/vector.h"
#include "base/Ptr.h"

#include "Config.h"

//forward declaration
class FrameCell;

class FrameHardware {

private:

  FrameCell &cell;

  const size_t facing;

  bool inbox_active;

  double stockpile_reserve;

  emp::Ptr<emp::vector<bool>> repr_pause_cur;
  emp::Ptr<emp::vector<bool>> repr_pause_nxt;

  Config::hardware_t cpu;

  size_t msg_dir;

public:

  FrameHardware(
      FrameCell &cell_,
      const size_t facing_,
      emp::Random &local_rng_,
      const Config &cfg,
      const Config::inst_lib_t &inst_lib,
      const Config::event_lib_t &event_lib
    ) : cell(cell_)
      , facing(facing_)
      , inbox_active(false)
      , stockpile_reserve(0)
      , repr_pause_cur(emp::NewPtr<emp::vector<bool>>(cfg.NLEV()+1))
      , repr_pause_nxt(emp::NewPtr<emp::vector<bool>>(cfg.NLEV()+1, true))
      , cpu(inst_lib, event_lib, &local_rng_)
    { cpu.PushTrait(this); }

  ~FrameHardware() {
    repr_pause_cur.Delete();
    repr_pause_nxt.Delete();
  }

  FrameCell& Cell() { return cell; }

  void Reset() {
    inbox_active = false;
    stockpile_reserve = 0;
    std::fill(repr_pause_cur->begin(), repr_pause_cur->end(), false);
    std::fill(repr_pause_nxt->begin(), repr_pause_nxt->end(), true);

    cpu.ResetHardware();
    cpu.ResetProgram();
    emp_assert(!cpu.GetProgram().GetSize());
  }

  double CheckStockpileReserve() const { return stockpile_reserve; }

  void AdjustStockpileReserve(const double amt) {
    stockpile_reserve = std::max(0.0,stockpile_reserve+amt);
  }

  void SetupCompute() {
    std::fill(repr_pause_cur->begin(), repr_pause_cur->end(), false);
    std::swap(repr_pause_cur, repr_pause_nxt);
    cpu.SpawnCore(
      Config::hardware_t::affinity_t(),
      0.5,
      {},
      false
    );
  }

  void StepProcess() { cpu.SingleProcess(); }

  void SetProgram(const Config::program_t & program) {
    cpu.SetProgram(program);
  }

  void PauseRepr(const size_t lev) { (*repr_pause_nxt)[lev] = true; }

  bool IsReprPaused(const size_t lev) const {
    return repr_pause_cur->at(lev) || repr_pause_nxt->at(lev);
  }

  size_t GetFacing() const {
    return facing;
  }

  size_t GetMsgDir() const {
    return msg_dir;
  }

  void SetMsgDir(const size_t new_dir) {
    msg_dir = new_dir;
  }

  void SetInboxActivity(const bool state) {
    inbox_active = state;
  }

  bool CheckInboxActivity() const {
    return inbox_active;
  }

  void QueueMessage(const Config::event_t &event) {
    cpu.QueueEvent(event);
  }

  void QueueMessages(Config::inbox_t &inbox) {
    while(inbox_active && !inbox.empty()) {
      QueueMessage(inbox.front());
      inbox.pop_front();
    }
    // clear inactive inboxes, too!
    inbox.clear();
  }

};
