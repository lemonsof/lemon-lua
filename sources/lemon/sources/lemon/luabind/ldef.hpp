/*!
 * \file ldef.hpp
 * \date 2014/04/29 16:16
 *
 * \author yayan
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef LEMON_LUABIND_LDEF_HPP
#define LEMON_LUABIND_LDEF_HPP
#include <list>
#include <string>
#include <type_traits>
#include <lua/lua.hpp>
#include <lemon/nocopyable.hpp>
#include <lemon/luabind/lcall.hpp>
#include <lemon/luabind/lstack.hpp>
namespace lemon{ namespace luabind{

    struct __def
    {
        virtual void process(lua_State *L) const = 0;
    };

    template<typename F> class __function : public __def
    {
    public:
        __function(const char* name, F f) :_name(name), _f(f)
        {

        }

        void process(lua_State *L) const
        {
            luaL_Reg reg [] =
            {
                { _name.c_str(), &__call },
                { nullptr, nullptr }
            };

            lua_pushlightuserdata(L, new_callsite(_f));

            luaL_setfuncs(L, reg, 1);
        }
    private:
        std::string                             _name;
        F                                       _f;
    };

    template<typename R, typename ...Args>
    inline __function<R(*)(Args...)> def(const char*name, R(*f)(Args...))
    {

        return __function<R(*)(Args...)>(name, f);
    }


    template<typename Type> class __class : public __def
    {
    public:
        typedef std::list<__def*>               methods;
        typedef std::list<__def*>               constructors;

        ~__class()
        {
            for (auto def : _methods)
            {
                delete def;
            }

            for (auto def : _constructors)
            {
                delete def;
            }
        }

        template<typename R,typename ...Args>
        __class & def(const char * name,R(Type::*f)(Args...))
        {
            _methods.push_back(new __function<R(Type::*)(Args...)>(name, f));
            return *this;
        }

        template<typename ...Args>
        static constructor_<Type> __constructors(Args ...args)
        {
            return constructor_<Type>{ new Type(args...) };
        }

        template<typename ...Args>
        __class & constructor(const char* name)
        {
            static_assert(std::is_constructible<Type, Args...>::value, "check class's constructor method");
            using return_type = constructor_<Type>;
            _constructors.push_back(new __function<return_type(*)(Args...)>(name, &__class<Type>::__constructors<Args...>));
            return *this;
        }

        static int __collection(lua_State *L)
        {
            delete *reinterpret_cast<Type**>(luaL_checkudata(L, 1, typeid(Type).name()));

            return 0;
        }
        
        void process(lua_State *L) const
        {
            if (!_constructors.empty())
            {
                for (auto def:_constructors)
                {
                    def->process(L);
                }
            }
            
            if (luaL_newmetatable(L, typeid(Type).name()))
            {
                lua_newtable(L);

                for (auto def : _methods)
                {
                    def->process(L);
                }

                lua_setfield(L, -2, "__index");

                if (!_constructors.empty())
                {
                    lua_pushcfunction(L, &__class<Type>::__collection);

                    lua_setfield(L, -2, "__gc");
                }

                lua_setmetatable(L, -2);
            }
        }

    private:
        methods                                 _methods;
        constructors                            _constructors;
    };

    template<typename Type>
    inline __class<Type> class_()
    {
        return __class<Type>();
    }

   
    class __module
    {
    public:
        __module(const __module & rhs) = delete;
        __module & operator = (const __module & rhs) = delete;
        __module & operator = (__module && rhs) = delete;

        __module(__module && rhs)
        {
            _L = rhs._L;
            _name = rhs._name;

            rhs._L = nullptr;
        }

        __module(lua_State*L, const std::string& name) :_L(L),_name(name)
        {
            lua_newtable(L);
        }

        ~__module()
        {
            if (_L != nullptr)
            {
                lua_setglobal(_L, _name.c_str());
            }
        }

        __module &  operator <= (const __def & def)
        {
            def.process(_L);

            return *this;
        }



    private:
        lua_State                                *_L;
        std::string                              _name;
    };

    inline __module module_(lua_State*L,const char * name)
    {
        return __module(L,name);
    }
} }

#endif // LEMON_LUABIND_LDEF_HPP