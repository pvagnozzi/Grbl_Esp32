#pragma once
#ifndef __VALUE_H__
#define __VALUE_H__

#include <utility>

#include "Object.h"
#include "Equatable.h"
#include "Assignable.h"

namespace Core {

    template <typename T>
    class Value : 
        public Object, 
        public Equatable<T>,
        public Equatable<Value<T>>,
        public Assignable<T>,
        public Assignable<Value<T>> {
    private:
        T m_value;

    public:
        typedef Value<T> SelfType;
        typedef Equatable<T> ValueEquatableType;
        typedef Equatable<Value<T>> SelfEquatableType;
        typedef Assignable<T> ValueAssignableType;
        typedef Assignable<Value<T>> SelfAssignableType;

        typedef typename ValueEquatableType::EqualsParameterType ValueEquatableParameterType;
        typedef typename SelfEquatableType::EqualsParameterType SelfEquatableParameterType;
        typedef typename ValueAssignableType::AssignableParameterType ValueAssignableParameterType;
        typedef typename SelfAssignableType::AssignableParameterType SelfAssignableParameterType;

        inline Value() :
            m_value() {
        }

        inline Value(T value) :
            m_value(value) {
        }

        inline Value(const SelfType& other) :
            m_value(other.m_value) {
        }

        inline Value(SelfType&& other) :
            m_value(std::move(other.m_value)) {
        }

        virtual ~Value() = default;        

        bool equals(ValueEquatableParameterType) const override {
            return m_value == value;
        }

        bool equals(SelfEquatableParameterType other) const override {
            return m_value == other.m_value;
        }

        void assign(ValueAssignableParameterType value) override {
            m_value = value;
        }

        void assign(SelfAssignableParameterType other) override {
            m_value = other.m_Value;
        }

        inline T value() const noexcept {
            return m_value;
        }

        inline operator T() const noexcept {
            return m_value;
        }

        inline SelfType& operator =(SelfAssignableParameterType other) {
            assign(other);
            return *this;
        }

        inline SelfType& operator =(ValueAssignableParameterType value) {
            assign(value);
            return *this;
        }

        inline SelfType& operator =(SelfType&& other) {
            m_value = std::move(other.m_value);
            return *this;
        }
    };
};

#endif