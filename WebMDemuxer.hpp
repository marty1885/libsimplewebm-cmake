/*
	MIT License

	Copyright (c) 2016 Błażej Szczygieł

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#ifndef WEBMDEMUXER_HPP
#define WEBMDEMUXER_HPP

#include <stddef.h>

#include <stdio.h>
#include <mkvparser/mkvparser.h>

namespace mkvparser {
	class IMkvReader;
	class Segment;
	class Cluster;
	class Block;
	class BlockEntry;
	class VideoTrack;
	class AudioTrack;
}

class MkvReader : public mkvparser::IMkvReader
{
public:
	MkvReader(const char *filePath) :
		m_file(fopen(filePath, "rb"))
	{}
	~MkvReader()
	{
		if (m_file)
			fclose(m_file);
	}

	int Read(long long pos, long len, unsigned char *buf)
	{
		if (!m_file)
			return -1;
		fseek(m_file, pos, SEEK_SET);
		const size_t size = fread(buf, 1, len, m_file);
		if (size < size_t(len))
			return -1;
		return 0;
	}
	int Length(long long *total, long long *available)
	{
		if (!m_file)
			return -1;
		const off_t pos = ftell(m_file);
		fseek(m_file, 0, SEEK_END);
		if (total)
			*total = ftell(m_file);
		if (available)
			*available = ftell(m_file);
		fseek(m_file, pos, SEEK_SET);
		return 0;
	}

private:
	FILE *m_file;
};

class WebMFrame
{
	WebMFrame(const WebMFrame &) = delete;
	void operator =(const WebMFrame &) = delete;
public:
	WebMFrame();
	~WebMFrame();

	inline bool isValid() const
	{
		return bufferSize > 0;
	}

	long bufferSize, bufferCapacity;
	unsigned char *buffer;
	double time;
	bool key;
};

class WebMDemuxer
{
	WebMDemuxer(const WebMDemuxer &) = delete;
	void operator =(const WebMDemuxer &) = delete;
public:
	enum VIDEO_CODEC
	{
		NO_VIDEO,
		VIDEO_VP8,
		VIDEO_VP9
	};
	enum AUDIO_CODEC
	{
		NO_AUDIO,
		AUDIO_VORBIS,
		AUDIO_OPUS
	};

	WebMDemuxer(MkvReader *reader, int videoTrack = 0, int audioTrack = 0);
	~WebMDemuxer();

	inline bool isOpen() const
	{
		return m_isOpen;
	}
	inline bool isEOS() const
	{
		return m_eos;
	}

	double getLength() const;

	VIDEO_CODEC getVideoCodec() const;
	int getWidth() const;
	int getHeight() const;

	AUDIO_CODEC getAudioCodec() const;
	const unsigned char *getAudioExtradata(size_t &size) const; // Needed for Vorbis
	double getSampleRate() const;
	int getChannels() const;
	int getAudioDepth() const;

	bool readFrame(WebMFrame *videoFrame, WebMFrame *audioFrame);

private:
	inline bool notSupportedTrackNumber(long videoTrackNumber, long audioTrackNumber) const;

    MkvReader *m_reader;
	mkvparser::Segment *m_segment;

	const mkvparser::Cluster *m_cluster;
	const mkvparser::Block *m_block;
	const mkvparser::BlockEntry *m_blockEntry;

	int m_blockFrameIndex;

	const mkvparser::VideoTrack *m_videoTrack;
	VIDEO_CODEC m_vCodec;

	const mkvparser::AudioTrack *m_audioTrack;
	AUDIO_CODEC m_aCodec;

	bool m_isOpen;
	bool m_eos;
};

#endif // WEBMDEMUXER_HPP
