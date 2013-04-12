// profiler.h
// Gregory Rosenblatt
// 1/23/05

#ifndef Starscape_Profiler_H_
#define Starscape_Profiler_H_

/** @namespace Profiler
 * To profile a particular scope, insert the PROFILE(name) macro at
 * the scope's beginning where name is a unique string.
 * 
 * - Getting Results
 * 	-# Make a call to Profiler::GenerateResultsList()
 * 	-# Access individual Profile History entries using Profiler::GetResultsList()
 * 	-# To start again from scratch use Profiler::Reset()
 */

// this definition should be moved to a config header
#define STARSCAPE_PROFILER_ENABLED

#ifdef STARSCAPE_PROFILER_ENABLED
#define PROFILE(name) Starscape::Profiler::ProfileSample profileSample(name);
#else
#define PROFILE(name)
#endif

#include "pooledlist.h"
#include "pooledstring.h"
#include <boost/functional.hpp>
#include <algorithm>
#include <cassert>

namespace Starscape {

	namespace Profiler {

		// pre-declarations
		class ProfileHistory;
		typedef PooledList<ProfileHistory*>::Type	ProfileList;

		/** Calculates results for all profiling entries
		 * Recursively generates statistics for all profiled scopes
		 */
		void GenerateResultsList();
		/** Retrieves the profiling data in the form of a list of entries */
		ProfileList& GetResultsList();
		/** Forces the profiling database to completely reset itself */
		void Reset();

		/** A single sample of data for a specific profile history */
		class ProfileSample {
		public:
			ProfileSample(const char* name);
			~ProfileSample();
		private:
			unsigned int TotalTime() const;
			ProfileHistory&	profile_;
			unsigned int	startTime_;
		};

		/** An accumulation of profiling sample data.
		 * Provides an interface to retrieve profiling statistics.
		 * All time statistics are in milliseconds.
		 */
		class ProfileHistory {
		public:
			ProfileHistory() : parent_(0), name_(""), level_(0), numSamples_(0), totalTime_(0),
				maxTime_(0), minTime_(2147483647)	{}
			/** Retrieves the name of this profiling history. */
			const char* GetName() const		{ return name_.c_str(); }
			/** The recursive scope level of this particular entry. */
			unsigned int GetLevel() const	{ return level_; }
			/** The number of data samples taken to form this history. */
			unsigned int NumSamples() const	{ return numSamples_; }
			/** The total amount of time spent in this entry during run time. */
			unsigned int TotalTime() const	{ return totalTime_; }
			/** The average amount of time spent in this entry each time the profiled scope was entered. */
			unsigned int AvgTime() const	{ if (numSamples_==0) return 0; return totalTime_/numSamples_; }
			/** The most amount of time ever spent in this profiled scope. */
			unsigned int MaxTime() const	{ return maxTime_; }
			/** The least amount of time ever spent in this profiled scope. */
			unsigned int MinTime() const	{ return minTime_; }
			/** The fraction of time this entry used of its parent entry's total time. */
			float RatioOfParentTime() const {
				if (!HasParent())
					return 1.0f;
				if (GetParent().TotalTime()==0)
					return 1.0f;
				return (static_cast<float>(TotalTime())
					/ static_cast<float>(GetParent().TotalTime()));
			}
		private:
			friend class ProfileSample;
			// these functions operate within the profiler database
			friend void AddToResults(ProfileHistory&, unsigned int);
			friend void Profiler::Reset();
			friend ProfileHistory& PushCurrentProfile(const char*);
			friend void PopCurrentProfile();
			// incorporates sample data into the history's statistics
			void AddSample(unsigned int sampleTime) {
				++numSamples_;
				totalTime_+=sampleTime;
				if (sampleTime>maxTime_)
					maxTime_=sampleTime;
				if (sampleTime<minTime_)
					minTime_=sampleTime;
			}
			void SetName(const char* name)	{ name_ = name; }
			void SetLevel(unsigned int level)	{ level_ = level; }
			// All data is returned to its initial state and all children
			// are recursively Reset() as well.
			void Reset() {
				numSamples_=0; totalTime_=0; maxTime_=0; minTime_=0;
				ResetChildren();
				childList_.clear();
				parent_=0; name_=""; level_=0;
			}
			bool HasParent() const	{ return parent_!=0; }
			ProfileHistory& GetParent() const	{ assert(HasParent()); return *parent_; }
			ProfileHistory* GetParentPtr()	{ return parent_; }
			// forces the parent to add this as a child, and then sets the parent for this
			void AddToParent(ProfileHistory& parent) {
				parent.AddChild(*this);
				parent_=&parent;
			}
			// recursively calls Reset() on all children
			void ResetChildren() {
				std::for_each(childList_.begin(), childList_.end()
								,boost::mem_fun(&ProfileHistory::Reset));
			}
			// this history will now recursively manage this child
			void AddChild(ProfileHistory& profile) {
				childList_.push_back(&profile);
			}
			ProfileList::iterator GetChildListBegin()	{ return childList_.begin(); }
			ProfileList::iterator GetChildListEnd()		{ return childList_.end(); }
			// data
			ProfileList		childList_;
			ProfileHistory*	parent_;
			PooledString	name_;
			unsigned int	level_;
			unsigned int	numSamples_;
			unsigned int	totalTime_,
							maxTime_,
							minTime_;
		};
	}
}

#endif
