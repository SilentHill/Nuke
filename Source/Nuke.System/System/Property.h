
#pragma once

namespace Nuke::System
{
    template <typename ValueType>
    class Property 
    {
    public:
        virtual ~Property() = default;
        virtual ValueType& operator= (const ValueType& f) 
        {
            return value = f; 
        }
        virtual const ValueType& operator() () const 
        {
            return value; 
        }
        virtual explicit operator const T& () const 
        {
            return value; 
        }
        virtual ValueType* operator->()
        {
            return &value; 
        }
    protected:
        ValueType value;
    };
}