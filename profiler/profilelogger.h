// profilelogger.h
// Gregory Rosenblatt
// Update 12/25/04

#ifndef Starscape_ProfileLogger_H_
#define Starscape_ProfileLogger_H_

#include <boost/bind.hpp>
#include "profiler.h"
#include "logger.h"

namespace Starscape {

	/** A logger that simplifies use of the profiler. */
	class ProfileLogger : public Logger {
	public:
		/** Records profiling results in the Logger's history. */
		void LogResults() {
			Profiler::GenerateResultsList();
			for_each(Profiler::GetResultsList().begin(), Profiler::GetResultsList().end()
					,boost::bind(&ProfileLogger::InsertStatistics, this, _1));
		}
		/** Resets the state of the profiler */
		void ResetProfiler()	{ Profiler::Reset(); }
	private:
		void InsertStatistics(Profiler::ProfileHistory* pprof) {
			assert(pprof!=0);
			Insert(pprof->GetLevel()); Insert(": ");
			Insert(pprof->GetName()); Insert(": ");
			Insert(pprof->NumSamples()); Flush();
			Insert("  Total Time (ms) = "); Insert(pprof->TotalTime()); Flush();
			Insert("  Avg Time (ms) = "); Insert(pprof->AvgTime()); Flush();
			Insert("  Max Time (ms) = "); Insert(pprof->MaxTime()); Flush();
			Insert("  Min Time (ms) = "); Insert(pprof->MinTime()); Flush();
			Insert("  % of Parent Time = "); Insert(pprof->RatioOfParentTime()*100.0f); Flush(); Flush();
		}
	};
}

#endif
