#pragma once
#ifndef __VALUE_H__
#define __VALUE_H__

#include "Object.h"
#include "Equatable.h"
#include "Comparable.h"
#include "Assignable.h"

namespace Core {

    template <typename T>
    class Value :
        public Object,
        public Equatable<T>,
        public Equatable<Value<T>>,
        public Assignable<T>,
        public Assignable<Value<T>> {
    public:
        typedef T ValueType;

    private:
        ValueType m_value;

    public:
        typedef Value<T> SelfType;

        inline Value() :
            m_value() {
        }

        inline Value(ValueType value) :
            m_value(value) {
        }

        inline Value(const SelfType& other) :
            m_value(other.m_value) {
        }

        bool equals(const ValueType value) const noexcept override {
            return m_value == value;
        }

        bool equals(const Value<T>& value) const noexcept override {
            return m_value == value.m_value;
        }

        void assign(const Value<T> value) noexcept override {
            m_value = value.m_value;
        }

        void assign(T value) noexcept override {
            m_value = value;
        }

        inline operator ValueType() const noexcept {
            return m_value;
        }

        inline ValueType value() const noexcept {
            return m_value;
        }

        inline void value(ValueType value) noexcept {
            m_value = value;
        }

        inline Value<T>&& operator =(T value) noexcept {
            assign(value);
            return *this;
        }

        inline Value<T>&& operator =(const Value<T>& value) noexcept {
            assign(value);
            return *this;
        }

    };
}

#endif
