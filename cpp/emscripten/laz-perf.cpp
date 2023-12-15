// laz-perf.cpp
// javascript bindings for laz-perf
//

#include <emscripten/bind.h>
#include <iostream>

#include "header.hpp"
#include "readers.hpp"
#include "writers.hpp"

using namespace emscripten;

class LASZip
{
	public:
		LASZip()
        {}

		void open(unsigned int b, size_t len)
        {
			char *buf = (char*) b;
			mem_file_.reset(new lazperf::reader::mem_file(buf, len));
		}

		void getPoint(int buf)
        {
			char *pbuf = reinterpret_cast<char*>(buf);
			mem_file_->readPoint(pbuf);
		}

		unsigned int getCount() const
        {
			return static_cast<unsigned int>(mem_file_->pointCount());
		}

        unsigned int getPointLength() const
        {
			return static_cast<unsigned int>(mem_file_->header().point_record_length);
        }

        unsigned int getPointFormat() const
        {
			return static_cast<unsigned int>(mem_file_->header().point_format_id);
        }

	private:
		std::shared_ptr<lazperf::reader::mem_file> mem_file_;
};

class LASZipWriter
{
public: 
    LASZipWriter() {}

    void open(unsigned int b, unsigned int hb, size_t chunk_size) {
        char *buf = (char*) b;
        char *header_buf = (char*) hb;
        mem_file_.reset(new lazperf::writer::mem_file(buf, header_buf, chunk_size));
    }

    void close() {
        mem_file_->close();
    }

    void writePoint(int buf) {
        char *pbuf = reinterpret_cast<char*>(buf);
        mem_file_->writePoint(pbuf);
    }

    unsigned int getBytesWritten() const
    {
        return static_cast<unsigned int>(mem_file_->bytesWritten());
    }
    
	private:
		std::shared_ptr<lazperf::writer::mem_file> mem_file_;
};

class ChunkDecoder
{
public:
    ChunkDecoder()
    {}

    void open(int pdrf, int point_length, unsigned int inputBuf)
    {
        int ebCount = point_length - lazperf::baseCount(pdrf);
        char *buf = reinterpret_cast<char *>(inputBuf);
        decomp_.reset(new lazperf::reader::chunk_decompressor(pdrf, ebCount, buf));
    }

    void getPoint(unsigned int outBuf)
    {
        char *buf = reinterpret_cast<char *>(outBuf);
        decomp_->decompress(buf);
    }

private:
    std::shared_ptr<lazperf::reader::chunk_decompressor> decomp_;
};

EMSCRIPTEN_BINDINGS(my_module) {
	class_<LASZip>("LASZip")
		.constructor()
		.function("open", &LASZip::open)
        .function("getPointLength", &LASZip::getPointLength)
        .function("getPointFormat", &LASZip::getPointFormat)
		.function("getPoint", &LASZip::getPoint)
		.function("getCount", &LASZip::getCount);
    
    class_<LASZipWriter>("LASZipWriter")
        .constructor()
        .function("open", &LASZipWriter::open)
        .function("close", &LASZipWriter::close)
        .function("writePoint", &LASZipWriter::writePoint)
        .function("getBytesWritten", &LASZipWriter::getBytesWritten);

    class_<ChunkDecoder>("ChunkDecoder")
        .constructor()
        .function("open", &ChunkDecoder::open)
        .function("getPoint", &ChunkDecoder::getPoint);
}

