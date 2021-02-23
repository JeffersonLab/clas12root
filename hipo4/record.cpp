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
  * File:   record.cpp
  * Author: gavalian
  *
  * Created on April 11, 2017, 4:47 PM
  */

#include "record.h"
//#include "hipoexceptions.h"

#ifdef __LZ4__
#include <lz4.h>
#endif

namespace hipo {


    record::record()= default;


    record::~record()= default;

    /**
     * Read
     */
    void record::readRecord(std::ifstream &stream, long position, int dataOffset){

        recordHeaderBuffer.resize(80);
        stream.seekg(position,std::ios::beg);

        stream.read( (char *) &recordHeaderBuffer[0],80);
        recordHeader.recordLength     = *(reinterpret_cast<int *>(&recordHeaderBuffer[ 0]));
        recordHeader.headerLength     = *(reinterpret_cast<int *>(&recordHeaderBuffer[ 8]));
        recordHeader.numberOfEvents   = *(reinterpret_cast<int *>(&recordHeaderBuffer[12]));
        recordHeader.bitInfo          = *(reinterpret_cast<int *>(&recordHeaderBuffer[20]));
        recordHeader.signatureString  = *(reinterpret_cast<int *>(&recordHeaderBuffer[28]));
        recordHeader.recordDataLength = *(reinterpret_cast<int *>(&recordHeaderBuffer[32]));
        recordHeader.userHeaderLength = *(reinterpret_cast<int *>(&recordHeaderBuffer[24]));
        int compressedWord            = *(reinterpret_cast<int *>(&recordHeaderBuffer[36]));

        if(recordHeader.signatureString==0xc0da0100) recordHeader.dataEndianness = 0;
        if(recordHeader.signatureString==0x0001dac0) recordHeader.dataEndianness = 1;

        if(recordHeader.signatureString==0x0001dac0){
          recordHeader.recordLength     = __builtin_bswap32(recordHeader.recordLength);
          recordHeader.headerLength     = __builtin_bswap32(recordHeader.headerLength);
          recordHeader.numberOfEvents   = __builtin_bswap32(recordHeader.numberOfEvents);
          recordHeader.recordDataLength = __builtin_bswap32(recordHeader.recordDataLength);
          recordHeader.userHeaderLength = __builtin_bswap32(recordHeader.userHeaderLength);
          recordHeader.bitInfo          = __builtin_bswap32(recordHeader.bitInfo);
          compressedWord                = __builtin_bswap32(compressedWord);
        }

        int compressedDataLengthPadding = (recordHeader.bitInfo>>24)&0x00000003;
        int headerLengthBytes     = recordHeader.headerLength*4;
        int dataBufferLengthBytes = recordHeader.recordLength * 4 - headerLengthBytes;

        recordHeader.userHeaderLengthPadding    = (recordHeader.bitInfo>>20)&0x00000003;
        recordHeader.recordDataLengthCompressed = compressedWord&0x0FFFFFFF;
        recordHeader.compressionType            = (compressedWord>>28)&0x0000000F;
        recordHeader.indexDataLength            = 4*recordHeader.numberOfEvents;

        /*printf(" allocating buffer for record, size = %d, padding = %d length = %d type = %d nevents = %d data length = %d\n",
          dataBufferLengthBytes,
          compressedDataLengthPadding, recordHeader.recordDataLengthCompressed*4,
          recordHeader.compressionType, recordHeader.numberOfEvents, recordHeader.recordDataLength);
          */
        //char *compressedBuffer    = (char*) malloc(dataBufferLengthBytes);

        if(dataBufferLengthBytes>recordCompressedBuffer.size()){
          int newSize = dataBufferLengthBytes + 5*1024;
          //printf("---> resizing internal compressed buffer size to from %ld to %d\n",
          //   recordCompressedBuffer.size(), newSize);
          recordCompressedBuffer.resize(newSize);
        }
        //dataBufferLengthBytes    -= compressedDataLengthPadding;
        long  dataposition = position + headerLengthBytes;
        //printf("position = %ld data position = %ld\n",position, dataposition);
        stream.seekg(dataposition,std::ios::beg);
        //stream.read( compressedBuffer, dataBufferLengthBytes);
        stream.read( (&recordCompressedBuffer[0]), dataBufferLengthBytes);
        //showBuffer(compressedBuffer, 10, 200);
        //printf("position = %ld data position = %ld \n",position, dataposition);
        int decompressedLength = recordHeader.indexDataLength +
                                 recordHeader.userHeaderLength +
                                 recordHeader.userHeaderLengthPadding +
                                 recordHeader.recordDataLength;

        if(recordBuffer.size()<decompressedLength){
          //printf(" resizing internal buffer from %lu to %d\n", recordBuffer.size(), recordHeader.recordDataLength);
          recordBuffer.resize(decompressedLength+1024);
        }
        //readBenchmark.pause();
        //for(int i = 0; i < recordBuffer.size(); i++) recordBuffer[i] = 0;
        //printf("****************** BEFORE padding = %d\n", compressedDataLengthPadding);
        //showBuffer(&recordBuffer[0], 10, 200);
        //unzipBenchmark.resume();
        if(recordHeader.compressionType==0){
          printf("compression type = 0 data length = %d\n",decompressedLength);
          memcpy((&recordBuffer[0]),(&recordCompressedBuffer[0]),decompressedLength);
        } else {
          int unc_result = getUncompressed((&recordCompressedBuffer[0]) , (&recordBuffer[0]),
				      dataBufferLengthBytes-compressedDataLengthPadding,
					         decompressedLength);
        }
        //unzipBenchmark.pause();
        //printf("******************\n");
        //showBuffer(&recordBuffer[0], 10, 200);
        //char *uncompressedBuffer  = getUncompressed(compressedBuffer,dataBufferLengthBytes,recordHeader.recordDataLength);
        //printf(" decompression size = %d  error = %d\n", unc_result,
	       //unc_result - decompressedLength);
        //free(compressedBuffer);
        /**
         * converting index array from lengths of each buffer in the
         * record to relative positions in the record stream.
         */

        int eventPosition = 0;
        for(int i = 0; i < recordHeader.numberOfEvents; i++){
            auto *ptr = reinterpret_cast<int*>(&recordBuffer[i*4]);
            int size = *ptr;
            if(recordHeader.dataEndianness==1) size = __builtin_bswap32(size);
            eventPosition += size;
            *ptr = eventPosition;
        }
	      //printf("final position = %d\n",eventPosition);
    }

    bool  record::readRecord(std::ifstream &stream, long position, int dataOffset, long inputSize){

      readBenchmark.resume();
      if((position+80)>=inputSize) return false;

      recordHeaderBuffer.resize(80);
      stream.seekg(position,std::ios::beg);

      stream.read( (char *) &recordHeaderBuffer[0],80);
      recordHeader.recordLength     = *(reinterpret_cast<int *>(&recordHeaderBuffer[0]));
      recordHeader.headerLength     = *(reinterpret_cast<int *>(&recordHeaderBuffer[8]));
      recordHeader.numberOfEvents   = *(reinterpret_cast<int *>(&recordHeaderBuffer[12]));
      recordHeader.bitInfo          = *(reinterpret_cast<int *>(&recordHeaderBuffer[20]));
      recordHeader.signatureString  = *(reinterpret_cast<int *>(&recordHeaderBuffer[28]));
      recordHeader.recordDataLength = *(reinterpret_cast<int *>(&recordHeaderBuffer[32]));
      recordHeader.userHeaderLength = *(reinterpret_cast<int *>(&recordHeaderBuffer[24]));
      int compressedWord            = *(reinterpret_cast<int *>(&recordHeaderBuffer[36]));

      if(recordHeader.signatureString==0xc0da0100) recordHeader.dataEndianness = 0;
      if(recordHeader.signatureString==0x0001dac0) recordHeader.dataEndianness = 1;

      if(recordHeader.signatureString==0x0001dac0){
        recordHeader.recordLength = __builtin_bswap32(recordHeader.recordLength);
        recordHeader.headerLength = __builtin_bswap32(recordHeader.headerLength);
        recordHeader.numberOfEvents = __builtin_bswap32(recordHeader.numberOfEvents);
        recordHeader.recordDataLength = __builtin_bswap32(recordHeader.recordDataLength);
        recordHeader.userHeaderLength = __builtin_bswap32(recordHeader.userHeaderLength);
        recordHeader.bitInfo          = __builtin_bswap32(recordHeader.bitInfo);
        compressedWord                = __builtin_bswap32(compressedWord);
      }

      int compressedDataLengthPadding = (recordHeader.bitInfo>>24)&0x00000003;
      int headerLengthBytes           = recordHeader.headerLength*4;
      int dataBufferLengthBytes       = recordHeader.recordLength * 4 - headerLengthBytes;

      recordHeader.userHeaderLengthPadding    = (recordHeader.bitInfo>>20)&0x00000003;
      recordHeader.recordDataLengthCompressed = compressedWord&0x0FFFFFFF;
      recordHeader.compressionType            = (compressedWord>>28)&0x0000000F;
      recordHeader.indexDataLength            = 4*recordHeader.numberOfEvents;

      /*printf(" allocating buffer for record, size = %d, padding = %d length = %d type = %d nevents = %d data length = %d\n",
        dataBufferLengthBytes,
        compressedDataLengthPadding, recordHeader.recordDataLengthCompressed*4,
        recordHeader.compressionType, recordHeader.numberOfEvents, recordHeader.recordDataLength);
        */
      //char *compressedBuffer    = (char*) malloc(dataBufferLengthBytes);

      if(dataBufferLengthBytes>recordCompressedBuffer.size()){
        //-- resize the buffer extending it by 500kB. hopefully
        //-- will be enough not to resize too many times.
        int newSize = dataBufferLengthBytes + 500*1024;
        // printout commented out --- by G.G.
        /* printf("---> resizing internal compressed buffer size to from %ld to %d\n",
           recordCompressedBuffer.size(), newSize); */
        recordCompressedBuffer.resize(newSize);
      }

      long  dataposition = position + headerLengthBytes;
      stream.seekg(dataposition,std::ios::beg);

      if(position+dataBufferLengthBytes+recordHeader.headerLength>inputSize){
        printf("**** warning : record at position %ld is incomplete.",position);
         return false;
      }

      stream.read( (&recordCompressedBuffer[0]), dataBufferLengthBytes);

      int decompressedLength = recordHeader.indexDataLength +
                               recordHeader.userHeaderLength +
                               recordHeader.userHeaderLengthPadding +
                               recordHeader.recordDataLength;

      if(recordBuffer.size()<decompressedLength){
        // Buffer resizes by 20% --- modified by me
        int resize_fraction = (int) (decompressedLength*0.2);
        //printf(" resizing internal buffer from %lu to %d\n", recordBuffer.size(), recordHeader.recordDataLength);
        recordBuffer.resize(decompressedLength + resize_fraction);
      }
      //for(int i = 0; i < recordBuffer.size(); i++) recordBuffer[i] = 0;
      //printf("****************** BEFORE padding = %d\n", compressedDataLengthPadding);
      //showBuffer(&recordBuffer[0], 10, 200);
      readBenchmark.pause();
      unzipBenchmark.resume();
      if(recordHeader.compressionType==0){
        printf("compression type = 0 data length = %d\n",decompressedLength);
        memcpy((&recordBuffer[0]),(&recordCompressedBuffer[0]),decompressedLength);
      } else {
        int unc_result = getUncompressed((&recordCompressedBuffer[0]) , (&recordBuffer[0]),
            dataBufferLengthBytes-compressedDataLengthPadding,
                 decompressedLength);
      }
      unzipBenchmark.pause();
      //printf("******************\n");
      //showBuffer(&recordBuffer[0], 10, 200);
      //char *uncompressedBuffer  = getUncompressed(compressedBuffer,dataBufferLengthBytes,recordHeader.recordDataLength);
      //printf(" decompression size = %d  error = %d\n", unc_result,
       //unc_result - decompressedLength);
      //free(compressedBuffer);
      /**
       * converting index array from lengths of each buffer in the
       * record to relative positions in the record stream.
       */
      indexBenchmark.resume();
      int eventPosition = 0;
      for(int i = 0; i < recordHeader.numberOfEvents; i++){
          auto *ptr = reinterpret_cast<int*>(&recordBuffer[i*4]);
          int size = *ptr;
          if(recordHeader.dataEndianness==1) size = __builtin_bswap32(size);
          eventPosition += size;
          *ptr = eventPosition;
      }
      indexBenchmark.pause();
      return true;
    }

    int   record::getRecordSizeCompressed(){
      return recordHeader.recordLength;
    }

    void  record::readRecord__(std::ifstream &stream, long position, long recordLength){

      stream.seekg(position,std::ios::beg);

      if(recordLength>recordCompressedBuffer.size()){
        int newSize = recordLength + 5*1024;
        //printf("---> resizing internal compressed buffer size to from %ld to %d\n",
           //recordCompressedBuffer.size(), newSize);
           //printf("---> after printout\n");
        recordCompressedBuffer.resize(newSize);
      }
      //printf(" trying seeksg\n");
      stream.seekg( position,std::ios::beg);
      //printf(" trying read\n");
      stream.read( (&recordCompressedBuffer[0]), recordLength);
      //printf(" readin was successfull....\n");
      //stream.read( (char *) &recordHeaderBuffer[0],80);
      recordHeader.recordLength    = *(reinterpret_cast<int *>(&recordCompressedBuffer[0]));
      recordHeader.headerLength    = *(reinterpret_cast<int *>(&recordCompressedBuffer[8]));
      recordHeader.numberOfEvents  = *(reinterpret_cast<int *>(&recordCompressedBuffer[12]));
      recordHeader.bitInfo         = *(reinterpret_cast<int *>(&recordCompressedBuffer[20]));
      recordHeader.signatureString  = *(reinterpret_cast<int *>(&recordCompressedBuffer[28]));
      recordHeader.recordDataLength = *(reinterpret_cast<int *>(&recordCompressedBuffer[32]));
      recordHeader.userHeaderLength = *(reinterpret_cast<int *>(&recordCompressedBuffer[24]));
      int compressedWord            = *(reinterpret_cast<int *>(&recordCompressedBuffer[36]));

      if(recordHeader.signatureString==0xc0da0100) recordHeader.dataEndianness = 0;
      if(recordHeader.signatureString==0x0001dac0) recordHeader.dataEndianness = 1;

      if(recordHeader.signatureString==0x0001dac0){
        recordHeader.recordLength = __builtin_bswap32(recordHeader.recordLength);
        recordHeader.headerLength = __builtin_bswap32(recordHeader.headerLength);
        recordHeader.numberOfEvents = __builtin_bswap32(recordHeader.numberOfEvents);
        recordHeader.recordDataLength = __builtin_bswap32(recordHeader.recordDataLength);
        recordHeader.userHeaderLength = __builtin_bswap32(recordHeader.userHeaderLength);
        recordHeader.bitInfo          = __builtin_bswap32(recordHeader.bitInfo);
        compressedWord                = __builtin_bswap32(compressedWord);
      }

      int compressedDataLengthPadding = (recordHeader.bitInfo>>24)&0x00000003;
      int headerLengthBytes     = recordHeader.headerLength*4;
      int dataBufferLengthBytes = recordHeader.recordLength * 4 - headerLengthBytes;

      recordHeader.userHeaderLengthPadding = (recordHeader.bitInfo>>20)&0x00000003;
      recordHeader.recordDataLengthCompressed = compressedWord&0x0FFFFFFF;
      recordHeader.compressionType            = (compressedWord>>28)&0x0000000F;
      recordHeader.indexDataLength            = 4*recordHeader.numberOfEvents;

      /*printf(" allocating buffer for record, size = %d, padding = %d length = %d type = %d nevents = %d data length = %d\n",
        dataBufferLengthBytes,
        compressedDataLengthPadding, recordHeader.recordDataLengthCompressed*4,
        recordHeader.compressionType, recordHeader.numberOfEvents, recordHeader.recordDataLength);
        */
      //char *compressedBuffer    = (char*) malloc(dataBufferLengthBytes);


      //dataBufferLengthBytes    -= compressedDataLengthPadding;
      //printf(" record header intialized....\n");
      long  dataposition = position + headerLengthBytes;
      //printf("position = %ld data position = %ld\n",position, dataposition);
      //stream.seekg(dataposition,std::ios::beg);
      //stream.read( compressedBuffer, dataBufferLengthBytes);
      //stream.read( (&recordCompressedBuffer[0]), dataBufferLengthBytes);
      //showBuffer(compressedBuffer, 10, 200);
      //printf("position = %ld data position = %ld \n",position, dataposition);
      int decompressedLength = recordHeader.indexDataLength +
                               recordHeader.userHeaderLength +
                               recordHeader.userHeaderLengthPadding +
                               recordHeader.recordDataLength;
      //printf(" decompressed length = %d\n",decompressedLength);
      if(recordBuffer.size()<decompressedLength){
        //printf(" resizing internal buffer from %lu to %d\n", recordBuffer.size(), recordHeader.recordDataLength);
        recordBuffer.resize(decompressedLength+1024);
      }
      //for(int i = 0; i < recordBuffer.size(); i++) recordBuffer[i] = 0;
      //printf("****************** BEFORE padding = %d\n", compressedDataLengthPadding);
      //showBuffer(&recordBuffer[0], 10, 200);
      if(recordHeader.compressionType==0){
        //printf("compression type = 0 data length = %d\n",decompressedLength);
        memcpy((&recordBuffer[0]),(&recordCompressedBuffer[0]),decompressedLength);
      } else {
        //printf(" running deompression %d %d %d\n",dataBufferLengthBytes-compressedDataLengthPadding,decompressedLength,dataposition);
        int unc_result = getUncompressed((&recordCompressedBuffer[56]) , (&recordBuffer[0]),
            dataBufferLengthBytes-compressedDataLengthPadding,
                 decompressedLength);
        //printf("end running deompression %d\n",unc_result);
      }
      //printf("******************\n");
      //showBuffer(&recordBuffer[0], 10, 200);
      //char *uncompressedBuffer  = getUncompressed(compressedBuffer,dataBufferLengthBytes,recordHeader.recordDataLength);
      //printf(" decompression size = %d  error = %d\n", unc_result,
       //unc_result - decompressedLength);
      //free(compressedBuffer);
      /**
       * converting index array from lengths of each buffer in the
       * record to relative positions in the record stream.
       */
       //printf(" deompression ..... ok \n");
      int eventPosition = dataposition;
      for(int i = 0; i < recordHeader.numberOfEvents; i++){
          auto *ptr = reinterpret_cast<int*>(&recordBuffer[i*4]);
          int size = *ptr;
          if(recordHeader.dataEndianness==1) size = __builtin_bswap32(size);
          eventPosition += size;
          *ptr = eventPosition;
      }
      //printf("final position = %d\n",eventPosition);
    }
    /**
     * returns number of events in the record.
     */
    int   record::getEventCount(){
      return recordHeader.numberOfEvents;
    }
    /**
     * reads content of the event with given index into a vector
     * vector will be resized to fit the data. The resulting
     * size of the vector can be used to veryfy the successfull read.
    */
    void  record::readEvent( std::vector<char> &vec, int index){

    }

    /**
     * returns a data object that points to the event inside of the
     * record. For given index the data object will be filled with the
     * pointer to the position in the buffer where the event starts and
     * with the size indicating length of the event.
    */
    void  record::getData(hipo::data &data, int index){
        int first_position = 0;
        if(index > 0){
          first_position  = *(reinterpret_cast<uint32_t *>(&recordBuffer[(index -1)*4]));
        }
        int last_position = *(reinterpret_cast<uint32_t *>(&recordBuffer[index*4]));
        int offset        = recordHeader.indexDataLength
                          + recordHeader.userHeaderLength
                          + recordHeader.userHeaderLengthPadding;
        data.setDataPtr(&recordBuffer[first_position+offset]);
        data.setDataSize(last_position-first_position);
        data.setDataOffset(first_position + offset);
    }

    void  record::readHipoEvent(hipo::event &event, int index){
          hipo::data event_data;
          getData(event_data,index);
          event.init(event_data.getDataPtr(), event_data.getDataSize());
    }
    /**
     * prints the content of given buffer in HEX format. Used for debugging.
     */
    void  record::showBuffer(const char *data, int wrapping, int maxsize)
    {
      for(int i = 0; i < maxsize; i++){
        printf("%X ", 0x000000FF&((unsigned int) data[i]));
        if( (i+1)%wrapping==0) printf("\n");
      }
      printf("\n");
    }
    /**
     * decompresses the buffer given with pointed *data, into a destination array
     * provided. The arguments indicate the compressed data length (dataLength),
     * and maximum decompressed length.
     * returns the number of bytes that were decompressed by LZ4
     */
    int  record::getUncompressed(const char *data,  char *dest, int dataLength, int dataLengthUncompressed){
      #ifdef __LZ4__
        int result = LZ4_decompress_safe(data,dest,dataLength,dataLengthUncompressed);
        //int result = LZ4_decompress_fast(data,dest,dataLengthUncompressed);
        return result;
        #endif

        #ifndef __LZ4__
          printf("\n   >>>>> LZ4 compression is not supported.");
          printf("\n   >>>>> check if libz4 is installed on your system.");
          printf("\n   >>>>> recompile the library with liblz4 installed.\n");
          return NULL;
        #endif

    }
    /**
     * deompresses the content of given buffer ( *data), into a newly allocated
     * memory. User is responsible for free-ing the allocated memory.
     */
    char *record::getUncompressed(const char *data, int dataLength,
                                  int dataLengthUncompressed){

      #ifdef __LZ4__
        char *output = (char *) malloc(dataLengthUncompressed);
        int   result = LZ4_decompress_safe(data,output,dataLength,dataLengthUncompressed);
        //int   result = LZ4_decompress_fast(data,output,dataLengthUncompressed);
        return output;
        //printf(" FIRST (%d) = %x %x %x %x\n",result);//,destUnCompressed[0],destUnCompressed[1],
        //destUnCompressed[2],destUnCompressed[3]);
        //LZ4_decompress_fast(buffer,destUnCompressed,decompressedLength);
        //LZ4_uncompress(buffer,destUnCompressed,decompressedLength);
        #endif

        #ifndef __LZ4__
          printf("\n   >>>>> LZ4 compression is not supported.");
          printf("\n   >>>>> check if libz4 is installed on your system.");
          printf("\n   >>>>> recompile the library with liblz4 installed.\n");
          return nullptr;
        #endif

    }


}
