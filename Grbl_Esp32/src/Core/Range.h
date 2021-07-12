#pragma once
#ifndef __CORE_RANGE_H__
#define __CORE_RANGE_H__

#include <utility>
#include "Object.h"

namespace Core {

    template <class Iterator>
    class Range : public Object	{
    public:								
        typedef Iterator IteratorType;
		typedef typename std::iterator_traits<Iterator>::value_type ValueType;
		typedef typename std::iterator_traits<Iterator>::difference_type DifferenceType;						
	    typedef RangeBase<Iterator> SelfType;

    private:			
        IteratorType m_start;
        IteratorType m_end;
	
    public:
        Range(Iterator start, Iterator end) :
            m_start(start),
            m_end(end) {				                
        }

        Range(const SelfType& other) :
            m_start(other.m_start),
            m_end(other.m_end) {
        }

        Range(SelfType&& other) :
            m_start(std::move(other.m_start)),
            m_end(std::move(other.m_end)) {
        }

        virtual ~RangeBase() {
        }

        inline IteratorType start() const noexcept { return this->m_start; }
        inline IteratorType end() const noexcept { return this->m_end; }

        inline SelfType subRange(INT skip = 0, INT take = 0) const noexcept { 
            auto start = this->m_start + skip;
            auto end = !take ? this->m_end : start + take;
            return SelfType(start, end);
        }

        inline SelfType skip(INT count) const { return this->subRange(count); }			
        inline SelfType take(INT count) const { return this->subRange(0, count); }

        template <class Func>
        inline IteratorType find(Func func) const {
            return std::find_if(this->start(), this->end(), func);
        }

        template <class Func>
        inline bool exists(Func func) const	{
            return std::any_of(this->start(), this->end(), func);
        }

        template <class F>
        inline F forEach(F func) {
            return std::for_each(this->m_start, this->m_end, func);
        }
    };

    
};

#endif