#include "Audio.h"

Audio::Audio() {}
Audio::~Audio() {}

bool Audio::check_al_errors(const std::string& filename, const std::uint_fast32_t line)
{
	ALenum error = alGetError();
	if (error != AL_NO_ERROR)
	{
		std::cerr << "***ERROR*** (" << filename << ": " << line << ")\n";
		switch (error)
		{
		case AL_INVALID_NAME:
			std::cerr << "AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function";
			break;
		case AL_INVALID_ENUM:
			std::cerr << "AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function";
			break;
		case AL_INVALID_VALUE:
			std::cerr << "AL_INVALID_VALUE: an invalid value was passed to an OpenAL function";
			break;
		case AL_INVALID_OPERATION:
			std::cerr << "AL_INVALID_OPERATION: the requested operation is not valid";
			break;
		case AL_OUT_OF_MEMORY:
			std::cerr << "AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory";
			break;
		default:
			std::cerr << "UNKNOWN AL ERROR: " << error;
		}
		std::cerr << std::endl;
		return false;
	}
	return true;
}

bool Audio::check_alc_errors(const std::string& filename, const std::uint_fast32_t line, ALCdevice* device)
{
	ALCenum error = alcGetError(device);
	if (error != ALC_NO_ERROR)
	{
		std::cerr << "***ERROR*** (" << filename << ": " << line << ")\n";
		switch (error)
		{
		case ALC_INVALID_VALUE:
			std::cerr << "ALC_INVALID_VALUE: an invalid value was passed to an OpenAL function";
			break;
		case ALC_INVALID_DEVICE:
			std::cerr << "ALC_INVALID_DEVICE: a bad device was passed to an OpenAL function";
			break;
		case ALC_INVALID_CONTEXT:
			std::cerr << "ALC_INVALID_CONTEXT: a bad context was passed to an OpenAL function";
			break;
		case ALC_INVALID_ENUM:
			std::cerr << "ALC_INVALID_ENUM: an unknown enum value was passed to an OpenAL function";
			break;
		case ALC_OUT_OF_MEMORY:
			std::cerr << "ALC_OUT_OF_MEMORY: an unknown enum value was passed to an OpenAL function";
			break;
		default:
			std::cerr << "UNKNOWN ALC ERROR: " << error;
		}
		std::cerr << std::endl;
		return false;
	}
	return true;
}

std::int32_t Audio::convert_to_int(char* buffer, std::size_t len)
{
	std::int32_t a = 0;
	if (endian::native == endian::little)
		std::memcpy(&a, buffer, len);
	else
		for (std::size_t i = 0; i < len; ++i)
			reinterpret_cast<char*>(&a)[3 - i] = buffer[i];
	return a;
}

bool Audio::load_wav_file_header(std::ifstream& file,
	std::uint8_t& channels,
	std::int32_t& sampleRate,
	std::uint8_t& bitsPerSample,
	ALsizei& size)
{
	char buffer[4];
	if (!file.is_open())
		return false;

	// the RIFF
	if (!file.read(buffer, 4))
	{
		std::cerr << "ERROR: could not read RIFF" << std::endl;
		return false;
	}
	if (std::strncmp(buffer, "RIFF", 4) != 0)
	{
		std::cerr << "ERROR: file is not a valid WAVE file (header doesn't begin with RIFF)" << std::endl;
		return false;
	}

	// the size of the file
	if (!file.read(buffer, 4))
	{
		std::cerr << "ERROR: could not read size of file" << std::endl;
		return false;
	}

	// the WAVE
	if (!file.read(buffer, 4))
	{
		std::cerr << "ERROR: could not read WAVE" << std::endl;
		return false;
	}
	if (std::strncmp(buffer, "WAVE", 4) != 0)
	{
		std::cerr << "ERROR: file is not a valid WAVE file (header doesn't contain WAVE)" << std::endl;
		return false;
	}

	// "fmt/0"
	if (!file.read(buffer, 4))
	{
		std::cerr << "ERROR: could not read fmt/0" << std::endl;
		return false;
	}

	// this is always 16, the size of the fmt data chunk
	if (!file.read(buffer, 4))
	{
		std::cerr << "ERROR: could not read the 16" << std::endl;
		return false;
	}

	// PCM should be 1?
	if (!file.read(buffer, 2))
	{
		std::cerr << "ERROR: could not read PCM" << std::endl;
		return false;
	}

	// the number of channels
	if (!file.read(buffer, 2))
	{
		std::cerr << "ERROR: could not read number of channels" << std::endl;
		return false;
	}
	channels = convert_to_int(buffer, 2);

	// sample rate
	if (!file.read(buffer, 4))
	{
		std::cerr << "ERROR: could not read sample rate" << std::endl;
		return false;
	}
	sampleRate = convert_to_int(buffer, 4);

	// (sampleRate * bitsPerSample * channels) / 8
	if (!file.read(buffer, 4))
	{
		std::cerr << "ERROR: could not read (sampleRate * bitsPerSample * channels) / 8" << std::endl;
		return false;
	}

	// ?? dafaq
	if (!file.read(buffer, 2))
	{
		std::cerr << "ERROR: could not read dafaq" << std::endl;
		return false;
	}

	// bitsPerSample
	if (!file.read(buffer, 2))
	{
		std::cerr << "ERROR: could not read bits per sample" << std::endl;
		return false;
	}
	bitsPerSample = convert_to_int(buffer, 2);

	// data chunk header "data"
	if (!file.read(buffer, 4))
	{
		std::cerr << "ERROR: could not read data chunk header" << std::endl;
		return false;
	}
	if (std::strncmp(buffer, "data", 4) != 0)
	{
		std::cerr << "ERROR: file is not a valid WAVE file (doesn't have 'data' tag)" << std::endl;
		return false;
	}

	// size of data
	if (!file.read(buffer, 4))
	{
		std::cerr << "ERROR: could not read data size" << std::endl;
		return false;
	}
	size = convert_to_int(buffer, 4);

	/* cannot be at the end of file */
	if (file.eof())
	{
		std::cerr << "ERROR: reached EOF on the file" << std::endl;
		return false;
	}
	if (file.fail())
	{
		std::cerr << "ERROR: fail state set on the file" << std::endl;
		return false;
	}

	return true;
}

char* Audio::load_wav(const std::string& filename,
	std::uint8_t& channels,
	std::int32_t& sampleRate,
	std::uint8_t& bitsPerSample,
	ALsizei& size)
{
	std::ifstream in(filename, std::ios::binary);
	if (!in.is_open())
	{
		std::cerr << "ERROR: Could not open \"" << filename << "\"" << std::endl;
		return nullptr;
	}
	if (!load_wav_file_header(in, channels, sampleRate, bitsPerSample, size))
	{
		std::cerr << "ERROR: Could not load wav header of \"" << filename << "\"" << std::endl;
		return nullptr;
	}

	char* data = new char[size];

	in.read(data, size);

	return data;
}

const std::size_t NUM_BUFFERS = 4;
const std::size_t BUFFER_SIZE = 65536; // 32kb of data in each buffer

void Audio::update_stream(const ALuint source,
	const ALenum& format,
	const std::int32_t& sampleRate,
	const std::vector<char>& soundData,
	std::size_t& cursor)
{
	ALint buffersProcessed = 0;
	alCall(alGetSourcei, source, AL_BUFFERS_PROCESSED, &buffersProcessed);

	if (buffersProcessed <= 0)
		return;

	while (buffersProcessed--)
	{
		ALuint buffer;
		alCall(alSourceUnqueueBuffers, source, 1, &buffer);

		ALsizei dataSize = BUFFER_SIZE;

		char* data = new char[dataSize];
		std::memset(data, 0, dataSize);

		std::size_t dataSizeToCopy = BUFFER_SIZE;
		if (cursor + BUFFER_SIZE > soundData.size())
			dataSizeToCopy = soundData.size() - cursor;

		std::memcpy(&data[0], &soundData[cursor], dataSizeToCopy);
		cursor += dataSizeToCopy;

		if (dataSizeToCopy < BUFFER_SIZE)
		{
			cursor = 0;
			std::memcpy(&data[dataSizeToCopy], &soundData[cursor], BUFFER_SIZE - dataSizeToCopy);
			cursor = BUFFER_SIZE - dataSizeToCopy;
		}

		alCall(alBufferData, buffer, format, data, BUFFER_SIZE, sampleRate);
		alCall(alSourceQueueBuffers, source, 1, &buffer);

		delete[] data;
	}
}

int Audio::playSound(std::string soundToPlay)
{
	ALCdevice* openALDevice = alcOpenDevice(nullptr);
	if (!openALDevice)
		return 0;

	ALCcontext* openALContext;
	if (!alcCall(alcCreateContext, openALContext, openALDevice, openALDevice, nullptr) || !openALContext)
	{
		std::cerr << "ERROR: Could not create audio context" << std::endl;
		return 0;
	}
	ALCboolean contextMadeCurrent = false;
	if (!alcCall(alcMakeContextCurrent, contextMadeCurrent, openALDevice, openALContext)
		|| contextMadeCurrent != ALC_TRUE)
	{
		std::cerr << "ERROR: Could not make audio context current" << std::endl;
		return 0;
	}

	std::uint8_t channels;
	std::int32_t sampleRate;
	std::uint8_t bitsPerSample;
	ALsizei dataSize;
	char* rawSoundData = load_wav(soundToPlay, channels, sampleRate, bitsPerSample, dataSize);
	if (rawSoundData == nullptr || dataSize == 0)
	{
		std::cerr << "ERROR: Could not load wav" << std::endl;
		return 0;
	}
	std::vector<char> soundData(rawSoundData, rawSoundData + dataSize);

	ALuint buffers[NUM_BUFFERS];

	alCall(alGenBuffers, NUM_BUFFERS, &buffers[0]);

	ALenum format;

	if (channels == 1 && bitsPerSample == 8)
		format = AL_FORMAT_MONO8;
	else if (channels == 1 && bitsPerSample == 16)
		format = AL_FORMAT_MONO16;
	else if (channels == 2 && bitsPerSample == 8)
		format = AL_FORMAT_STEREO8;
	else if (channels == 2 && bitsPerSample == 16)
		format = AL_FORMAT_STEREO16;
	else
	{
		std::cerr
			<< "ERROR: unrecognised wave format: "
			<< channels << " channels, "
			<< bitsPerSample << " bps" << std::endl;
		return 0;
	}

	for (std::size_t i = 0; i < NUM_BUFFERS; ++i)
	{
		alCall(alBufferData, buffers[i], format, &soundData[i * BUFFER_SIZE], BUFFER_SIZE, sampleRate);
	}

	ALuint source;
	alCall(alGenSources, 1, &source);
	alCall(alSourcef, source, AL_PITCH, 1);
	alCall(alSourcef, source, AL_GAIN, 1.0f);
	alCall(alSource3f, source, AL_POSITION, 0, 0, 0);
	alCall(alSource3f, source, AL_VELOCITY, 0, 0, 0);
	alCall(alSourcei, source, AL_LOOPING, AL_FALSE);

	alCall(alSourceQueueBuffers, source, NUM_BUFFERS, &buffers[0]);

	alCall(alSourcePlay, source);

	ALint state = AL_PLAYING;

	std::size_t cursor = BUFFER_SIZE * NUM_BUFFERS;

	while (state == AL_PLAYING)
	{
		update_stream(source, format, sampleRate, soundData, cursor);
		alCall(alGetSourcei, source, AL_SOURCE_STATE, &state);
	}

	alCall(alDeleteSources, 1, &source);
	alCall(alDeleteBuffers, NUM_BUFFERS, &buffers[0]);

	alcCall(alcMakeContextCurrent, contextMadeCurrent, openALDevice, nullptr);
	alcCall(alcDestroyContext, openALDevice, openALContext);

	ALCboolean closed;
	//alcCall(alcCloseDevice, closed, openALDevice, openALDevice);

	return 0;
}
