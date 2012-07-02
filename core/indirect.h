// indirect.h
// Gregory Rosenblatt
// 5/18/05

#ifndef Starscape_Indirect_H_
#define Starscape_Indirect_H_

#include <boost/type_traits/add_pointer.hpp>
#include <functional>

namespace Starscape {

	template <typename Func>
	struct Indirect : std::binary_function<
						typename boost::add_pointer<
							typename Func::first_argument_type>::type
						,typename boost::add_pointer<
					  		typename Func::second_argument_type>::type
						,typename Func::result_type> {
		template <typename PtrType1, typename PtrType2>
		typename Func::result_type operator()(const PtrType1& lhs
											,const PtrType2& rhs) const {
			return func(*lhs, *rhs);
		}
		Func func;
	};
}

#endif
