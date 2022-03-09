//******************************************************************************
//*       ██╗  ██╗██╗██████╗  ██████╗     ██╗  ██╗    ██████╗                  *
//*       ██║  ██║██║██╔══██╗██╔═══██╗    ██║  ██║   ██╔═████╗                 *
//*       ███████║██║██████╔╝██║   ██║    ███████║   ██║██╔██║                 *
//*       ██╔══██║██║██╔═══╝ ██║   ██║    ╚════██║   ████╔╝██║                 *
//*       ██║  ██║██║██║     ╚██████╔╝         ██║██╗╚██████╔╝                 *
//*       ╚═╝  ╚═╝╚═╝╚═╝      ╚═════╝          ╚═╝╚═╝ ╚═════╝                  *
//************************ Jefferson National Lab (2017) ***********************
/*
 *   Copyright (c) 2017.  Jefferson Lab (JLab). All rights reserved. Permission
 *   to use, copy, modify, and distribute  this software and its documentation
 *   for educational, research, and not-for-profit purposes, without fee and
 *   without a signed licensing agreement.
 *
 *   IN NO EVENT SHALL JLAB BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL
 *   INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING
 *   OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF JLAB HAS
 *   BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *   JLAB SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *   PURPOSE. THE HIPO DATA FORMAT SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF
 *   ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS". JLAB HAS NO OBLIGATION TO
 *   PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 *   This software was developed under the United States Government license.
 *   For more information contact author at gavalian@jlab.org
 *   Department of Experimental Nuclear Physics, Jefferson Lab.
 */

/*
 * File:   record.h
 * Author: gavalian
 *
 * Created on April 11, 2017, 4:47 PM
 */

#ifndef HIPORECORD_H
#define HIPORECORD_H

#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>

#include "event.h"
#include "utils.h"

namespace hipo {

    typedef struct recordHeader_t {
      int signatureString{}; // 1) identifier string is HREC (int = 0x43455248
      int recordLength{}; // 2) TOTAL Length of the RECORD, includes INDEX array
      int recordDataLength{}; // 3) Length of the DATA uncompressed
      int recordDataLengthCompressed{}; // 4) compressed length of the DATA buffer
      int numberOfEvents{} ; // 5) number of event, data buckets in DATA buffer
      int headerLength{} ; // 6) Length of the buffer represengin HEADER for the record
      int indexDataLength{} ; // 7) Length of the index buffer (in bytes)
      int userHeaderLength{}; // user header length in bytes
      int userHeaderLengthPadding{}; // the padding added to user header Length
      int bitInfo{};
      int compressionType{};
      int compressedLengthPadding{};
      int dataEndianness{};
    } recordHeader_t;

    class data {
      private:
      const char  *data_ptr{};
      int          data_size{};
      int          data_endianness{};
      int          data_offset{};

      public:
        data(){ data_ptr = nullptr; data_size = 0;}
        ~data()= default;

        void setDataPtr(const char *__ptr){ data_ptr = __ptr;}
        void setDataSize(int __size){ data_size = __size;}
        void setDataOffset(int __offset) { data_offset = __offset;}
        void setDataEndianness(int __endianness) { data_endianness = __endianness;}

        const uint32_t   *getEvioPtr(){ return reinterpret_cast<const uint32_t *>(data_ptr);}
        int         getEvioSize(){ return (int) data_size/4 ;}
        const char *getDataPtr(){ return data_ptr;}
        int         getDataSize(){ return data_size;}
        int         getDataEndianness(){ return data_endianness;}
        int         getDataOffset(){ return data_offset;}

    };

    class record {

      private:

        //std::vector< std::vector<char> > eventBuffer;
        std::vector<char>  recordHeaderBuffer;
        recordHeader_t     recordHeader{};

        std::vector<char>  recordBuffer;
        std::vector<char>  recordCompressedBuffer;

        hipo::benchmark                 readBenchmark;
        hipo::benchmark                 unzipBenchmark;
        hipo::benchmark                 indexBenchmark;

        char *getUncompressed(const char *data, int dataLength, int dataLengthUncompressed);
        int   getUncompressed(const char *data, char *dest, int dataLength, int dataLengthUncompressed);
        void  showBuffer(const char *data, int wrapping, int maxsize);

    public:

        record();
        ~record();

        void  readRecord(std::ifstream &stream, long position, int dataOffset);
        void  readRecord__(std::ifstream &stream, long position, long recordLength);
        bool  readRecord(std::ifstream &stream, long position, int dataOffset, long inputSize);
        int   getEventCount();
        int   getRecordSizeCompressed();

        void  readEvent( std::vector<char> &vec, int index);
        void  readHipoEvent(hipo::event &event, int index);
        void  getData(   hipo::data &data, int index);

        hipo::benchmark  &getReadBenchmark(){ return readBenchmark;}
        hipo::benchmark  &getUnzipBenchmark(){ return unzipBenchmark;}
        hipo::benchmark  &getIndexBenchmark(){ return indexBenchmark;}
    };
}
#endif /* HIPORECORD_H */
