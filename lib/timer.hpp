//
// Created by newap on 3/27/2020.
//

#ifndef HW_ARCHIVER_LIB_TIMER_HPP_
#define HW_ARCHIVER_LIB_TIMER_HPP_

#include <chrono>

/**
 * Class for measuring time
 */
class Timer {
 public:
  /**
   * Default constructor
   */
  Timer() :
      m_beg(clock_::now()) {
  }

  /**
   * Resets the timer
   */
  void reset() {
    m_beg = clock_::now();
  }

  /**
   * Counts and returns elapsed time
   * @return elapsed time
   */
  [[nodiscard]] double elapsed() const {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(
        clock_::now() - m_beg).count();
  }

 private:
  /**
   * Typedef for clock
   */
  typedef std::chrono::high_resolution_clock clock_;

  /**
   * Start time
   */
  std::chrono::time_point<clock_> m_beg;
};

#endif //HW_ARCHIVER_LIB_TIMER_HPP_
