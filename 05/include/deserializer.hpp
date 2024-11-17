template<typename T>
typename std::enable_if<!std::is_array<T>::value, Deserializer&>::type
operator<<(Deserializer& ser, const T& obj)
{
#ifdef DEBUG
    std::cout << "Deserializing object of " << typeid(T).name() << std::endl;
#endif
    ser.load(obj);
    return ser;
}

template<typename T>
typename std::enable_if<std::is_array<T>::value, Deserializer&>::type
operator<<(Deserializer& ser, const T& arr)
{
#ifdef DEBUG
    std::cout << "Deserializing array of " << typeid(T).name() << " size " << std::size(arr) << std::endl;
#endif
    for (const auto& val : arr) {
        ser.load(val);
    }
    return ser;
}

template<typename T>
void Deserializer::read(T& arg)
{
#ifdef DEBUG
    std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
    in_ >> arg;
}

template<>
void Deserializer::read<bool>(bool& arg)
{
#ifdef DEBUG
    std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
    in_ >> std::boolalpha >> arg;
}

template<>
void Deserializer::read<std::string>(std::string& arg)
{
#ifdef DEBUG
    std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
    char c;
    std::string str;
    std::string_view::size_type size;
    in_ >> size;
    in_.read(&c, sizeof(c));
    arg.resize(size);
    in_.read(const_cast<char*>(arg.data()), size); // hack to avoid unnecessary memcpy: read directly to string buffer
}

template<typename T, typename... Args>
Deserializer::Error Deserializer::process(T& first, Args& ... args)
{
#ifdef DEBUG
    std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
    Error err = process(first);
    if (err!=NoError) {
        return err;
    }
    return process(args...);
}

template<typename T>
typename std::enable_if<!is_serializable<T, void(Deserializer&)>::value, Deserializer::Error>::type
Deserializer::process(T& arg)
{
#ifdef DEBUG
    std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
    read(arg);
    return !in_ ? CorruptedArchive : NoError;
}

template<typename T>
typename std::enable_if<is_serializable<T, void(Deserializer&)>::value, Deserializer::Error>::type
Deserializer::process(T& arg)
{
#ifdef DEBUG
    std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
    return load(arg);
}