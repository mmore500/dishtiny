#pragma once
#ifndef DISH_MANAGER_MANAGERFAMILY_HPP_INCLUDE
#define DISH_MANAGER_MANAGERFAMILY_HPP_INCLUDE

#include <unordered_set>

namespace dish {

class ManagerFamily {

private:
  emp::vector<size_t> cell_gen;
  size_t parent_pos;
  size_t birth_update;
  std::unordered_set<size_t> child_pos;
  Config::chanid_t prev_chan;

public:
  ManagerFamily(const Config &cfg)
  : cell_gen(cfg.NLEV() + 1, 0)
  , parent_pos(0)
  , birth_update(0)
  , prev_chan(0)
  { ; }

  void Reset(const size_t cur_update) {
    birth_update = cur_update;
    child_pos.clear();
    std::fill(
      std::begin(cell_gen),
      std::end(cell_gen),
      0
    );
  }

  size_t GetBirthUpdate() const { return birth_update; }

  size_t GetCellAge(const size_t cur_update) {
    emp_assert(cur_update >= birth_update);
    return cur_update - birth_update;
  };

  bool IsParentPos(const size_t pos) const { return pos == parent_pos; }

  size_t GetParentPos() const { return parent_pos; }

  bool HasChildPos(const size_t pos) const {
    return child_pos.find(pos) != child_pos.end();
  }

  Config::chanid_t GetPrevChan() const { return prev_chan; }

  void SetPrevChan(const Config::chanid_t chan) { prev_chan = chan; }

  void SetParentPos(const size_t pos) { parent_pos = pos; }

  void AddChildPos(const size_t pos) { child_pos.insert(pos); }

  void SetCellGen(const emp::vector<size_t>& gen, const size_t replev) {
    cell_gen = gen;
    for (size_t lev = 0; lev <= replev; ++lev) ++cell_gen[lev];
  }

  const emp::vector<size_t> & GetCellGen() const { return cell_gen; }
};

} // namespace dish

#endif // #ifndef DISH_MANAGER_MANAGERFAMILY_HPP_INCLUDE
