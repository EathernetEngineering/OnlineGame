#pragma once
#include <common/Common.hpp>

namespace cee {
	namespace net {
		template<typename T>
		class TsQueue {
		public:
			TsQueue() = default;
			TsQueue(const TsQueue<T>&) = delete;
			virtual ~TsQueue() { clear(); }
			
		public:
			const T& front()
			{
				std::scoped_lock lock(m_MuxQueue);
				return m_DeqQueue.front();
			}
			
			const T& back()
			{
				std::scoped_lock lock(m_MuxQueue);
				return m_DeqQueue.back();
			}
			
			T pop_front()
			{
				std::scoped_lock lock(m_MuxQueue);
				auto t = std::move(m_DeqQueue.front());
				m_DeqQueue.pop_front();
				return t;
			}
			
			T pop_back()
			{
				std::scoped_lock lock(m_MuxQueue);
				auto t = std::move(m_DeqQueue.back());
				m_DeqQueue.pop_back();
				return t;
			}
			
			void push_front(const T& item)
			{
				std::scoped_lock lock(m_MuxQueue);
				m_DeqQueue.emplace_front(std::move(item));
				
				std::unique_lock<std::mutex> ul(m_MuxBlocking);
				m_CvBlocking.notify_one();
			}
			
			void push_back(const T& item)
			{
				std::scoped_lock lock(m_MuxQueue);
				m_DeqQueue.emplace_back(std::move(item));
				
				std::unique_lock<std::mutex> ul(m_MuxBlocking);
				m_CvBlocking.notify_one();
			}
			
			bool empty()
			{
				std::scoped_lock lock(m_MuxQueue);
				return m_DeqQueue.empty();
			}
			
			size_t count()
			{
				std::scoped_lock lock(m_MuxQueue);
				return m_DeqQueue.size();
			}
			
			void clear()
			{
				std::scoped_lock lock(m_MuxQueue);
				m_DeqQueue.clear();
			}
			
			void wait()
			{
				while (empty())
				{
					std::unique_lock<std::mutex> ul(m_MuxBlocking);
					m_CvBlocking.wait(ul);
				}
			}
			
		protected:
			std::mutex m_MuxQueue;
			std::deque<T> m_DeqQueue;
			std::condition_variable m_CvBlocking;
			std::mutex m_MuxBlocking;
		};
	}
}
