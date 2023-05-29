#pragma once

#include <algorithm>
#include <cassert>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <ostream>
#include <sstream>
#include "Math/TL_Math.h"

class indent_ostream
{
public:
    indent_ostream() = default;

    std::ostringstream stream;
    int indent_level = 0;

    indent_ostream& print_indent()
    {
        *this << std::string("                    ").substr(0, indent_level);
        return *this;
    }

    indent_ostream& print_indent_increase()
    {
        increase_indent();
        return print_indent();
    }

    indent_ostream& print_indent_decrease()
    {
        decrease_indent();
        return print_indent();
    }

    indent_ostream& print_linebreak()
    {
        *this << '\n';
        print_indent();
        return *this;
    }

    void increase_indent() { indent_level += 4; }
    void decrease_indent() { indent_level -= 4; }

#define OVERRIDE_STREAM_OPERATOR(TValue) \
    indent_ostream& operator<<(TValue _value) { stream << _value; return *this; }

    OVERRIDE_STREAM_OPERATOR(short)
    OVERRIDE_STREAM_OPERATOR(unsigned short)
    OVERRIDE_STREAM_OPERATOR(int)
    OVERRIDE_STREAM_OPERATOR(unsigned int)
    OVERRIDE_STREAM_OPERATOR(long)
    OVERRIDE_STREAM_OPERATOR(long long)
    OVERRIDE_STREAM_OPERATOR(float)
    OVERRIDE_STREAM_OPERATOR(double)
    OVERRIDE_STREAM_OPERATOR(long double)
    // OVERRIDE_STREAM_OPERATOR(bool)
    OVERRIDE_STREAM_OPERATOR(char)
    OVERRIDE_STREAM_OPERATOR(std::string)

    friend std::ostream& operator<<(std::ostream& os, indent_ostream& _stream)
    {
        os << _stream.stream.str();
        return os;
    }

    friend indent_ostream& operator<<(indent_ostream& _o1, indent_ostream& _o2)
    {
        // assert(_o1 == _o2);
        return _o1;
    }
};

template <typename TKey, typename TValue>
indent_ostream& operator<<(indent_ostream& os, const std::map<TKey, TValue>& _map)
{
    os << "{\n";
    {
        os.increase_indent();

        auto it = _map.begin();
        if (_map.empty() == false)
        {
            os << it->first << ':' << it->second;
        }

        ++it;
        for (; it != _map.end(); ++it)
        {
            os << ",\n";
            os << it->first << ':' << it->second;
        }
        os.decrease_indent();
    }
    os << '}';
    return os;
}

template <typename T>
indent_ostream& operator<<(indent_ostream& os, const std::vector<T>& _vec)
{
    os << "[\n" << os.print_indent_increase();
    {
        os << "{\n" << os.print_indent_increase();
        os << os.print_indent_increase() << *_vec.begin();
        os << "\n" << os.print_indent_decrease() << "}";

        for (auto it = _vec.begin() + 1; it != _vec.end(); it++)
        {
            os << ",\n";

            os << "{\n";
            os.print_indent_increase();
            os << *it << '\n';
            os.print_indent_decrease();
            os << '}';
        }
        os.decrease_indent();
    }
    os << "\n" << os.print_indent_decrease() << "]";
    return os;
}

class UELevelParser
{
public:
    UELevelParser() = delete;
    ~UELevelParser() = delete;

    struct Object
    {
        std::string name;
        std::string className;
        std::map<std::string, std::string> properties;

        template <typename T>
        T GetPropertyAs(std::string _propertyName)
        {
            std::string _valueString = properties.find(_propertyName);
            return T::Parse(_valueString);
        }
    };

    struct Actor
    {
        std::string name;
        std::vector<Object> components;

        std::vector<Object> GetComponentsOf(const std::string& _componentTypeName) const
        {
            std::vector<Object> _out;

            std::for_each(components.begin(), components.end(), [&_componentTypeName, &_out](const Object _comp)
            {
                if (_comp.className == _componentTypeName)
                    _out.push_back(_comp);
            });

            return _out;
        }
    };

    struct Level
    {
        std::vector<Actor> actors;

        /**
         * \brief 특정 타입의 Component를 가지는 Actor 목록을 반환합니다.
         * \param _componentTypeName 특정 타입의 이름입니다.
         */
        std::vector<Actor> GetActorsHasComponent(const std::string& _componentTypeName) const
        {
            std::vector<Actor> _out;

            for (const auto& a : actors)
            {
                const auto _iter = std::find_if(a.components.begin(), a.components.end(),
                                                [&_componentTypeName](const Object _comp) { return _comp.className == _componentTypeName; });
                if (_iter == a.components.end())
                    continue;

                _out.push_back(a);
            }

            return _out;
        }
    };

    struct Property_String
    {
        std::string value;

        static Property_String Parse(const std::string& _str)
        {
            return Property_String{_str};
        }
    };

    struct Property_QuotedString
    {
        std::string value;

        static Property_QuotedString Parse(const std::string& _str)
        {
            return Property_QuotedString{_str.substr(1, _str.size() - 2)};
        }
    };

    struct Property_PathExtension
    {
        std::string value;

        static Property_PathExtension Parse(const std::string _str)
        {
            return Property_PathExtension{_str.substr(_str.find(".") + 1)};
        }
    };

    struct Property_Bool
    {
        bool value;

        static Property_Bool Parse(const std::string& _str)
        {
            bool _value = _str == "False" ? false : true;

            return Property_Bool{_value};
        }
    };

    struct Property_Vector
    {
        TL_Math::Vector3 value;

        static Property_Vector Parse(std::string _str)
        {
            _str = _str.substr(1, _str.length() - 2); // 여는 괄호와 닫는 괄호 ()를 삭제합니다.

            const size_t _firstComma = _str.find(",");
            const size_t _secondComma = _str.find(",", _firstComma + 1);
            const size_t _firstSubs = _str.find("=");
            const size_t _secondSubs = _str.find("=", _firstSubs + 1);
            const size_t _thirdSubs = _str.find("=", _secondSubs + 1);

            const std::string _xString = _str.substr(_firstSubs + 1, _firstComma - _firstSubs - 1);
            const std::string _yString = _str.substr(_secondSubs + 1, _secondComma - _secondSubs - 1);
            const std::string _zString = _str.substr(_thirdSubs + 1);

            const float _x = std::stof(_xString);
            const float _y = std::stof(_yString);
            const float _z = std::stof(_zString);

            // UE의 좌표계 (z-up 왼손 좌표계) 에서 DirectX의 좌표계 (y-up 왼손 좌표계) 로 변환합니다.
            return Property_Vector{{_y, _z, _x}};
        }
    };

    struct Property_RotationEuler
    {
        TL_Math::Vector3 value;

        static Property_RotationEuler Parse(std::string _str)
        {
            _str = _str.substr(1, _str.length() - 2); // 여는 괄호와 닫는 괄호 ()를 삭제합니다.

            const size_t _firstComma = _str.find(",");
            const size_t _secondComma = _str.find(",", _firstComma + 1);
            const size_t _firstSubs = _str.find("=");
            const size_t _secondSubs = _str.find("=", _firstSubs + 1);
            const size_t _thirdSubs = _str.find("=", _secondSubs + 1);

            const std::string _pitchString = _str.substr(_firstSubs + 1, _firstComma - _firstSubs - 1);
            const std::string _yawString = _str.substr(_secondSubs + 1, _secondComma - _secondSubs - 1);
            const std::string _rollString = _str.substr(_thirdSubs + 1);

            const float _pitch = std::stof(_pitchString);
            const float _yaw = std::stof(_yawString);
            const float _roll = std::stof(_rollString);

            return Property_RotationEuler{{_pitch, _yaw, _roll}};
        }
    };

    struct Property_Map
    {
        std::string key;
        std::string value; // 문자열을 감싸는 '" "' 이 삭제된 형태로 저장합니다.

        static Property_Map Parse(const std::string& _str)
        {
            const size_t _firstApostrophe = _str.find("'");
            const size_t _firstQuotationMark = _firstApostrophe + 1;
            const size_t _secondQuotationMark = _str.find("\"", _firstQuotationMark + 1);
            const std::string _ueClass = _str.substr(0, _firstApostrophe);
            const std::string _classPath = _str.substr(_firstQuotationMark + 1,
                                                       _secondQuotationMark - _firstQuotationMark - 1);

            return Property_Map{_ueClass, _classPath};
        }
    };

    struct Component_StaticMeshComponent
    {
        std::string meshFileNameWithoutExtension;
        TL_Math::Vector3 relativeLocation;
        TL_Math::Vector3 relativeRotation;
        TL_Math::Vector3 relativeScale3D;

        static Component_StaticMeshComponent Parse(const Object _obj)
        {
            auto _iter = _obj.properties.find("StaticMesh");
            if (_iter == _obj.properties.end())
                return {""};

            const std::string _staticMeshString = Property_Map::Parse(_iter->second).value;

            size_t _dotPosition = _staticMeshString.find(".");
            const std::string _staticMeshFileName = _staticMeshString.substr(_dotPosition + 1);

            TL_Math::Vector3 _relativeLocation = TL_Math::Vector3::Zero;
            TL_Math::Vector3 _relativeRotation = TL_Math::Vector3::Zero;
            TL_Math::Vector3 _relativeScale3D = TL_Math::Vector3::One;

            _iter = _obj.properties.find("RelativeLocation");
            if (_iter != _obj.properties.end())
            {
                std::string _relativeLocationString = _iter->second;
                _relativeLocation = Property_Vector::Parse(_relativeLocationString).value;
            }

            _iter = _obj.properties.find("RelativeRotation");
            if (_iter != _obj.properties.end())
            {
                std::string _relativeRotationString = _iter->second;
                _relativeRotation = Property_RotationEuler::Parse(_relativeRotationString).value;
            }

            _iter = _obj.properties.find("RelativeScale3D");
            if (_iter != _obj.properties.end())
            {
                std::string _relativeScale3DString = _iter->second;
                _relativeScale3D = Property_Vector::Parse(_relativeScale3DString).value;
            }

            return Component_StaticMeshComponent{_staticMeshFileName, _relativeLocation, _relativeRotation, _relativeScale3D};
        }
    };

    static Level Parse(std::stringstream& _stream);
};

void ParseObject(std::stringstream& _stream, int _startPos, int _endPos, UELevelParser::Actor& _ownerRef)
{
    std::string _name;
    std::string _className;
    auto _iter = _ownerRef.components.end();

    while (true)
    {
        std::string _token;
        _stream >> _token;

        const int _pos = (int)_stream.tellg();
        if (_pos >= _endPos)
            break;

        if (_token.find("Class") == 0)
        {
            _className = UELevelParser::Property_PathExtension::Parse(_token).value;
        }
        else if (_token.find("Name") == 0)
        {
            assert(_name.empty());

            // Name 토큰 내용을 읽습니다.
            // Name 토큰의 값을 감싸는 쌍따옴표 ("")를 제거한 형태로 저장합니다.
            std::string _name = _token.substr(std::string{"Name"}.size() + 1);
            _name = UELevelParser::Property_QuotedString::Parse(_name).value;

            // 이미 있는 오브젝트인지 확인합니다.
            _iter = std::find_if(_ownerRef.components.begin(), _ownerRef.components.end(),
                                 [&_name](UELevelParser::Object c) { return c.name.find(_name) == 0; });
            if (_iter == _ownerRef.components.end())
            {
                // 없다면 추가합니다.
                UELevelParser::Object _object{_name, _className};
                _ownerRef.components.push_back({_object});
                _iter = _ownerRef.components.end() - 1;
            }
        }
        else if (_token.find("Archetype") == 0
            || _token.find("ActorFolderPath") == 0)
        {
            // Archetype, ActorFolderPath 토큰은 무시합니다.
        }
        else if (_token.find("CustomProperties") == 0)
        {
            // CustomProperties 토큰은 대응하는 값을 가지고 있지 않으므로 무시합니다.
            continue;
        }
        else
        {
            // 그 외 토큰들의 값을 읽어 저장합니다.
            size_t _dividerPos = _token.find('=');

            if (_dividerPos > _token.size())
            {
                // 값이 없는 토큰인 경우 무시합니다.
                continue;
            }

            auto _key = _token.substr(0, _dividerPos);
            auto _value = _token.substr(_dividerPos + 1);
            (*_iter).properties.insert({_key, _value});
        }
    }
}

void ParseActor(std::stringstream& _stream, int _startPos, int _endPos, UELevelParser::Level& _levelRef)
{
    UELevelParser::Actor _actor;

    while (true)
    {
        std::string _token;
        _stream >> _token;

        const int _pos = (int)_stream.tellg();
        if (_pos >= _endPos)
            break;

        if (_token == "Begin")
        {
            std::string _beginForWhat;
            _stream >> _beginForWhat;

            if (_beginForWhat == "Object") // Begin Object
            {
                std::size_t _start = static_cast<int>(_stream.tellg()) + 1;
                std::size_t _end = _stream.str().find("End Object", _start);
                assert(_end < _endPos);

                ParseObject(_stream, _start, _end, _actor);
                _stream.seekg(_end);
            } // End Object
        }
        else if (_token.find("Name") == 0)
        {
            // Name 토큰 내용을 읽습니다.
            std::string _name = _token.substr(std::string{"Name"}.size() + 1);
            _actor.name = _name;
        }
        else
        {
            // 그 외 Class, Archetype, ActorFolderPath 등의 토큰을 무시합니다.
        }
    }

    _levelRef.actors.push_back(_actor);
}

UELevelParser::Level ParseLevel(std::stringstream& _stream, int _startPos, int _endPos)
{
    UELevelParser::Level _level;

    while (true)
    {
        std::string _token;
        _stream >> _token;

        const int _pos = (int)_stream.tellg();
        if (_pos >= _endPos)
            break;

        if (_token == "Begin")
        {
            std::string _beginForWhat;
            _stream >> _beginForWhat;

            if (_beginForWhat == "Actor") // Begin Actor
            {
                std::size_t _start = static_cast<int>(_stream.tellg()) + 1;
                std::size_t _end = _stream.str().find("End Actor", _start);
                assert(_end < _endPos);

                ParseActor(_stream, _start, _end, _level);
                _stream.seekg(_end);
            } // End Actor
        }
    }

    return _level;
}

UELevelParser::Level UELevelParser::Parse(std::stringstream& _stream)
{
    UELevelParser::Level _level;
    int _endPos = 99999999;

    std::string _token;
    _stream >> _token; // Begin
    _stream >> _token; // Map

    _stream >> _token;
    if (_token == "Begin")
    {
        std::string _beginForWhat;
        _stream >> _beginForWhat;

        if (_beginForWhat == "Level") // Begin Level
        {
            std::size_t _start = static_cast<int>(_stream.tellg()) + 1;
            std::size_t _end = _stream.str().find("End Level", _start);
            assert(_end < _endPos);

            _level = ParseLevel(_stream, _start, _end);
        } // End Level
    }

    return _level;
}

std::ostream& operator<<(std::ostream& os, const TL_Math::Vector3& obj)
{
    return os
        << "x: " << obj.x
        << " y: " << obj.y
        << " z: " << obj.z;
}

std::ostream& operator<<(std::ostream& os, const UELevelParser::Property_String& obj)
{
    return os << "value: " << obj.value;
}

std::ostream& operator<<(std::ostream& os, const UELevelParser::Property_QuotedString& obj)
{
    return os << "value: " << obj.value;
}

std::ostream& operator<<(std::ostream& os, const UELevelParser::Property_Bool& obj)
{
    return os << "value: " << obj.value;
}

std::ostream& operator<<(std::ostream& os, const UELevelParser::Property_Vector& obj)
{
    return os << "value: " << obj.value;
}

std::ostream& operator<<(std::ostream& os, const UELevelParser::Property_Map& obj)
{
    return os
        << "key: " << obj.key
        << ", value: " << obj.value;
}

std::ostream& operator<<(std::ostream& os, const UELevelParser::Component_StaticMeshComponent& obj)
{
    return os
        << "meshFileNameWithoutExtension: " << obj.meshFileNameWithoutExtension
        << ", relativeLocation: " << obj.relativeLocation
        << ", relativeScale3D: " << obj.relativeScale3D;
}

indent_ostream& operator<<(indent_ostream& os, const UELevelParser::Object& obj)
{
    os << "name: " << obj.name << ",\n";
    os.print_indent();
    os << "properties: " << obj.properties;
    return os;
}

indent_ostream& operator<<(indent_ostream& os, const UELevelParser::Actor& obj)
{
    os << "name: " << obj.name << ",\n";
    os.print_indent();
    os << "components: " << obj.components;
    return os;
}

indent_ostream& operator<<(indent_ostream& os, const UELevelParser::Level& obj)
{
    return os << "actors: " << obj.actors;
}
