// listbox.h
// Gregory Rosenblatt
// 5/19/05

#ifndef Starscape_Gui_ListBox_H_
#define Starscape_Gui_ListBox_H_

#include "gui/appearance/list/listboxbrush.h"
#include "gui/control/list/listitem.h"
#include "pooledlist.h"
#include "indirect.h"
#include <boost/shared_ptr.hpp>
#include <boost/iterator/indirect_iterator.hpp>
#include <algorithm>

namespace Starscape {

	namespace Gui {

		template <typename DataType>
		class ListBox : public Window {
		public:
			typedef ListItem<DataType>	ItemType;
		private:
			typedef boost::shared_ptr<ItemType>	ItemPtr;
			typedef typename PooledList<ItemPtr>::Type	ItemList;
		public:
			typedef typename ItemList::iterator	Iterator;
			enum { DEFAULT_NAME_LENGTH = 16
					,DEFAULT_NUM_ITEMS = 8 };
			ListBox(const ListBoxBrush& brush
					,const Command& onSelect = Command()
					,unsigned int charsPerName = DEFAULT_NAME_LENGTH
					,unsigned int numItems = DEFAULT_NUM_ITEMS)
				: selected_(0), onSelect_(onSelect), brush_(brush), nextPos_(0)
			{
				Size size = brush_.GetPreferredSize(charsPerName, numItems);
				SetSize(size.x, size.y);
			}
			virtual ~ListBox()	{}
			void AddItem(const char* name, const DataType& data) {
				Iterator itr = list_.insert(list_.end(), ItemPtr());
				ItemPtr& item = list_.back();
				item.reset(new ItemType(*this, brush_.GetItemButtonBrush()
										,name, data, itr));
				AddChild(*item);
				SetItemWidth(*item);
				MoveItemToNextPos(*item);
			}
			void RemoveItem(ItemType& item) {
				assert(item.self_!=list_.end());
				item.Remove();
				list_.erase(item.self_);
			}
			const ItemType& GetSelectedItem() const {
				assert(selected_!=0);
				return *selected_;
			}
			unsigned int Count() const	{ return list_.size(); }
			void Sort() {
				BasicSort();
				Reposition(list_.begin(), list_.end());
			}
			void ReverseSort() {
				BasicSort();
				Reposition(list_.rbegin(), list_.rend());
			}
			void OnResize() {
				std::for_each(boost::make_indirect_iterator(list_.begin())
							,boost::make_indirect_iterator(list_.end())
							,boost::bind(&ItemType::SetWidth, _1, Width()));
			}
		private:
			ItemType& GetSelectedItem() {
				assert(selected_!=0);
				return *selected_;
			}
			void SetItemWidth(ItemType& item) {
				item.SetWidth(Width());
			}
			void MoveItemToNextPos(ItemType& item) {
				item.SetPosY(nextPos_);
				nextPos_ += item.Height();
			}
			void BasicSort() {
				list_.sort(Indirect<std::less<ItemType> >());
			}
			template <class IteratorType>
			void Reposition(IteratorType itr, IteratorType end) {
				nextPos_ = 0;
				std::for_each(boost::make_indirect_iterator(itr)
							,boost::make_indirect_iterator(end)
							,boost::bind(&ListBox<DataType>::MoveItemToNextPos
										,this, _1));
			}
			friend class ListItem<DataType>;
			void OnSelect() const	{ if (onSelect_) onSelect_(); }
			virtual void SelectItem(ItemType& item) {
				if (selected_) {
					if (selected_ != &item)
						GetSelectedItem().Deselect();
				}
				selected_ = &item;
				OnSelect();
			}
			void OnRender()	{ brush_.Draw(GetRenderingRect()); }
			ListBox(const ListBox&);	// noncopyable
			void operator=(const ListBox&);
			ItemList			list_;
			ItemType*			selected_;
			Command				onSelect_;
			const ListBoxBrush&	brush_;
			CoordScalar			nextPos_;
		};
	}
}

#endif
