//
// Created by newap on 3/27/2020.
//

#ifndef HW_ARCHIVER_LIB_TIMER_H_
#define HW_ARCHIVER_LIB_TIMER_H_

#include <chrono>

class Timer {
 public:
  Timer() :
      m_beg(clock_::now()) {
  }

  void reset() {
    m_beg = clock_::now();
  }

  [[nodiscard]] double elapsed() const {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(
        clock_::now() - m_beg).count();
  }

 private:
  typedef std::chrono::high_resolution_clock clock_;
  std::chrono::time_point<clock_> m_beg;
};

#endif //HW_ARCHIVER_LIB_TIMER_H_
