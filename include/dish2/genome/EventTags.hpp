#pragma once
#ifndef DISH2_GENOME_EVENTTAGS_HPP_INCLUDE
#define DISH2_GENOME_EVENTTAGS_HPP_INCLUDE

namespace dish2 {

template<typename Spec>
struct EventTags {

  bool operator==(const EventTags& other) const { return true; }


  template<typename Archive> void serialize( Archive & ar ) { /*ar(
  );*/ }


};

} // namespace dish2

#endif // #ifndef DISH2_GENOME_EVENTTAGS_HPP_INCLUDE
