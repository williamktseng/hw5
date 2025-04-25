#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#endif

#include "schedwork.h"
using namespace std;

static bool scheduleHelper(const AvailabilityMatrix& avail,
                           size_t dailyNeed, size_t maxShifts,
                           DailySchedule& sched,
                           vector<size_t>& counts,
                           size_t day);
static bool fillSlots(const AvailabilityMatrix& avail,
                      size_t dailyNeed, size_t maxShifts,
                      DailySchedule& sched,
                      vector<size_t>& counts,
                      size_t day, size_t slot);

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    size_t n = avail.size();
    if (n == 0) return false;
    sched.assign(n, vector<Worker_T>(dailyNeed));
    vector<size_t> counts(avail[0].size(), 0);
    return scheduleHelper(avail, dailyNeed, maxShifts, sched, counts, 0);
}

static bool scheduleHelper(const AvailabilityMatrix& avail,
                           size_t dailyNeed, size_t maxShifts,
                           DailySchedule& sched,
                           vector<size_t>& counts,
                           size_t day)
{
    if (day == avail.size()) {
        return true;
    }
    return fillSlots(avail, dailyNeed, maxShifts, sched, counts, day, 0);
}

static bool fillSlots(const AvailabilityMatrix& avail,
                      size_t dailyNeed, size_t maxShifts,
                      DailySchedule& sched,
                      vector<size_t>& counts,
                      size_t day, size_t slot)
{
    if (slot == dailyNeed) {
        return scheduleHelper(avail, dailyNeed, maxShifts, sched, counts, day + 1);
    }
    size_t k = avail[day].size();
    for (Worker_T w = 0; w < k; ++w) {
        if (avail[day][w] && counts[w] < maxShifts) {
            bool duplicate = false;
            for (size_t i = 0; i < slot; ++i) {
                if (sched[day][i] == w) {
                    duplicate = true;
                    break;
                }
            }
            if (duplicate) continue;
            sched[day][slot] = w;
            counts[w]++;
            if (fillSlots(avail, dailyNeed, maxShifts, sched, counts, day, slot + 1)) {
                return true;
            }
            counts[w]--;
        }
    }
    return false;
}