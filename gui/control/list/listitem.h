// listitem.h
// Gregory Rosenblatt
// 5/19/05

#ifndef Starscape_Gui_ListItem_H_
#define Starscape_Gui_ListItem_H_

#include "gui/control/button/itembutton.h"
#include <boost/bind.hpp>

namespace Starscape {

	namespace Gui {

		template<typename DataType> class ListBox;	// pre-declaration

		template <typename DataType>
		class ListItem : public ItemButton {
		public:
			typedef ListBox<DataType>	ListBoxType;
			ListItem(ListBoxType& parent, const ButtonBrush& brush
					,const char* name, const DataType& data
					,const typename ListBoxType::Iterator& self)
				: ItemButton(brush, boost::bind(&ListBoxType::SelectItem
												,&parent, boost::ref(*this))
							,name)
				,data_(data), self_(self)	{}
			DataType& GetData()	{ return data_; }
			const DataType& GetData() const	{ return data_; }
		private:
			friend class ListBox<DataType>;
			void Remove()	{ RemoveFromParent(); }	// expose to listbox
			DataType						data_;
			typename ListBoxType::Iterator	self_;
		};
	}
}

#endif
