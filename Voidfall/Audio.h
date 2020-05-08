#pragma once

#include <AL/al.h>
#include <AL/alc.h>

#include <cstring>
#include <iostream>
#include <fstream>
#include <type_traits>
#include <vector>

#define alCall(function, ...) alCallImpl(__FILE__, __LINE__, function, __VA_ARGS__)
#define alcCall(function, device, ...) alcCallImpl(__FILE__, __LINE__, function, device, __VA_ARGS__)


#ifndef Audio_h
#define Audio_h

class Audio
{
public:

    Audio();
    ~Audio();

    bool check_al_errors(const std::string& filename, const std::uint_fast32_t line);

    bool check_alc_errors(const std::string& filename, const std::uint_fast32_t line, ALCdevice* device);

    bool load_wav_file_header(std::ifstream& file, std::uint8_t& channels, std::int32_t& sampleRate, std::uint8_t& bitsPerSample, ALsizei& size);

    std::int32_t convert_to_int(char* buffer, std::size_t len);

    char* load_wav(const std::string& filename,
        std::uint8_t& channels,
        std::int32_t& sampleRate,
        std::uint8_t& bitsPerSample,
        ALsizei& size);

    void update_stream(const ALuint source,
        const ALenum& format,
        const std::int32_t& sampleRate,
        const std::vector<char>& soundData,
        std::size_t& cursor);

    template<typename alFunction, typename... Params>
    auto alCallImpl(const char* filename,
        const std::uint_fast32_t line,
        alFunction function,
        Params... params)
        ->typename std::enable_if_t<!std::is_same_v<void, decltype(function(params...))>, decltype(function(params...))>{
        auto ret = function(std::forward<Params>(params)...);
        check_al_errors(filename, line);
        return ret;
    }

    template<typename alFunction, typename... Params>
    auto alCallImpl(const char* filename,
        const std::uint_fast32_t line,
        alFunction function,
        Params... params)
        ->typename std::enable_if_t<std::is_same_v<void, decltype(function(params...))>, bool>{
        function(std::forward<Params>(params)...);
        return check_al_errors(filename, line);
    }

    template<typename alcFunction, typename... Params>
    auto alcCallImpl(const char* filename,
        const std::uint_fast32_t line,
        alcFunction function,
        ALCdevice* device,
        Params... params)
        ->typename std::enable_if_t<std::is_same_v<void, decltype(function(params...))>, bool>{
        function(std::forward<Params>(params)...);
        return check_alc_errors(filename, line, device);
    }

    template<typename alcFunction, typename ReturnType, typename... Params>
    auto alcCallImpl(const char* filename,
        const std::uint_fast32_t line,
        alcFunction function,
        ReturnType& returnValue,
        ALCdevice* device,
        Params... params)
        ->typename std::enable_if_t<!std::is_same_v<void, decltype(function(params...))>, bool>{
        returnValue = function(std::forward<Params>(params)...);
        return check_alc_errors(filename, line, device);
    }

    int playSound(std::string soundToPlay);

    enum class endian
    {
#ifdef _WIN32
        little = 0,
        big = 1,
        native = little
#else
        little = __ORDER_LITTLE_ENDIAN__,
        big = __ORDER_BIG_ENDIAN__,
        native = __BYTE_ORDER__
#endif
    };
};

#endif /* Audio_h */
